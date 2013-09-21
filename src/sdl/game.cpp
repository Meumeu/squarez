/*
 * Squarez puzzle game
 * Copyright (C) 2013  Guillaume Meunier <guillaume.meunier@centraliens.net>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "game.h"
#include "window.h"
#include <stdexcept>
#include <chrono>
#include <iostream>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#define NB_SYMBOLS 3
#define BOARD_SIZE 8

#ifndef DATADIR // for kdevelop
#error DATADIR not defined
#define DATADIR
#endif

const squarez::Colour symbol_colors[] =
{
	squarez::Colour(1.0, 0.0, 0.0),
	squarez::Colour(1.0, 0.8, 0.0),
	squarez::Colour(0.1, 0.2, 1.0),
	squarez::Colour(1.0, 0.2, 1.0),
	squarez::Colour(0.0, 1.0, 0.0),
	squarez::Colour(0.0, 1.0, 1.0)
};

static void set_cell_color(squarez::Cell& c, bool hover, bool selected)
{
	if (c.symbol < 0 || c.symbol >= NB_SYMBOLS)
		throw std::runtime_error("invalid symbol");

	squarez::Colour col = symbol_colors[c.symbol];
	
	if (selected)
	{
		c.colour = col;
		c.border_colour = squarez::Colour(0, 0, 0);
		c.alpha = 1.0;
		c.border_alpha = 1.0;
		c.border_size = 0.2;
	}
	else if (hover)
	{
		c.colour = col;
		c.border_colour = col * 0.7;
		c.alpha = 0.7;
		c.border_alpha = 1.0;
		c.border_size = 0.2;
	}
	else
	{
		c.colour = col;
		c.alpha = 1.0;
		c.border_size = 0.0;
	}
}

squarez::Game::Game(Window& w) : timer_id(0), user_event_id(0), _window(w), shader_rounded_rectangle(DATADIR "/squarez/roundrect.vert.glsl", DATADIR "/squarez/roundrect.frag.glsl"),
	vbo_id(0), board(BOARD_SIZE, NB_SYMBOLS)
{
	timer_id = SDL_AddTimer(20, timerCallback, this);
	if (!timer_id)
		throw std::runtime_error(SDL_GetError());
	
	user_event_id = SDL_RegisterEvents(1);
	
	glGenBuffers(1, &vbo_id);
	if (!vbo_id)
		throw std::runtime_error("Cannot create VBO");
	
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	windowResized(_window.width(), _window.height());
	
	cells.resize(BOARD_SIZE);
	for(int x = 0; x < BOARD_SIZE; ++x)
	{
		for(int y = 0; y < BOARD_SIZE; ++y)
		{
			cells[x].emplace_back(&shader_rounded_rectangle, vbo_id);
			Cell& c = cells[x][y];
			c.symbol = board.get(x, y);
			c.size.setInstantly(0.8);
			c.border_radius = 0.5;
			set_cell_color(c, false, false);
			
			c.angle = 0;
			c.x.setInstantly(x + 0.5);
			c.y.setInstantly(y + 0.5);
			
			c.size.setPeriod(std::chrono::milliseconds(300));
			c.x.setPeriod(std::chrono::milliseconds(300));
			c.y.setPeriod(std::chrono::milliseconds(300));
		}
	}
}

squarez::Game::~Game()
{
	if (vbo_id)
		glDeleteBuffers(1, &vbo_id);
		
	if (timer_id)
		SDL_RemoveTimer(timer_id);
}

Uint32 squarez::Game::timerCallback(Uint32 interval, void* param)
{
	Game * g = (Game*)param;
	
	SDL_Event e;
	memset(&e, 0, sizeof(e));
	e.type = g->user_event_id;
	e.user.code = 0;
	SDL_PushEvent(&e);
	
	return 1;
}

void squarez::Game::mouseDown(int x, int y, int button)
{
	float fx = x * xmax / width;
	float fy = y * ymax / height;
	
	for(unsigned int i = 0; i < BOARD_SIZE; ++i)
	{
		for(unsigned int j = 0; j < BOARD_SIZE; ++j)
		{
			Cell& c = cells[i][j];
			if (fabs(fx - c.x) < c.size / 2 && fabs(fy - c.y) < c.size / 2)
			{
				current_selection.addPoint(i, j);
			}
		}
	}
	
	resetCellColours(x, y);
	selectionChanged(current_selection);
}

void squarez::Game::mouseMoved(int x, int y)
{
	resetCellColours(x, y);
}

void squarez::Game::resetCellColours(int x, int y)
{
	float fx = x * xmax / width;
	float fy = y * ymax / height;
	
	for(unsigned int i = 0; i < BOARD_SIZE; ++i)
	{
		for(unsigned int j = 0; j < BOARD_SIZE; ++j)
		{
			Cell& c = cells[i][j];
			
			bool selected = false;
			for(const std::pair<unsigned int, unsigned int>& p: current_selection.getPoints())
				selected = selected || (p.first == i && p.second == j);
			
			set_cell_color(c, fabs(fx - c.x) < c.size / 2 && fabs(fy - c.y) < c.size / 2, selected);
		}
	}
}

void squarez::Game::windowResized(int _width, int _height)
{
	width = _width;
	height = _height;
	glViewport(0, 0, width, height);
	
	float aspect_ratio = width / height;
	if (aspect_ratio > 1)
	{
		xmax = BOARD_SIZE * aspect_ratio;
		ymax = BOARD_SIZE;
	}
	else
	{
		xmax = BOARD_SIZE;
		ymax = BOARD_SIZE / aspect_ratio;
	}
	
	renderFrame(std::chrono::steady_clock::now());
}


void squarez::Game::run()
{
	auto t0 = std::chrono::steady_clock::now();
	bool running = true;
	while(running)
	{
		SDL_Event e;
		SDL_WaitEvent(&e);
		
		switch(e.type)
		{
			case SDL_QUIT:
				running = false;
				break;
				
			case SDL_KEYDOWN:
				if (e.key.keysym.sym == SDLK_ESCAPE)
					running = false;
				break;
				
			case SDL_WINDOWEVENT:
				if (e.window.event == SDL_WINDOWEVENT_RESIZED)
					windowResized(e.window.data1, e.window.data2);
				break;
				
			case SDL_MOUSEBUTTONDOWN:
				mouseDown(e.button.x, e.button.y, e.button.button);
				break;
			
			case SDL_MOUSEMOTION:
				mouseMoved(e.motion.x, e.motion.y);
				break;
				
			default:
				if (e.type == user_event_id)
				{
					timeTick(std::chrono::steady_clock::now() - t0);
					renderFrame(std::chrono::steady_clock::now());
					SDL_FlushEvent(user_event_id);
					
				}
		}
	}
}


void squarez::Game::renderFrame(std::chrono::time_point<std::chrono::steady_clock> t)
{
	squarez::Animatable::setTime(t);
	
	
	glClearColor(0.88, 0.88, 0.88, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	
	for(auto& i: cells)
	{
		for(auto& j: i)
		{
			j.render(0, 0, xmax, ymax);
		}
	}
	
	for(auto& i: removed_cells)
	{
		i.render(0, 0, xmax, ymax);
	}
	
	for(std::list<Cell>::iterator it = removed_cells.begin(); it != removed_cells.end(); )
	{
		if (it->alpha == 0)
		{
			it = removed_cells.erase(it);
		}
		else
		{
			++it;
		}
	}
	
	SDL_GL_SwapWindow(_window.getWindow());
}

void squarez::Game::applyTransition(const squarez::Transition& transition)
{
	auto old_cells = cells;
	for(const squarez::Transition::CellTransition& tr: transition.getCellTransition())
	{
		if (tr.isNew())
		{
			Cell& c = cells[tr._tox][tr._toy];
			c.symbol = tr._symbol;
			
			c.x.setInstantly(tr._fromx + 0.5);
			c.x = tr._tox + 0.5;
			
			c.y.setInstantly(tr._fromy + 0.5);
			c.y = tr._toy + 0.5;
			
			set_cell_color(c, false, false);
		}
		else if (tr._removed)
		{
			removed_cells.push_back(old_cells[tr._fromx][tr._fromy]);
			Cell& c = removed_cells.back();
			
			c.alpha.setPeriod(std::chrono::milliseconds(300));
			set_cell_color(c, false, false);
			c.size = c.size * 5;
			c.alpha = 0;
			c.border_size = 0;
		}
		else
		{
			cells[tr._tox][tr._toy] = old_cells[tr._fromx][tr._fromy];
			cells[tr._tox][tr._toy].x = tr._tox + 0.5;
			cells[tr._tox][tr._toy].y = tr._toy + 0.5;
		}
	}
	
	current_selection = Selection();
	
	board.applyTransition(transition);
}

void squarez::Game::timeTick(std::chrono::duration<float>)
{
}

void squarez::Game::setSelection(const Selection& selection)
{
	current_selection = selection;
}

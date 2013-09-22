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

#include "sdlui.h"
#include "window.h"
#include <shared/rules/rules.h>
#include <stdexcept>
#include <chrono>
#include <iostream>
#include <cmath>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#ifndef DATADIR // for kdevelop
#error DATADIR not defined
#define DATADIR
#endif

const squarez::Colour symbol_colours[] =
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
	if (c.symbol >= sizeof(symbol_colours) / sizeof(symbol_colours[0]))
		throw std::runtime_error("invalid symbol");

	squarez::Colour col = symbol_colours[c.symbol];
	
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

squarez::SdlUI::SdlUI(squarez::Window& w, std::shared_ptr< squarez::Rules > rules) : UI(rules), _window(w), shader_rounded_rectangle(DATADIR "/squarez/roundrect.vert.glsl", DATADIR "/squarez/roundrect.frag.glsl"),
	vbo_id(0)
{
	glGenBuffers(1, &vbo_id);
	if (!vbo_id)
		throw std::runtime_error("Cannot create VBO");
	
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	windowResized(_window.width(), _window.height());
	
	GameBoard const& board = rules->getBoard();
	
	cells.resize(board.size());
	
	for(unsigned int x = 0; x < board.size(); ++x)
	{
		for(unsigned int y = 0; y < board.size(); ++y)
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

squarez::SdlUI::~SdlUI()
{
	if (vbo_id)
		glDeleteBuffers(1, &vbo_id);
}

void squarez::SdlUI::mouseDown(int x, int y, int button)
{
	float fx = x * xmax / width;
	float fy = y * ymax / height;
	
	for(unsigned int i = 0; i < cells.size(); ++i)
	{
		for(unsigned int j = 0; j < cells[i].size(); ++j)
		{
			Cell& c = cells[i][j];
			if (std::fabs(fx - c.x) < c.size / 2 && std::fabs(fy - c.y) < c.size / 2)
			{
				current_selection.addPoint(i, j);
			}
		}
	}
	
	resetCellColours(x, y);
	rules->onSelect(current_selection);
}

void squarez::SdlUI::mouseMoved(int x, int y)
{
	resetCellColours(x, y);
}

void squarez::SdlUI::resetCellColours(int x, int y)
{
	float fx = x * xmax / width;
	float fy = y * ymax / height;
	
	for(unsigned int i = 0; i < cells.size(); ++i)
	{
		for(unsigned int j = 0; j < cells[i].size(); ++j)
		{
			Cell& c = cells[i][j];
			
			bool selected = false;
			for(const std::pair<unsigned int, unsigned int>& p: current_selection.getPoints())
				selected = selected || (p.first == i && p.second == j);
			
			set_cell_color(c, fabs(fx - c.x) < c.size / 2 && fabs(fy - c.y) < c.size / 2, selected);
		}
	}
}

void squarez::SdlUI::windowResized(int _width, int _height)
{
	width = _width;
	height = _height;
	glViewport(0, 0, width, height);
	
	float aspect_ratio = width / height;
	if (aspect_ratio > 1)
	{
		xmax = rules->getBoard().size() * aspect_ratio;
		ymax = rules->getBoard().size();
	}
	else
	{
		xmax = rules->getBoard().size();
		ymax = rules->getBoard().size() / aspect_ratio;
	}
	
	renderFrame(std::chrono::steady_clock::now());
}


void squarez::SdlUI::run()
{
	auto t0 = std::chrono::steady_clock::now();
	bool running = true;
	while(running)
	{
		SDL_Event e;
		while(SDL_PollEvent(&e))
		{
			switch(e.type)
			{
				case SDL_QUIT:
					running = false;
					break;
					
				case SDL_KEYDOWN:
					if (e.key.keysym.sym == SDLK_ESCAPE)
						running = false;
					break;
					
#ifdef USE_SDL2
				case SDL_WINDOWEVENT:
					if (e.window.event == SDL_WINDOWEVENT_RESIZED)
						windowResized(e.window.data1, e.window.data2);
					break;
#else
				case SDL_VIDEORESIZE:
					windowResized(e.resize.w, e.resize.h);
					break;
#endif
					
				case SDL_MOUSEBUTTONDOWN:
					mouseDown(e.button.x, e.button.y, e.button.button);
					break;
				
				case SDL_MOUSEMOTION:
					mouseMoved(e.motion.x, e.motion.y);
					break;
			}
		}
		
		timeTick(std::chrono::steady_clock::now() - t0);
		renderFrame(std::chrono::steady_clock::now());
	}
}


void squarez::SdlUI::renderFrame(std::chrono::time_point<std::chrono::steady_clock> t)
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
	
#ifdef USE_SDL2
	SDL_GL_SwapWindow(_window.getWindow());
#else
	SDL_GL_SwapBuffers();
#endif
}

void squarez::SdlUI::onTransition(const squarez::Transition& transition)
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
}

void squarez::SdlUI::onScoreChanged(int new_score)
{
// TODO
}

void squarez::SdlUI::onSelectionAccepted(const squarez::Selection& selection)
{
// TODO
}


void squarez::SdlUI::timeTick(std::chrono::duration<float>)
{
}

void squarez::SdlUI::setSelection(const Selection& selection)
{
	current_selection = selection;
}

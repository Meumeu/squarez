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

#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <vector>
#include <chrono>

#include "shader.h"
#include "animatable.h"
#include "cell.h"
#include "shared/gameboard.h"

namespace squarez
{
class Window;

class Game
{
private:
	
	SDL_TimerID timer_id;
	Uint32 user_event_id;
	squarez::Window& _window;
	float xmax, ymax;
	float width, height;
	shader shader_rounded_rectangle;
	
	GLuint vbo_id;
	int coords_id;
	int cell_coords_id;
	int color_id;
	int border_radius_id;
	
	static Uint32 timerCallback(Uint32 interval, void * param);
	
	GameBoard board;
	Selection current_selection;
	std::vector<std::vector<Cell>> cells;
	std::vector<Cell> removed_cells;
	
	void renderFrame(std::chrono::time_point<std::chrono::steady_clock> t);
	void mouseDown(int x, int y, int button);
	void mouseMoved(int x, int y);
	void windowResized(int width, int height);
	
protected:
	virtual void selectionChanged(Selection& selection);
	void applyTransition(const Transition& transition);
	
public:
	Game(Window& w);
	virtual ~Game();
	
	void run();
};
}

#endif // GAME_H

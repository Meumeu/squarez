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

#ifdef USE_SDL2
#include <SDL2/SDL.h>
#else
#include <SDL/SDL.h>
#endif

#include <vector>
#include <list>
#include <chrono>

#include "shader.h"
#include "animatable.h"
#include "cell.h"
#include "shared/gameboard.h"
#include "client/ui.h"

namespace squarez
{
class Window;

class SdlUI : public UI
{
private:
	squarez::Window& _window;
	float xmax, ymax;
	float width, height;
	shader shader_rounded_rectangle;
	
	GLuint vbo_id;
	int coords_id;
	int cell_coords_id;
	int color_id;
	int border_radius_id;
	
	Selection current_selection;
	std::vector<std::vector<Cell>> cells;
	std::list<Cell> removed_cells;
	
	void renderFrame(std::chrono::time_point<std::chrono::steady_clock> t);
	void mouseMoved(int x, int y);
	void windowResized(int width, int height);
	
protected:
	void setSelection(const Selection& selection);
	virtual void mouseDown(int x, int y, int button);
	virtual void timeTick(std::chrono::duration<float> t);
	void resetCellColours(int x, int y);
	
public:
	virtual void onTransition(Transition const& transition); // Called before the board is modified
	virtual void onScoreChanged(int new_score);
	virtual void onSelectionAccepted(Selection const& selection);
	
	SdlUI(Window& w, std::shared_ptr<Rules> rules);
	virtual ~SdlUI();
	
	void run();
};
}

#endif // GAME_H

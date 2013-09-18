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

#ifndef SQUAREZ_DEMO_H
#define SQUAREZ_DEMO_H

#include "game.h"

namespace squarez {

class Demo : public squarez::Game
{
private:
	std::chrono::duration<float> last_square;
	Transition transition;
	int status;
	
protected:
	virtual void selectionChanged(const Selection& selection);
	virtual void mouseDown(int x, int y, int button);
	virtual void timeTick(std::chrono::duration<float> t);
public:
	Demo(Window& w) : Game(w), last_square(std::chrono::seconds(-10)), status(0) {};
};
}

#endif // SQUAREZ_DEMO_H

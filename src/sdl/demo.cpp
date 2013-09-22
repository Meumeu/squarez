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

#include "demo.h"
/*
void squarez::Demo::mouseDown(int x, int y, int button)
{
}

void squarez::Demo::selectionChanged(const squarez::Selection& selection)
{
}

void squarez::Demo::timeTick(std::chrono::duration<float> t)
{
	float dt = (t - last_square).count();
	
	if (status == 0)
	{
		auto transitions = board.findTransitions();
	
		if (transitions.empty())
		{
			transition = Transition(board.size());
			setSelection(Selection());
		}
		else
		{
			transition = transitions[rand() % transitions.size()];
			setSelection(transition._selection);
			resetCellColours(-1, -1);
		}
		status = 1;
	}
	
	if (status == 1)
	{
		if (dt >= 1.2)
			status = 2;
	}
	
	if (status == 2)
	{
		applyTransition(transition);
		status = 3;
	}
	
	if (status == 3)
	{
		if (dt >= 1.5)
		{
			last_square = t;
			status = 0;
		}
	}
}*/

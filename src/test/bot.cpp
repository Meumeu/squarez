/*
 * Squarez puzzle game
 * Copyright (C) 2013  Patrick Nicolas <patricknicolas@laposte.net>
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

#include "shared/gameboard.h"
#include <iostream>

int main()
{
	squarez::GameBoard board(8, 3);
	int score = 0;
	int nbIter = 0;
	
	while (true)
	{
		squarez::Transition best_t;
		int nbCarres = 0;

		for (auto const& transition: board.findTransitions())
		{
			if (transition._score > best_t._score)
			best_t = transition;
			
			if (transition._score)
				nbCarres++;
		}

		board.applyTransition(best_t);

		if (best_t._score)
		{
			nbIter++;
			score += best_t._score;
			std::cout << "Score: " << score << std::endl;
			std::cout << "Number of carrés: " << nbCarres << std::endl;
			std::cout << "Number of itérations: " << nbIter << std::endl;
			std::cout << board << std::endl;
		}
		else
		{
			return 1;
		}
	}
}
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

#include "transition.h"

#include "gameboard.h"

#include <cstdlib>
#include <map>

namespace squarez
{

Transition::Transition(const GameBoard& board, const Selection& selection, uint32_t score): _score(score), _selection(selection)
{
	std::map<std::pair<int16_t, int16_t>, CellTransition> cellTransition;
	
	for (std::pair<int16_t, int16_t> point: selection.getPoints())
	{
		cellTransition.insert(std::make_pair(point, CellTransition(point.first, point.second)));
		while (point.second > 0)
		{
			--point.second;
			auto it = cellTransition.find(point);
			if  (it != cellTransition.end())
			{
				if (not it->second._removed)
				{
					it->second._toy++;
				}
			}
			else
			{
				cellTransition.insert(std::make_pair(point, CellTransition(point.first, point.second, point.first, point.second+1)));
			}
		}
		--point.second;
		
		uint8_t symbol = std::rand() % board.symbol();
		if (cellTransition.find(point) != cellTransition.end())
		{
			cellTransition.find(point)->second._toy++;
			--point.second;
			cellTransition.insert(std::make_pair(point, CellTransition(point.first, point.second, point.first, point.second+2, symbol)));
		}
		else
		{
			cellTransition.insert(std::make_pair(point, CellTransition(point.first, point.second, point.first, point.second+1, symbol)));
		}
	}
	
	for (auto const& cell : cellTransition)
	{
		_cells.push_back(cell.second);
	}
}

}

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
#include <algorithm>
#include <sstream>

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
		
		unsigned int symbol = std::rand() % board.symbol();
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

Transition::Transition(unsigned int size): _score(0)
{
	std::vector<std::pair<unsigned int,unsigned int>> positions(size*size);
	for (unsigned int y = 0; y < size ; ++y)
		for (unsigned int x = 0; x < size ; ++x)
			positions[x * size + y] = std::pair<unsigned int, unsigned int>(x,y);

	std::random_shuffle(positions.begin(), positions.end());
	
	for (unsigned int y = 0; y < size ; ++y)
		for (unsigned int x = 0; x < size ; ++x)
			_cells.push_back(CellTransition(x, y, positions[x * size + y].first, positions[x * size + y].second));
}

namespace
{
	void fill_from_stream(std::istream& serialized, uint32_t &_score, Selection &_selection, std::vector<Transition::CellTransition> &_cells)
{
	serialized >> _score;
	_selection = Selection(serialized);

	size_t numCells;
	serialized >> numCells;
	for (size_t x = 0; x < numCells; ++x)
	{
		_cells.push_back(Transition::CellTransition(serialized));
	}
}
}

Transition::Transition(std::istream& serialized)
{
	fill_from_stream(serialized, _score, _selection, _cells);
}

Transition::Transition(const std::string& serialized)
{
	std::stringstream str(serialized);
	fill_from_stream(str, _score, _selection, _cells);
}

void Transition::serialize(std::ostream& serialized) const
{
	serialized <<  _score << " ";
	_selection.serialize(serialized); serialized << " ";

	serialized <<  _cells.size() <<  " ";
	for (auto const& cell :  _cells)
	{
		cell.serialize(serialized);
	}
}

Transition::CellTransition::CellTransition(std::istream& serialized)
{
	serialized >> _fromx >> _fromy >> _tox >> _toy >> _symbol >> _removed;
}
void Transition::CellTransition::serialize(std::ostream& serialized) const
{
	serialized << _fromx << " " << _fromy << " " << _tox << " " << _toy << " " << _symbol << " " << _removed << " ";
}


}

/*
 * Squarez puzzle game
 * Copyright (C) 2015  Guillaume Meunier <guillaume.meunier@centraliens.net>
 * Copyright (C) 2013-2015  Patrick Nicolas <patricknicolas@laposte.net>
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
#include "utils/serializer.h"

#include <map>
#include <algorithm>
#include <sstream>

namespace squarez
{

Transition::Transition(const GameBoard& board, const Selection& selection, std::mt19937 & generator, uint32_t score): _score(score), _selection(selection)
{
	auto it1 = selection.begin();
	auto it2 = it1; it2++;
	auto end = selection.end();
	auto dist = [&board](std::mt19937 & generator){ return generator() % board._symbols; };

	while (it1 != end)
	{
		const auto x = it1->first;
		_cells.push_back(CellTransition(x, it1->second));
		if (it2 != end and x == it2->first)
		{
			_cells.push_back(CellTransition(x, it2->second));
			_cells.push_back(CellTransition(x, -2, x, 0, dist(generator)));
			_cells.push_back(CellTransition(x, -1, x, 1, dist(generator)));
			for (unsigned int y = 0; y < it1->second ; y++)
				_cells.push_back(CellTransition(x, y, x, y+2));
			for (unsigned int y = it1->second + 1 ; y < it2->second ; y ++)
				_cells.push_back(CellTransition(x, y, x, y+1));
			it1++; it2++;
		}
		else
		{
			_cells.push_back(CellTransition(x, -1, x, 0, dist(generator)));
			for (unsigned int y = 0; y < it1->second ; y++)
				_cells.push_back(CellTransition(x, y, x, y+1));
		}
		it1++; it2++;
	}
}

Transition::Transition(unsigned int size, std::mt19937 & generator): _score(0)
{
	std::vector<std::pair<unsigned int,unsigned int>> positions(size*size);
	for (unsigned int y = 0; y < size ; ++y)
		for (unsigned int x = 0; x < size ; ++x)
			positions[x * size + y] = std::pair<unsigned int, unsigned int>(x,y);

	// Reimplement std::shuffle as we want it to call the URNG exactly the same way on all platforms
	for (std::size_t i = 0; i < size * size - 1; ++i)
	{
		std::swap(positions[i], positions[i + generator() % (size * size - i)]);
	}
	
	for (unsigned int y = 0; y < size ; ++y)
		for (unsigned int x = 0; x < size ; ++x)
			_cells.push_back(CellTransition(x, y, positions[x * size + y].first, positions[x * size + y].second));
}

Transition::Transition(DeSerializer & serialized)
{
	serialized >> _score;
	_selection = Selection(serialized);
	serialized >> _cells;
}

Serializer& operator<<(Serializer& out, const Transition& transition)
{
	out << transition._score << transition._selection << transition._cells;
	return out;
}

Serializer & operator<<(Serializer & out, Transition::CellTransition const& cellTransition)
{
	out << cellTransition._fromx << cellTransition._fromy << cellTransition._tox << cellTransition._toy << cellTransition._symbol << cellTransition._removed;
	return out;
}
DeSerializer & operator>>(DeSerializer & in, Transition::CellTransition & cellTransition)
{
	in >> cellTransition._fromx >> cellTransition._fromy >> cellTransition._tox >> cellTransition._toy >> cellTransition._symbol >> cellTransition._removed;
	return in;
}

}

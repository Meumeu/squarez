/*
 * Squarez puzzle game
 * Copyright (C) 2013-2015  Guillaume Meunier <guillaume.meunier@centraliens.net>
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

#include "selection.h"
#include "gameboard.h"

#include "utils/serializer.h"

#include <sstream>
#include <stdexcept>

namespace squarez
{
bool Selection::togglePoint(std::shared_ptr<Cell> cell)
{
	if (_points.find(cell) != _points.end())
	{
		_points.erase(cell);
		return false;
	}
	
	_points.insert(cell);
	return true;
}

void Selection::setState(Selection::State state)
{
	_state = state;
}

Serializer& operator<<(Serializer& out, const Selection& selection)
{
	std::set<std::pair<unsigned int, unsigned int>> points;
	for (auto c : selection._points)
		points.insert(std::make_pair<unsigned int, unsigned int>(c->x(), c->y()));
	out << points;
	return out;
}

Selection::Selection(DeSerializer& serialized, const GameBoard & board)
{
	std::set<std::pair<unsigned int, unsigned int>> points;
	serialized >> points;
	for (auto p : points)
		_points.insert(board.getPtr(p.first, p.second));
}

bool VisibleSelection::togglePoint(std::shared_ptr<Cell> cell)
{
	bool res = squarez::Selection::togglePoint(cell);
	_proxy->changed();
	return res;
}

void VisibleSelection::setState(Selection::State state)
{
	if (state == this->state())
		return;
	squarez::Selection::setState(state);
	_proxy->stateChanged(state);
}

}

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

static unsigned int norm(const Cell & p0, const Cell & p1)
{
	return (p1.x() - p0.x())*(p1.x() - p0.x()) + (p1.y() - p0.y())*(p1.y() - p0.y());
}

// Check if the angle p0p1,p1p2 is a square angle
static bool isSquareAngle(const Cell & p0, const Cell & p1, const Cell & p2)
{
	return ((p1.x() - p0.x()) * (p1.x() - p2.x()) + (p1.y() - p0.y()) * (p1.y()- p2.y())) == 0;
}

unsigned int Selection::score() const
{
	// Check that we are actually selecting 4 points
	if (not isValid())
		return 0;

	auto it = begin();
	auto p0 = *(it++);
	auto p1 = *(it++);
	auto p2 = *(it++);
	auto p3 = *it;

	// Check that symbols are all the same
	if (p0->symbol != p1->symbol
		or p2->symbol != p3->symbol
		or p0->symbol != p2->symbol)
		return 0;

	// Now verify that it is a square: 4 edges with the same length and a square angle
	auto score = norm(*p0, *p1);
	if (norm(*p0,*p2) != score or norm(*p2,*p3) != score or norm(*p1,*p3) != score or not isSquareAngle(*p0,*p1,*p3) or score == 0)
		return 0;

	// Simple score calculation: surface of the square, with a x2 bonus if it is not parallel to the edge nor 45 degrees
	if (p0->x() != p1->x() and p0->y() != p1->y() and p0->x() != p3->x() and p0->y() != p3->y())
		return 2*score;

	return score;
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

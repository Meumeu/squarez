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

#include "selection.h"

#include "shared/serializer.h"

#include <sstream>
#include <stdexcept>

namespace squarez
{
bool Selection::addPoint(unsigned int x, unsigned int y)
{
	std::pair<unsigned int, unsigned int> point(x,y);
	if (_points.find(point) != _points.end())
	{
		_points.erase(point);
		return false;
	}
	
	if (_points.size() >= 4)
		return false;
	
	_points.insert(point);
	return true;
}

namespace {
std::pair<unsigned int, unsigned int> getnth(unsigned int i, std::set<std::pair<unsigned int, unsigned int>> const& points)
{
	if ( i > points.size())
		throw std::out_of_range("Invalid index in selection");

	auto it = points.begin();
	for (; i != 0; --i, ++it) {}
	return *it;
}
}

unsigned int Selection::getX(unsigned int i) const
{
	return getnth(i, _points).first;
}
unsigned int Selection::getY(unsigned int i) const
{
	return getnth(i, _points).second;
}

Serializer& operator<<(Serializer& out, const Selection& selection)
{
	out << selection._points;
	return out;
}

Selection::Selection(Serializer& serialized)
{
	serialized >> _points;
}

}

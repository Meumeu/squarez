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

#ifndef SELECTION_H
#define SELECTION_H

#include <set>
#include <utility>

namespace squarez
{

class DeSerializer;
class Serializer;

class Selection
{
	friend Serializer & operator<<(Serializer & out, Selection const& selection);
public:
	typedef std::set<std::pair<unsigned int, unsigned int>>::iterator iterator;
	typedef std::set<std::pair<unsigned int, unsigned int>>::const_iterator const_iterator;

	Selection() {}
	Selection(DeSerializer& serialized);

	bool togglePoint(unsigned int x, unsigned int y);

	bool isValid() const { return _points.size() == 4; }

	iterator begin() { return _points.begin(); }
	iterator end() { return _points.end(); }
	const_iterator cbegin() const { return _points.cbegin(); }
	const_iterator cend() const { return _points.cend(); }
	const_iterator begin() const { return _points.cbegin(); }
	const_iterator end() const { return _points.cend(); }
	
private:
	std::set<std::pair<unsigned int, unsigned int>> _points;
};

Serializer & operator<<(Serializer & out, Selection const& selection);

}

#endif // SELECTION_H

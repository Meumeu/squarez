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

#ifndef SQUAREZ_COLOUR_H
#define SQUAREZ_COLOUR_H

namespace squarez {

class Colour
{
public:
	float r, g, b;
	Colour(float r, float g, float b) : r(r), g(g), b(b) {}
	Colour() {}
};

Colour operator+(const Colour& rhs, const Colour& lhs);
Colour operator-(const Colour& rhs, const Colour& lhs);
Colour operator*(const Colour& rhs, float lhs);
Colour operator*(float rhs, const Colour& lhs);
Colour operator/(const Colour& rhs, float lhs);

}

#endif // SQUAREZ_COLOUR_H

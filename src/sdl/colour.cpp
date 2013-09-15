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

#include "colour.h"

squarez::Colour squarez::operator*(const squarez::Colour& rhs, float lhs)
{
	return Colour(rhs.r * lhs, rhs.g * lhs, rhs.b * lhs);
}

squarez::Colour squarez::operator*(float rhs, const squarez::Colour& lhs)
{
	return Colour(rhs * lhs.r, rhs * lhs.g, rhs * lhs.b);
}

squarez::Colour squarez::operator+(const squarez::Colour& rhs, const squarez::Colour& lhs)
{
	return Colour(rhs.r + lhs.r, rhs.g + lhs.g, rhs.b + rhs.b);
}

squarez::Colour squarez::operator-(const squarez::Colour& rhs, const squarez::Colour& lhs)
{
	return Colour(rhs.r - lhs.r, rhs.g - lhs.g, rhs.b - rhs.b);
}

squarez::Colour squarez::operator/(const squarez::Colour& rhs, float lhs)
{
	return Colour(rhs.r / lhs, rhs.g / lhs, rhs.b / lhs);
}


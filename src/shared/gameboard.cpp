/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2013  <copyright holder> <email>
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

#include "gameboard.h"
#include <stdexcept>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <boost/range/irange.hpp>

namespace squarez
{

void Selection::addPoint(uint8_t x, uint8_t y)
{
	if (_points.size() >= 4)
		return;
	
	_points.push_back(std::make_pair(x,y));
}


GameBoard::GameBoard(uint8_t size, uint8_t numberOfSymbols): _direction(down), _symbols(numberOfSymbols), _size(size)
{
	_cells.resize(size * size);
	std::srand(std::time(0));
	for (auto & cell : _cells)
		cell = std::rand() % numberOfSymbols;

}

uint8_t GameBoard::get(uint8_t x, uint8_t y) const
{
	if (x >= _size)
		throw std::out_of_range("x out of range");
	if (y >= _size)
		throw std::out_of_range("y out of range");
	return _cells[x * _size + y];
}


void GameBoard::set(uint8_t x, uint8_t y, uint8_t symbol)
{
	if (x >= _size)
		throw std::out_of_range("x out of range");
	if (y >= _size)
		throw std::out_of_range("y out of range");
	if (symbol >= _symbols)
		throw std::out_of_range("symbol out of range");
	_cells[x * _size + y] = symbol;
}

static uint16_t norm(std::pair<uint8_t, uint8_t> p0, std::pair<uint8_t, uint8_t> p1)
{
	return (p1.first - p0.first)*(p1.first - p0.first) + (p1.second - p0.second)*(p1.second - p0.second);
}

// Check if the angle p0p1,p1p2 is a square angle
static bool isSquareAngle(std::pair<uint8_t, uint8_t> p0, std::pair<uint8_t, uint8_t> p1, std::pair<uint8_t, uint8_t> p2)
{
	return ((p1.first - p0.first) * (p1.first - p2.first) + (p1.second - p0.second) * (p1.second- p2.second)) == 0;
}


Transition GameBoard::selectSquare(const Selection& selection) const
{
	auto points = selection.getPoints();
	// Check that we are actually selecting 4 points
	if (points.size() != 4)
		return Transition(*this);
	std::sort(points.begin(), points.end());
	auto p0 = points[0],
	p1 = points[1],
	p2 = points[2],
	p3 = points[3];
	
	// Check that symbols are all the same
	if (this->get(p0) != this->get(p1) or this->get(p2) != this->get(p3) or this->get(p0) != this->get(p2))
		return Transition(*this);
	
	// Now verify that it is a square: 4 edges with the same length and a square angle
	auto score = norm(p0,p1);
	if (norm(p0,p2) != score or norm(p2,p3) != score or norm(p1,p3) != score or not isSquareAngle(p0,p1,p3) or score == 0)
		return Transition(*this);

	// Simple score calculation: surface of the square, with a x2 bonus if it is not parallel to the edge	
	if (p0.first != p1.first or p0.second != p1.second)
		score *= 2;
	return Transition(*this, selection, score);
}

void GameBoard::applyTransition(const Transition& transition)
{
	int xinc = 0, yinc = 0;
	boost::strided_integer_range<uint8_t> xrange = boost::irange<uint8_t>(0, _size, 1);
	boost::strided_integer_range<uint8_t> yrange = xrange;
	switch (_direction)
	{
		case up:
			yinc = 1;
			yrange = boost::irange<uint8_t, int>(_size, 0, -1);
			break;
		case down:
			yinc = -1;
			break;
		case left:
			xinc = -1;
			break;
		case right:
			xinc = 1;
			xrange = boost::irange<uint8_t, int>(_size, 0, -1);
			break;
	}

	for (uint8_t x: xrange)
		for(uint8_t y: yrange)
		{
			auto cellTransition = transition(x,y);
			if (cellTransition._move)
				this->set(x + cellTransition._move * xinc, y + cellTransition._move * yinc, this->get(x, y));
		}

}

std::ostream& operator<<(std::ostream& out, GameBoard const& board)
{
	for (uint8_t x = 0 ; x< board.size() ; ++x)
	{
		for (uint8_t y = 0 ; y< board.size() ; ++y)
			out << (int)board.get(x,y)<< " ";
		out << std::endl;
	}
	return out;
}
}

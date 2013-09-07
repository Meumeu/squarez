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
#include <iostream>

namespace squarez
{

bool Selection::addPoint(uint8_t x, uint8_t y)
{
	std::pair<uint8_t, uint8_t> point(x,y);
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

bool Selection::getPoint(uint8_t x, uint8_t y) const
{
	return _points.find(std::pair<uint8_t, uint8_t>(x,y)) != _points.end();
}

GameBoard::GameBoard(uint8_t size, uint8_t numberOfSymbols): _symbols(numberOfSymbols), _size(size)
{
	_cells.resize(size * size);
	std::srand(std::time(0));
	for (auto & cell : _cells)
	{
		cell = std::rand() % numberOfSymbols;
	}

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
	auto it = points.begin();
	auto p0 = *(it++);
	auto p1 = *(it++);
	auto p2 = *(it++);
	auto p3 = *it;
	
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
#if 0
namespace
{
	struct Range
	{
		struct const_iterator
		{
			uint8_t _val;
			int _increment;
			uint8_t operator*() const { return _val;}
			const_iterator operator++() { _val += _increment; return *this;}
			bool operator!=(const_iterator const& other) const
			{ return _val != other._val;}
			const_iterator(uint8_t val, int increment): _val(val), _increment(increment) {}
		};
		Range(uint8_t from, uint8_t to):
			_begin(from, (to > from) ? 1 : -1),
			_end(to + ((to > from) ? 1 : -1), 1) {}
		const_iterator begin() const { return _begin; }
		const_iterator end() const { return _end;}
		const_iterator _begin;
		const_iterator _end;
	};
}
#endif

void GameBoard::applyTransition(const Transition& transition)
{
	if (transition._score == 0)
		return;

	for (uint8_t x = 0; x < _size; ++x)
		for(uint8_t y = _size; y > 0 ; --y)
		{
			auto cellTransition = transition(x, y - 1);
			if (cellTransition._move and not cellTransition._removed)
				this->set(x, y + cellTransition._move - 1, this->get(x, y - 1));
		}
		
	for (auto cell: transition.getNewCells())
	{
		this->set(cell._x, cell._y, cell._symbol);
	}

}

std::ostream& operator<<(std::ostream& out, GameBoard const& board)
{
	for (uint8_t y = 0 ; y< board.size() ; ++y)
	{
		for (uint8_t x = 0 ; x< board.size() ; ++x)
			out << (int)board.get(x,y)<< " ";
		out << std::endl;
	}
	return out;
}

void GameBoard::print() const {
	std::cout << *this << std::endl;
}

}

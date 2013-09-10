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

#include "gameboard.h"
#include "selection.h"
#include <stdexcept>
#include <cstdlib>
#include <ctime>
#include <iostream>

namespace squarez
{

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
		return Transition();
	
	auto it = points.begin();
	auto p0 = *(it++);
	auto p1 = *(it++);
	auto p2 = *(it++);
	auto p3 = *it;
	
	// Check that symbols are all the same
	if (this->get(p0) != this->get(p1) or this->get(p2) != this->get(p3) or this->get(p0) != this->get(p2))
		return Transition();
	
	// Now verify that it is a square: 4 edges with the same length and a square angle
	auto score = norm(p0,p1);
	if (norm(p0,p2) != score or norm(p2,p3) != score or norm(p1,p3) != score or not isSquareAngle(p0,p1,p3) or score == 0)
		return Transition();

	// Simple score calculation: surface of the square, with a x2 bonus if it is not parallel to the edge nor 45 degrees
	if (p0.first != p1.first and p0.second != p1.second and p0.first != p3.first and p0.second != p3.second)
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
	if (transition.getCellTransition().empty())
		return;
	
	auto oldCells = _cells;
	for (auto const& cellTransition: transition.getCellTransition())
	{
		if (not cellTransition._removed)
		{
			uint8_t symbol = (cellTransition.isNew()) ? cellTransition._symbol : oldCells.at(cellTransition._fromx * _size + cellTransition._fromy);
			this->set(cellTransition._tox, cellTransition._toy, symbol);
		}
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

void GameBoard::print() const
{
	std::cout << *this << std::endl;
}

std::vector<Transition> GameBoard::findTransitions() const
{
	std::vector<Transition> res;
	
	for(int x1 = 0; x1 < _size; x1++)
		for(int y1 = 0; y1 < _size; y1++)
			for(int x2 = x1; x2 < _size; x2++)
				for(int y2 = y1; y2 < _size; y2++)
				{
					int x3, y3, x4, y4;
					x3 = x2 + y1 - y2;
					y3 = y2 + x2 - x1;
					x4 = x1 + y1 - y2;
					y4 = y1 + x2 - x1;
					
					if (x3 < 0 or y3 < 0 or x4 < 0 or y4 < 0 or x3 >= _size or y3 >= _size or x4 >= _size or y4 >= _size)
						continue;
					
					squarez::Selection s;
					s.addPoint(x1, y1);
					s.addPoint(x2, y2);
					s.addPoint(x3, y3);
					s.addPoint(x4, y4);
					
					squarez::Transition const& t = this->selectSquare(s);
					
					if (t._score)
						res.push_back(t);
				}

	return res;
}
}

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



#include "gameboard.h"
#include "selection.h"
#include "rules/rules.h"
#include <stdexcept>
#include <ctime>
#include <iostream>
#include <random>

namespace
{
std::size_t idx(unsigned int x, unsigned int y, unsigned int size)
{
	if (x >= size)
		throw std::out_of_range("x out of range");
	
	if (y >= size)
		throw std::out_of_range("y out of range");
	
	return x * size + y;
}
}

namespace squarez
{

GameBoard::GameBoard(unsigned int size, unsigned int numberOfSymbols, std::mt19937 & generator, Rules & rules):
_symbols(numberOfSymbols), _size(size), _rules(rules)
{
	_cells.resize(_size * _size);
	for (unsigned int x = 0 ; x < _size ; ++x)
		for (unsigned int y = 0 ; y < _size ; ++y)
			_cells[idx(x, y, _size)].reset(new Cell(
				generator() % numberOfSymbols, x, y, _rules, _rules.cellProxyFactory()));
}

std::shared_ptr<Cell> GameBoard::getPtr(unsigned int x, unsigned int y) const
{
	return _cells[idx(x, y, _size)];
}

const Cell & GameBoard::get(unsigned int x, unsigned int y) const
{
	return *_cells[idx(x, y, _size)];
}

Cell & GameBoard::access(unsigned int x, unsigned int y)
{
	return *_cells[idx(x, y, _size)];
}

Transition GameBoard::selectSquare(const Selection& selection, std::mt19937 & generator, bool allowDefeat) const
{
	auto score = selection.score();
	if (score == 0)
		return Transition();

	if (allowDefeat)
		return Transition(*this, selection, generator, score);
	
	// We do not allow defeat, test if the transition leads to a defeat scenario

	GameBoard after(*this);
	Transition res(*this, selection, generator, score);
	after.applyTransition(res, false);
	while (not after.hasTransition())
	{
		res = Transition(_size, generator);
		after._cells = _cells;
		after.applyTransition(res, false);
	}
	res._score = score;
	res._selection = selection;
	return res;
}

void GameBoard::applyTransition(const Transition& transition, bool updateProxy)
{
	if (transition.getCellTransition().empty())
		return;

	auto cellProxyFactory = updateProxy ?
		_rules.cellProxyFactory() :
		[](Cell & owner) { return std::unique_ptr<Cell::Proxy>(new Cell::Proxy(owner));};

	auto oldCells = _cells;
	for (auto const& cellTransition: transition.getCellTransition())
	{
		if (not cellTransition._removed)
		{
			const unsigned int tox = cellTransition._tox;
			const unsigned int toy = cellTransition._toy;
			if (cellTransition.isNew())
			{
				_cells[idx(tox, toy, _size)].reset(new Cell(
					cellTransition._symbol, cellTransition._fromx, cellTransition._fromy, _rules, cellProxyFactory));
				_cells[idx(tox, toy, _size)]->move(tox, toy);
			}
			else
			{
				auto & cell = oldCells[idx(cellTransition._fromx, cellTransition._fromy, _size)];
				_cells[idx(tox, toy, _size)] = cell;
				cell->move(tox, toy);
			}
		}
	}
}

std::ostream& operator<<(std::ostream& out, GameBoard const& board)
{
	for (unsigned int y = 0 ; y< board._size ; ++y)
	{
		for (unsigned int x = 0 ; x< board._size ; ++x)
			out << board.get(x,y).symbol << " ";
		out << std::endl;
	}
	return out;
}

bool GameBoard::hasTransition() const
{
	for(unsigned int x1 = 0; x1 < _size - 1; x1++)
		for(unsigned int y1 = 0; y1 < _size; y1++)
			for(unsigned int x2 = x1 + 1; x2 < _size; x2++)
				for(unsigned int y2 = y1; y2 < _size; y2++)
				{
					unsigned int x3, y3, x4, y4;
					if (y2 > x2 + y1 or x1 > y2 + x2 or y2 > x1 + y1 or x1 > y1 + x2)
						continue;
					x3 = x2 + y1 - y2;
					y3 = y2 + x2 - x1;
					x4 = x1 + y1 - y2;
					y4 = y1 + x2 - x1;

					if (x3 >= _size or y3 >= _size or x4 >= _size or y4 >= _size)
						continue;

					squarez::Selection s;
					s.togglePoint(getPtr(x1, y1));
					s.togglePoint(getPtr(x2, y2));
					s.togglePoint(getPtr(x3, y3));
					s.togglePoint(getPtr(x4, y4));

					if (s.score() != 0)
						return true;
				}
	return false;
}

}

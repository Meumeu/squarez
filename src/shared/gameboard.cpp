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
#include "serializer.h"
#include <stdexcept>
#include <ctime>
#include <iostream>
#include <random>

#ifdef SQUAREZ_QT
#include "qt/cell.h"
#endif

namespace squarez
{

GameBoard::GameBoard(unsigned int size, unsigned int numberOfSymbols, std::mt19937 & generator): _symbols(numberOfSymbols), _size(size)
{
	_cells.resize(size * size);
	std::uniform_int_distribution<unsigned int> dist(0, numberOfSymbols -1);
	for (auto & cell : _cells)
	{
		cell = dist(generator);
	}
#ifdef SQUAREZ_QT
	for (unsigned int x = 0 ; x < _size ; ++x)
		for (unsigned int y = 0 ; y < _size ; ++y)
			_qtCells.append(new qt::Cell(x, y, this->get(x, y)));
#endif
}

GameBoard::GameBoard(const GameBoard & other):
#ifdef SQUAREZ_QT
	QAbstractListModel(),
#endif
	_symbols(other._symbols), _cells(other._cells), _size(other._size)
{
}

GameBoard::~GameBoard()
{
#ifdef SQUAREZ_QT
	while (not _qtCells.empty())
		delete _qtCells.takeLast();
#endif
}

unsigned int GameBoard::get(unsigned int x, unsigned int y) const
{
	if (x >= _size)
		throw std::out_of_range("x out of range");
	
	if (y >= _size)
		throw std::out_of_range("y out of range");
	
	return _cells[x * _size + y];
}


void GameBoard::set(unsigned int x, unsigned int y, unsigned int symbol)
{
	if (x >= _size)
		throw std::out_of_range("x out of range");
	
	if (y >= _size)
		throw std::out_of_range("y out of range");
	
	if (symbol >= _symbols)
		throw std::out_of_range("symbol out of range");
	
	_cells[x * _size + y] = symbol;
}

typedef std::pair<unsigned int, unsigned int> point_t;

static unsigned int norm(point_t p0, point_t p1)
{
	return (p1.first - p0.first)*(p1.first - p0.first) + (p1.second - p0.second)*(p1.second - p0.second);
}

// Check if the angle p0p1,p1p2 is a square angle
static bool isSquareAngle(point_t p0, point_t p1, point_t p2)
{
	return ((p1.first - p0.first) * (p1.first - p2.first) + (p1.second - p0.second) * (p1.second- p2.second)) == 0;
}

static unsigned int computeScore(GameBoard const& board, Selection const& selection)
{
	auto const& points = selection.getPoints();

	// Check that we are actually selecting 4 points
	if (points.size() != 4)
		return 0;

	auto it = points.begin();
	auto p0 = *(it++);
	auto p1 = *(it++);
	auto p2 = *(it++);
	auto p3 = *it;

	// Check that symbols are all the same
	if (board.get(p0) != board.get(p1) or board.get(p2) != board.get(p3) or board.get(p0) != board.get(p2))
		return 0;

	// Now verify that it is a square: 4 edges with the same length and a square angle
	auto score = norm(p0,p1);
	if (norm(p0,p2) != score or norm(p2,p3) != score or norm(p1,p3) != score or not isSquareAngle(p0,p1,p3) or score == 0)
		return 0;

	// Simple score calculation: surface of the square, with a x2 bonus if it is not parallel to the edge nor 45 degrees
	if (p0.first != p1.first and p0.second != p1.second and p0.first != p3.first and p0.second != p3.second)
		return 2*score;

	return score;
}

Transition GameBoard::selectSquare(const Selection& selection, std::mt19937 & generator, bool allowDefeat) const
{
	auto score = computeScore(*this, selection);
	if (score == 0)
		return Transition();

	if (allowDefeat)
		return Transition(*this, selection, generator, score);
	
	// We do not allow defeat, test if the transition leads to a defeat scenario

	GameBoard after(*this);
	Transition res(*this, selection, generator, score);
	after.applyTransition(res);
	while (not after.hasTransition())
	{
		res = Transition(_size, generator);
		after._cells = _cells; // The model will be out of sync but who cares
		after.applyTransition(res);
	}
	res._score = score;
	res._selection = selection;
	return res;
}

void GameBoard::applyTransition(const Transition& transition)
{
	if (transition.getCellTransition().empty())
		return;

	auto oldCells = _cells;
	for (auto const& cellTransition: transition.getCellTransition())
	{
		if (not cellTransition._removed)
		{
			unsigned int symbol = (cellTransition.isNew()) ? cellTransition._symbol : oldCells.at(cellTransition._fromx * _size + cellTransition._fromy);
			this->set(cellTransition._tox, cellTransition._toy, symbol);
		}
	}
#ifdef SQUAREZ_QT
	// qt cells may be empty if we are just testing future state
	if (not _qtCells.empty())
	{
		// We will need to get the qt cell by position, so build the mapping
		std::vector<unsigned int> qtCellIndexes(_cells.size(), 0);
		std::set<unsigned int, std::greater<unsigned int>> removed;
		for (int index = 0 ; index < _qtCells.size() ; ++index)
		{
			qt::Cell * qtCell = _qtCells[index];
			qtCellIndexes[qtCell->getX() * _size + qtCell->getY()] = index;
		}
		for (auto const& cellTransition: transition.getCellTransition())
		{
			int index = qtCellIndexes[cellTransition._fromx * _size + cellTransition._fromy];
			if (not cellTransition._removed)
			{
				if (cellTransition._fromx >= 0 and cellTransition._fromy >= 0)
				{
					qt::Cell * cell = _qtCells[index];
					cell->setX(cellTransition._tox);
					cell->setY(cellTransition._toy);
				}
			}
			else
			{
				removed.insert(index);
			}
		}
		for (unsigned int index : removed)
		{
			beginRemoveRows(QModelIndex(), index, index);
			delete _qtCells.takeAt(index);
			endRemoveRows();
		}
		for (auto const& cellTransition: transition.getCellTransition())
		{
			if (not cellTransition._removed and (cellTransition._fromx < 0 or cellTransition._fromy < 0))
			{
				int position = _qtCells.size();
				qt::Cell * newCell = new qt::Cell(cellTransition._fromx, cellTransition._fromy, cellTransition._symbol);
				beginInsertRows(QModelIndex(), position, position);
				_qtCells.append(newCell);
				endInsertRows();
				newCell->setX(cellTransition._tox);
				newCell->setY(cellTransition._toy);
			}
		}
	}

#endif
}

std::ostream& operator<<(std::ostream& out, GameBoard const& board)
{
	for (unsigned int y = 0 ; y< board.size() ; ++y)
	{
		for (unsigned int x = 0 ; x< board.size() ; ++x)
			out << (int)board.get(x,y)<< " ";
		out << std::endl;
	}
	return out;
}

std::vector<Transition> GameBoard::findTransitions() const
{
	std::vector<Transition> res;
	std::mt19937 generator;
	
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
					s.addPoint(x1, y1);
					s.addPoint(x2, y2);
					s.addPoint(x3, y3);
					s.addPoint(x4, y4);
					
					squarez::Transition const& t = this->selectSquare(s, generator, true);
					
					if (t._score)
						res.push_back(t);
				}

	return res;
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
					s.addPoint(x1, y1);
					s.addPoint(x2, y2);
					s.addPoint(x3, y3);
					s.addPoint(x4, y4);

					if (computeScore(*this, s) != 0)
						return true;
				}
	return false;
}

GameBoard::GameBoard(DeSerializer& serialized)
{
	serialized >> _symbols >> _size >> _cells;
#ifdef SQUAREZ_QT
	for (unsigned int x = 0 ; x < _size ; ++x)
		for (unsigned int y = 0 ; y < _size ; ++y)
			_qtCells.append(new qt::Cell(x, y, this->get(x, y)));
#endif
}

Serializer& operator<<(Serializer& out, const GameBoard& board)
{
	return out << board._symbols << board._size << board._cells;
}

#ifdef SQUAREZ_QT

QHash<int, QByteArray> GameBoard::roleNames() const
{
	QHash<int, QByteArray> roles;
	roles[0] = "cell";
	return roles;
}

int GameBoard::rowCount(const QModelIndex &) const
{
	return _qtCells.size();
}

QVariant GameBoard::data(const QModelIndex &index, int /*role*/) const
{
	return QVariant::fromValue(_qtCells.at(index.row()));
}

#endif

}

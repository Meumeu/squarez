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

#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <vector>
#include <set>
#include <iostream>
#include <random>

#include "transition.h"

#ifdef SQUAREZ_QT
#include <QList>
#include <QObject>
#include <QAbstractListModel>
#include "qt/cell.h"
#endif

namespace squarez
{

class DeSerializer;
class Selection;
class Serializer;

class GameBoard
#ifdef SQUAREZ_QT
	: public QAbstractListModel
{
	Q_OBJECT
	Q_PROPERTY(int size READ size CONSTANT)
	Q_PROPERTY(int symbol READ symbol CONSTANT)
private:
	QList<qt::Cell*> _qtCells;
public:
	int rowCount(const QModelIndex &) const;
	QVariant data(const QModelIndex &index, int role) const;
	QHash<int, QByteArray> roleNames() const;

#else
{
#endif
private:
	friend Serializer & operator<<(Serializer & out, GameBoard const& board);
	// Create an initialized gameboard (but with no qt data)
	GameBoard(const GameBoard & other);
	
public:
	// Create a new gameboard from its serialized representation
	GameBoard(DeSerializer & serialized);

	// Create a new random gameboard with given size and possible different symbols
	GameBoard(unsigned int size, unsigned int numberOfSymbols, std::mt19937 & generator);

	~GameBoard();
	
	// Attempt to remove the selected cells from the board, return the score.
	// Score is 0 only if selection is invalid
	// If allowDefeat is false, a selection that would lead to an impossible grid
	// will create a shuffle transition instead
	Transition selectSquare(Selection const& selection, std::mt19937 & generator, bool allowDefeat = true) const;

	std::vector<Transition> findTransitions() const;
	bool hasTransition() const;
	
	void applyTransition(Transition const& transition);
	
	// Get the symbol of the board at coordinates x,y
	unsigned int get(unsigned int x, unsigned int y) const;
	unsigned int get(std::pair<unsigned int, unsigned int> point) const {return get(point.first, point.second);}
	
	// Set the symbol of the board at coordinates x,y
	void set(unsigned int x, unsigned int y, unsigned int symbol);
	void set(std::pair<unsigned int, unsigned int> point, unsigned int symbol) {set(point.first, point.second, symbol);}
	
	unsigned int size() const { return _size;}
	unsigned int symbol() const { return _symbols; }
	
private:
	// Number of possible values for each cell
	unsigned int _symbols;
	
	// The board itself
	std::vector<unsigned int> _cells;
	unsigned int _size;

};

Serializer & operator<<(Serializer & out, GameBoard const& board);
std::ostream & operator<<(std::ostream & out, GameBoard const& board);
}

#endif // GAMEBOARD_H

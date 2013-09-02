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

#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <stdint.h>
#include <vector>
#include <set>
#include <ostream>

#include "transition.h"

namespace squarez
{
	
class Selection
{
public:
	void addPoint(uint8_t x, uint8_t y);
	std::set<std::pair<uint8_t, uint8_t>> const& getPoints() const {return _points;}
private:
	std::set<std::pair<uint8_t, uint8_t>> _points;
};

class GameBoard
{
	
public:
	GameBoard(uint8_t size, uint8_t numberOfSymbols);
	
	// Attempt to remove the selected cells from the board, return the score.
	// Score is 0 only if selection is invalid
	Transition selectSquare(Selection const& selection) const;
	
	void applyTransition(Transition const& transition);
	
	// Get the symbol of the board at coordinates x,y
	uint8_t get(uint8_t x, uint8_t y) const;
	uint8_t get(std::pair<uint8_t, uint8_t> point) const {return get(point.first, point.second);}
	
	// Set the symbol of the board at coordinates x,y
	void set(uint8_t x, uint8_t y, uint8_t symbol);
	void set(std::pair<uint8_t, uint8_t> point, uint8_t symbol) {set(point.first, point.second, symbol);}
	
	uint8_t size() const { return _size;}
	uint8_t symbol() const { return _symbols; }
private:
	// Number of possible values for each cell
	uint8_t _symbols;
	
	// The board itself
	std::vector<uint8_t> _cells;
	const uint8_t _size;

};

std::ostream & operator<<(std::ostream & out, GameBoard const& board);
}

#endif // GAMEBOARD_H

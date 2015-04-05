/*
 * Squarez puzzle game
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

#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <vector>
#include <set>
#include <iostream>
#include <random>

#include "cell.h"
#include "transition.h"

namespace squarez
{

class Rules;
class Selection;

class GameBoard
{
public:
	// Create a new random gameboard with given size and possible different symbols
	GameBoard(unsigned int size, unsigned int numberOfSymbols, std::mt19937 & generator, Rules & rules);
	
	// Attempt to remove the selected cells from the board, return the score.
	// Score is 0 only if selection is invalid
	// If allowDefeat is false, a selection that would lead to an impossible grid
	// will create a shuffle transition instead
	Transition selectSquare(Selection const& selection, std::mt19937 & generator, bool allowDefeat = true) const;

	bool hasTransition() const;
	
	void applyTransition(Transition const& transition, bool updateProxy = true);
	
	// Get the Cell at coordinates x,y
	const Cell & get(unsigned int x, unsigned int y) const;
	std::shared_ptr<Cell> getPtr(unsigned int x, unsigned int y) const;

	Cell & access(unsigned int x, unsigned int y);
	
	// Number of possible values for each cell
	const unsigned int _symbols;
	// Number of rows or columns
	const unsigned int _size;
private:
	
	// The board itself
	std::vector<std::shared_ptr<Cell>> _cells;

	Rules & _rules;
};
std::ostream & operator<<(std::ostream & out, GameBoard const& board);
}

#endif // GAMEBOARD_H

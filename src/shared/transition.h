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

#ifndef TRANSITION_H
#define TRANSITION_H

#include "selection.h"

#include <vector>
#include <string>
#include <iostream>

namespace squarez
{
	
class GameBoard;

class Transition
{
public:
	struct CellTransition
	{
		CellTransition(int16_t fromx, int16_t fromy, unsigned int tox, unsigned int toy, unsigned int symbol = 0xFF):
			_fromx(fromx), _fromy(fromy), _tox(tox), _toy(toy), _symbol(symbol), _removed(false) {}
		CellTransition(int16_t deletex, int16_t deletey):
			_fromx(deletex), _fromy(deletey), _tox(deletex), _toy(deletey), _symbol(0xFF), _removed(true) {}
		CellTransition(std::istream & serialized);
		
		int16_t _fromx;
		int16_t _fromy;
		
		unsigned int _tox;
		unsigned int _toy;
		
		unsigned int _symbol;
		bool _removed;
		
		bool isNew() const { return _symbol != 0xFF;}
		void serialize(std::ostream & serialized) const;
	};
	
	//Create an empty transition
	Transition() : _score(0) {}
	
	//Create a "move" transition, by removing the selection and pushing down the elements
	Transition(GameBoard const& board, Selection const& selection, uint32_t score);
	
	//Creates a "shuffle" transition, by randomly moving elements
	Transition(unsigned int size);

	Transition(std::istream & serialized);
	Transition(std::string const& serialized);

	std::vector<CellTransition> const& getCellTransition() const { return _cells; }
	std::size_t size() const { return _cells.size(); }
	CellTransition const& get(std::size_t pos) const { return _cells.at(pos); }

	void serialize(std::ostream & serialized) const;
	
	uint32_t _score;
	Selection _selection;
private:
	std::vector<CellTransition> _cells;
};

}
#endif

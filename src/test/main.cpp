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

#include <iostream>
#include "shared/gameboard.h"

int main() {
	squarez::GameBoard b(8,3);
	
	b.set(0,4,0);
	b.set(0,6,0);
	b.set(2,4,0);
	b.set(2,6,0);
	b.set(3,4,1);
	
	std::cout << std::endl << b<< std::endl;
	
	squarez::Selection s;
	s.addPoint(0,4);
	s.addPoint(3,4);
	s.addPoint(0,6);
	s.addPoint(2,6);
	s.addPoint(3,4);
	s.addPoint(2,4);
	squarez::Transition t = b.selectSquare(s);
	
	b.applyTransition(t);
	std::cout << std::endl << b<< std::endl;
	
	return 0;
}

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
#include <stdexcept>
#include <sstream>
#include "shared/gameboard.h"

#define SIZE 8
#define SYMBOLS 3

void testScore()
{
	std::cout << "Score calculation testing... ";
	squarez::GameBoard board(SIZE, 1);
	
	for(int x1 = 0; x1 < SIZE; x1++)
		for(int y1 = 0; y1 < SIZE; y1++)
			for(int x2 = x1; x2 < SIZE; x2++)
				for(int y2 = y1; y2 < SIZE; y2++)
				{
					int x3, y3, x4, y4;
					x3 = x2 + y1 - y2;
					y3 = y2 + x2 - x1;
					x4 = x1 + y1 - y2;
					y4 = y1 + x2 - x1;
					
					if (x3 < 0 or y3 < 0 or x4 < 0 or y4 < 0 or x3 >= SIZE or y3 >= SIZE or x4 >= SIZE or y4 >= SIZE)
						continue;
					
					squarez::Selection s;
					s.addPoint(x1, y1);
					s.addPoint(x2, y2);
					s.addPoint(x3, y3);
					s.addPoint(x4, y4);
					
					// Expected score: area when 2 points are aligned, area*2 in other cases
					uint16_t score = (x2 - x1) * (x2 -x1) + (y2 - y1) * (y2 - y1);
					if (x1 != x2 and x1 != x3 and x1 != x4 and y1 != y2 and y1 != y3 and y1 != y4)
						score *= 2;
					
					auto t = board.selectSquare(s, true);
					
					if (t._score != score)
					{
						std::stringstream text;
						text << "Score calculation mismatch, expected " << score << " got " << t._score;
						throw std::runtime_error(text.str());
					}
				}
				
	std::cout << "passed" << std::endl;
}

void testSelection()
{
	std::cout << "Selection validation testing... ";
	squarez::GameBoard board(SIZE, SYMBOLS);
	
	// Not matching symbols
	squarez::Selection s;
	s.addPoint(0,0); board.set(0,0,0);
	s.addPoint(0,1); board.set(0,1,1);
	s.addPoint(1,0);
	s.addPoint(1,1);
	
	auto t = board.selectSquare(s, true);
	if (t._score)
	{
		std::stringstream text;
		text << "Square with different symbols validated with score " << t._score;
		throw std::runtime_error(text.str());
	}
	
	// Non square selection
	s = squarez::Selection();
	s.addPoint(0,0); board.set(0,0,0);
	s.addPoint(0,1); board.set(0,1,0);
	s.addPoint(1,0); board.set(1,0,0);
	s.addPoint(2,1); board.set(2,1,0);
	
	t = board.selectSquare(s, true);
	if (t._score)
	{
		std::stringstream text;
		text << "Non square selection validated with score " << t._score;
		throw std::runtime_error(text.str());
	}
	
	std::cout << "passed" << std::endl;
}

void testSquareFinding()
{
	std::cout << "Square finding testing... ";
	squarez::GameBoard board(3, 2);
	
	//No square can be found
	uint8_t situation0[] =
	{0, 0, 0,
	 0, 1, 0,
	 1, 1, 0};
	for (uint8_t x = 0; x < 3; x++)
		for (uint8_t y = 0; y < 3; y++)
			board.set(x,y,situation0[3*x + y]);

	if (board.hasTransition())
	{
		std::stringstream text;
		text << "Impossible transition found";
		throw std::runtime_error(text.str());
	}
	
	// Only one square
	uint8_t situation1[] =
	{0, 0, 0,
	 0, 1, 0,
	 0, 1, 0};
	for (uint8_t x = 0; x < 3; x++)
		for (uint8_t y = 0; y < 3; y++)
			board.set(x,y,situation1[3*x + y]);
	auto const& transitions = board.findTransitions();
	if (transitions.size() != 1)
	{
		std::stringstream text;
		text << "Wrong number of transition found: expected 1 got " << transitions.size() ;
		for (auto const& t : transitions)
		{
			text << std::endl << "{";
			for (auto const& p: t._selection.getPoints())
				text << "(" << (uint16_t)p.first << ", " << (uint16_t)p.second << ") ";
			text << "}";
		}
		throw std::runtime_error(text.str());
	}
	
	// This transition leads to a situation with no square, test no-shuffle/shuffle cases
	board = squarez::GameBoard(4,3);
	uint8_t situation2[] =
	{0, 0, 0, 1, // X 0 X 1
	 0, 1, 1, 0, // X 1 X 0
	 0, 2, 0, 0, // 0 2 1 0
	 0, 2, 1, 0};// 0 2 1 0
	 for (uint8_t x = 0; x < 4; x++)
		 for (uint8_t y = 0; y < 4; y++)
			 board.set(x,y,situation2[4*x + y]);
	squarez::Selection s;
	s.addPoint(0,0);
	s.addPoint(0,2);
	s.addPoint(2,0);
	s.addPoint(2,2);
	
	// allowDefeat = true
	auto const& t1 = board.selectSquare(s, true);
	squarez::GameBoard lost = board;
	lost.applyTransition(t1);
	if (lost.hasTransition())
	{
		std::stringstream text;
		text << "Board should not have a transition" << std::endl << lost;
		throw std::runtime_error(text.str());
	}
	
	// allowDefeat = false
	auto const& t2 = board.selectSquare(s, false);
	squarez::GameBoard ok = board;
	ok.applyTransition(t2);
	if (not ok.hasTransition())
	{
		std::stringstream text;
		text << "Board should have a transition";
		throw std::runtime_error(text.str());
	}
	std::cout << "passed" << std::endl;
}

int main() {
	try
	{
		testSelection();
		testScore();
		testSquareFinding();
	}
	catch (std::exception &e)
	{
		std::cout << std::endl << "test case failed:" << std::endl << e.what() << std::endl;
		return 1;
	}
	return 0;
}

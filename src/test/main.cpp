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
#include <future>
#include <cmath>
#include <random>

#include "shared/board/gameboard.h"
#include "client/httprequest.h"
#include "shared/network/urltools.h"
#include "shared/serializer.h"

#define SIZE 8
#define SYMBOLS 3

void testScore(std::mt19937 & generator)
{
	std::cout << "Score calculation testing... ";
	squarez::GameBoard board(SIZE, 1, generator);
	
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
					
					auto t = board.selectSquare(s, generator, true);
					
					if (t._score != score)
					{
						std::stringstream text;
						text << "Score calculation mismatch, expected " << score << " got " << t._score;
						throw std::runtime_error(text.str());
					}
				}
				
	std::cout << "passed" << std::endl;
}

void testSelection(std::mt19937 & generator)
{
	std::cout << "Selection validation testing... ";
	squarez::GameBoard board(SIZE, SYMBOLS, generator);
	
	// Not matching symbols
	squarez::Selection s;
	s.addPoint(0,0); board.set(0,0,0);
	s.addPoint(0,1); board.set(0,1,1);
	s.addPoint(1,0);
	s.addPoint(1,1);
	
	auto t = board.selectSquare(s, generator, true);
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
	
	t = board.selectSquare(s, generator, true);
	if (t._score)
	{
		std::stringstream text;
		text << "Non square selection validated with score " << t._score;
		throw std::runtime_error(text.str());
	}
	
	std::cout << "passed" << std::endl;
}

void testSquareFinding(std::mt19937 & generator)
{
	std::cout << "Square finding testing... ";
	squarez::GameBoard board(3, 2, generator);
	
	//No square can be found
	unsigned int situation0[] =
	{0, 0, 0,
	 0, 1, 0,
	 1, 1, 0};
	for (unsigned int x = 0; x < 3; x++)
		for (unsigned int y = 0; y < 3; y++)
			board.set(x,y,situation0[3*x + y]);

	if (board.hasTransition())
	{
		std::stringstream text;
		text << "Impossible transition found";
		throw std::runtime_error(text.str());
	}
	
	// Only one square
	unsigned int situation1[] =
	{0, 0, 0,
	 0, 1, 0,
	 0, 1, 0};
	for (unsigned int x = 0; x < 3; x++)
		for (unsigned int y = 0; y < 3; y++)
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
	board = squarez::GameBoard(4,3, generator);
	auto init_lost = [](squarez::GameBoard & board)
	{
		unsigned int situation2[] =
		{0, 0, 0, 1, // X 0 X 1
			0, 1, 1, 0, // X 1 X 0
			0, 2, 0, 0, // 0 2 1 0
			0, 2, 1, 0};// 0 2 1 0
		for (unsigned int x = 0; x < 4; x++)
			for (unsigned int y = 0; y < 4; y++)
				board.set(x,y,situation2[4*x + y]);
	};
	squarez::Selection s;
	s.addPoint(0,0);
	s.addPoint(0,2);
	s.addPoint(2,0);
	s.addPoint(2,2);
	
	// allowDefeat = true
	init_lost(board);
	board.applyTransition(board.selectSquare(s, generator, true));
	if (board.hasTransition())
	{
		std::stringstream text;
		text << "Board should not have a transition" << std::endl << board;
		throw std::runtime_error(text.str());
	}
	
	// allowDefeat = false
	init_lost(board);
	board.applyTransition(board.selectSquare(s, generator, false));
	if (not board.hasTransition())
	{
		std::stringstream text;
		text << "Board should have a transition";
		throw std::runtime_error(text.str());
	}
	std::cout << "passed" << std::endl;
}

void testSerialization1()
{
	std::cout << "Serialization validation 1 testing... ";
	
	squarez::StringSerializer s;
	
	s << 42;
	s << "toto";
	s << std::string("foobar");
	s << "";
	s << std::pair<int, float>(4156, 4168.48);
// 	std::cout << "<" << s.get() << ">" << std::endl;
	squarez::StringDeSerializer s2(s.get());
	
	int x;
	std::string y;
	std::pair<int, float> z;
	
	s2 >> x;
	if (x != 42)
		throw std::runtime_error("Bad deserialization 1");

	s2 >> y;
	if (y != "toto")
		throw std::runtime_error("Bad deserialization 2");
	
	s2 >> y;
	if (y != "foobar")
		throw std::runtime_error("Bad deserialization 3");
	
	s2 >> y;
	if (y != "")
		throw std::runtime_error("Bad deserialization 4");
	
	s2 >> z;
	if (z.first != 4156 || std::fabs(z.second - 4168.48) > 0.0001)
		throw std::runtime_error("Bad deserialization 5");
	
	std::cout << "passed" << std::endl;
}

void testSerialization2(std::mt19937 & generator)
{
	std::cout << "Serialization validation 2 testing... ";
	
	squarez::GameBoard board(SIZE, SYMBOLS, generator);

	squarez::StringSerializer stream;
	stream << board;

	squarez::StringDeSerializer stream2(stream.get());
	
	squarez::GameBoard board1(stream2);
	bool sameCells = true;
	for (unsigned int x = 0 ; x < board.size() ; ++x)
		for (unsigned int y = 0 ; y < board.size() ; ++y)
			sameCells = sameCells and board.get(x,y) == board1.get(x,y);

	if (not (board.size() == board1.size())
		or not (board.symbol() == board1.symbol())
		or not sameCells)
	{
		std::stringstream text;
		text << "Board changed during serialization:" << board << std::endl << board1;
		throw std::runtime_error(text.str());
	}
	
	std::cout << "passed" << std::endl;
}

void testHttpRequest()
{
	std::cout << "XMLHttpRequest testing... ";
	
	std::promise<bool> p;
	std::future<bool> f = p.get_future();
	
	squarez::HttpRequest xhr;
	
	xhr.request("http://www.google.com/",
		[&p](std::string const&) { p.set_value(true); },
		[&p]() { p.set_value(false); });
	
	if (f.wait_for(std::chrono::seconds(10)) != std::future_status::ready)
	{
		throw std::runtime_error("XMLHttpRequest timeout");
	}
	
	if (!f.get())
	{
		throw std::runtime_error("XMLHttpRequest error");
	}
	
	p = std::promise<bool>();
	f = p.get_future();
	
	xhr.request("http://error.meumeu.org/",
		[&p](std::string const&) {  p.set_value(true); },
		[&p]() {  p.set_value(false); });
	
	if (f.wait_for(std::chrono::seconds(10)) != std::future_status::ready)
	{
		throw std::runtime_error("XMLHttpRequest timeout");
	}
	
	if (f.get())
	{
		throw std::runtime_error("XMLHttpRequest missing error");
	}
	std::cout << "passed" << std::endl;
}

void testUrlEncode()
{
	std::cout << "urlencode testing... ";
	
	if (squarez::urlTools::urlencode("toto") != "toto")
		throw std::runtime_error("urlencode error");
	
	if (squarez::urlTools::urlencode("to/to") != "to%2fto")
		throw std::runtime_error("urlencode error");
	
	if (squarez::urlTools::urlencode("to\nto") != "to%0ato")
		throw std::runtime_error("urlencode error");
	
	if (squarez::urlTools::urlencode("&é\"'(-è_çà)=+*!:;,") != "%26%c3%a9%22'(-%c3%a8_%c3%a7%c3%a0)%3d%2b*!%3a%3b%2c")
		throw std::runtime_error("urlencode error");
	
	std::cout << "passed" << std::endl;
}

int main() {
	std::mt19937 generator = std::mt19937(std::random_device()());
	try
	{
		testSelection(generator);
		testScore(generator);
		testSquareFinding(generator);
		testSerialization1();
		testSerialization2(generator);
		testHttpRequest();
		testUrlEncode();
	}
	catch (std::exception &e)
	{
		std::cout << std::endl << "test case failed:" << std::endl << e.what() << std::endl;
		return 1;
	}
	return 0;
}

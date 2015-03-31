/*
 * Squarez puzzle game
 * Copyright (C) 2013-2015  Guillaume Meunier <guillaume.meunier@centraliens.net>
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

#define private public
#define protected public

#include "game/constants.h"
#include "rules/singleplayerrules.h"
#include <iostream>

namespace
{
	class DummyProxy : public squarez::SinglePlayerRules::Proxy
	{
		virtual void scoreChanged(unsigned int) override {};
		virtual void gameOverChanged(bool) override {};
		virtual void timerUpdated() override {};
		virtual void nameRequired() override {};
		virtual void animateSquare(std::array<squarez::Cell *, 4>) override {};
		virtual void networkError() override {}

		virtual std::unique_ptr<squarez::Cell::Proxy> cellProxyFactory(squarez::Cell & owner) override {return std::unique_ptr<squarez::Cell::Proxy>(new squarez::Cell::Proxy(owner));}
	};


	std::vector<squarez::Transition> findTransitions(const squarez::GameBoard & board)
	{
		std::vector<squarez::Transition> res;
		std::mt19937 generator;

		for(unsigned int x1 = 0; x1 < board._size - 1; x1++)
			for(unsigned int y1 = 0; y1 < board._size; y1++)
				for(unsigned int x2 = x1 + 1; x2 < board._size; x2++)
					for(unsigned int y2 = y1; y2 < board._size; y2++)
					{
						unsigned int x3, y3, x4, y4;
						if (y2 > x2 + y1 or x1 > y2 + x2 or y2 > x1 + y1 or x1 > y1 + x2)
							continue;
						x3 = x2 + y1 - y2;
						y3 = y2 + x2 - x1;
						x4 = x1 + y1 - y2;
						y4 = y1 + x2 - x1;

						if (x3 >= board._size or y3 >= board._size or x4 >= board._size or y4 >= board._size)
							continue;

						squarez::Selection s;
						s.togglePoint(x1, y1);
						s.togglePoint(x2, y2);
						s.togglePoint(x3, y3);
						s.togglePoint(x4, y4);

						squarez::Transition const& t = board.selectSquare(s, generator, true);

						if (t._score)
							res.push_back(t);
					}

					return res;
	}
}

int main()
{
	DummyProxy proxy;
	squarez::SinglePlayerRules rules(proxy, squarez::constants::default_timer());
	int score = 0;
	int nbIter = 0;
	auto & board = *rules._board;

	std::mt19937 generator;
	
	while (true)
	{
		squarez::Transition best_t;
		int nbCarres = 0;

		for (auto const& transition: findTransitions(board))
		{
			if (transition._score > best_t._score)
			best_t = transition;
			
			if (transition._score)
				nbCarres++;
		}

		best_t = board.selectSquare(best_t._selection, generator, true);

		board.applyTransition(best_t);

		if (best_t._score)
		{
			nbIter++;
			score += best_t._score;
			std::cout << "Score: " << score << std::endl;
			std::cout << "Number of carrés: " << nbCarres << std::endl;
			std::cout << "Number of itérations: " << nbIter << std::endl;
			std::cout << board << std::endl;
		}
		else
		{
			return 1;
		}
	}
}

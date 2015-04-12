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

#include "tutorialrules.h"

#include "game/constants.h"

squarez::TutorialRules::TutorialRules(squarez::Rules::Proxy & proxy, int board_size, int nb_symbols):
	squarez::Rules(proxy, board_size, nb_symbols, 0, // Hardcoded random seed
		Timer(std::chrono::hours(24), std::chrono::hours(24), std::chrono::hours(24)), "Tutorial"),
_step(1)
{
	_proxy.message("Squarez rules\n(click to continue)");
}

namespace
{
bool isGood(squarez::Selection const & sel)
{
	auto it = sel.begin();
	auto p0 = *(it++);
	auto p1 = *(it++);
	auto p2 = *(it++);
	auto p3 = *it;
	return p0->x() != p1->x() and p0->y() != p1->y() and p0->x() != p3->x() and p0->y() != p3->y();
}

void selectCells(std::unique_ptr<squarez::VisibleSelection>& selection)
{
	for(auto& i: *selection)
	{
		i->setSelected(true);
	}
	selection->setState(squarez::Selection::State::validated);
}
}

static squarez::Selection findSquare(squarez::GameBoard const & board, std::function<bool(squarez::Selection const &)> predicate)
{
	// Selection that doesn't match the request, but that we return if we don't find a good one
	squarez::Selection lastChance;
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
					s.togglePoint(board.getPtr(x1, y1));
					s.togglePoint(board.getPtr(x2, y2));
					s.togglePoint(board.getPtr(x3, y3));
					s.togglePoint(board.getPtr(x4, y4));
					if (s.score() != 0)
					{
						if (predicate(s))
							return s;
						lastChance = s;
					}
				}

	return lastChance;
}

void squarez::TutorialRules::onClick (squarez::Cell& /*cell*/)
{
	++_step;
	switch (_step)
	{
		case 1:
			break;
		case 2:
			_proxy.message("Spot similar elements at the edges of a square shape");
			break;
		case 3:
			_selection.reset(new VisibleSelection(selectionProxyFactory(), findSquare(*_board, std::unary_negate<std::function<bool(squarez::Selection const &)>>(isGood))));
			selectCells(_selection);
			break;
		case 4:
		{
			_proxy.message("Bigger squares give more points");
			squarez::Transition const & transition = _board->selectSquare(*_selection, _random_generator, false);
			this->setScore(score() + transition._score);
			this->applyTransition(transition);
			_selection.reset();
			break;
		}
		case 5:
			_proxy.message("Squares not aligned with the grid give double score");
			break;
		case 6:
			_selection.reset(new VisibleSelection(selectionProxyFactory(), findSquare(*_board, isGood)));
			selectCells(_selection);
			_proxy.message("Squares not aligned with the grid give double score");
			break;
		case 7:
		{
			squarez::Transition const & transition = _board->selectSquare(*_selection, _random_generator, false);
			this->setScore(score() + transition._score);
			this->applyTransition(transition);
			_selection.reset();

			_proxy.message("Time is limited\nEach square you select refills time based on its score");
			_timer = constants::default_timer();
			_proxy.timerUpdated();
			break;
		}
		case 8:
			_proxy.message("Total amount of available time gets shorter as you progress");
			_selection.reset(new VisibleSelection(selectionProxyFactory(), findSquare(*_board, isGood)));
		default:
			if (_selection.get() && _selection->isValid())
			{
				_proxy.message("");
				auto const & transition = _board->selectSquare(*_selection, _random_generator, false);
				this->setScore(score() + transition._score);
				this->refillTimer(transition._score * 2);
				this->applyTransition(transition);
				_selection.reset();
			}
			else
			{
				_proxy.message("");
				_selection.reset(new VisibleSelection(selectionProxyFactory(), findSquare(*_board, isGood)));
				selectCells(_selection);
			}
	}
}

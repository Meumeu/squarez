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

#include "tutorialrules.h"

#ifdef SQUAREZ_QT
#define MESSAGE(text) emit message(text)
#else
#include "ui.h"
#define MESSAGE(text) _ui->onMessage(text)
#endif

squarez::TutorialRules::TutorialRules(int board_size, int nb_symbols):
squarez::Rules(board_size, nb_symbols, "Tutorial"),
_timer(std::chrono::seconds(10), std::chrono::seconds(60), std::chrono::seconds(180)),
_step(0)
{
	_timer.setPause(true);
}

static squarez::Transition findHorizontalTransition(squarez::GameBoard const & board)
{
	auto const & transitions = board.findTransitions();
	for (auto const& transition : transitions)
	{
		if (transition._selection.getX(0) == transition._selection.getX(1))
			return transition;
	}
	// There is no horizontal transition, give anything else
	return transitions.at(0);
}

static squarez::Transition findGoodTransition(squarez::GameBoard const & board)
{
	auto const & transitions = board.findTransitions();
	for (auto const& transition : transitions)
	{
		if (transition._selection.getX(0) != transition._selection.getX(1)
				and transition._selection.getX(0) != transition._selection.getX(2))
			return transition;
	}
	return transitions.at(0);
}

void squarez::TutorialRules::next()
{
	++_step;
	switch (_step)
	{
		case 1:
			MESSAGE("Squarez rules\n(click to continue)");
			break;
		case 3:
			_selection = findHorizontalTransition(*_board)._selection;
			this->acceptSelection(_selection);
		case 2:
			MESSAGE("Spot similar elements at the edges of a square shape");
			break;
		case 4:
		case 8:
		{
			MESSAGE("");
			squarez::Transition const & transition = _board->selectSquare(_selection, false);
			this->setScore(getScore() + transition._score);
			this->applyTransition(transition);
			break;
		}
		case 5:
			MESSAGE("Bigger squares give more points");
			break;
		case 7:
			_selection = findGoodTransition(*_board)._selection;
			this->acceptSelection(_selection);
		case 6:
			MESSAGE("Squares not aligned with the grid give double score");
			break;
		case 9:
			MESSAGE("Time is limited\nEach square you select refills time based on its score");
			_timer.setPause(false);
			break;
		case 10:
			MESSAGE("Total amount of available time gets shorter as you progress");
			_selection = squarez::Selection();
		default:
			if (_selection.getPoints().empty())
			{
				auto const & transitions = _board->findTransitions();
				_selection = transitions.at(std::rand() % transitions.size())._selection;
				this->acceptSelection(_selection);
			}
			else
			{
				MESSAGE("");
				auto const & transition = _board->selectSquare(_selection, false);
				this->setScore(getScore() + transition._score);
				_timer.refill(transition._score * 2);
				this->applyTransition(transition);;
				_selection = squarez::Selection();
			}
	}
}

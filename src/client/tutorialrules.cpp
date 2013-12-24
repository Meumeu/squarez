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
#include "ui.h"

squarez::TutorialRules::TutorialRules(int board_size, int nb_symbols):
squarez::Rules(board_size, nb_symbols, "Tutorial"),
_timer(std::chrono::seconds(10), std::chrono::seconds(60), std::chrono::seconds(180)),
_score(0), _step(0)
{
	_timer.pause();
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
			ui->onMessage("Squarez rules\n(click to continue)");
			break;
		case 3:
			_selection = findHorizontalTransition(board)._selection;
			ui->onSelectionAccepted(_selection);
		case 2:
			ui->onMessage("Spot similar elements at the edges of a square shape");
			break;
		case 4:
		case 8:
		{
			ui->onMessage("");
			squarez::Transition const & transition = board.selectSquare(_selection, false);
			ui->onTransition(transition);
			_score += transition._score;
			ui->onScoreChanged(_score);
			board.applyTransition(transition);
			break;
		}
		case 5:
			ui->onMessage("Bigger squares give more points");
			break;
		case 7:
			_selection = findGoodTransition(board)._selection;
			ui->onSelectionAccepted(_selection);
		case 6:
			ui->onMessage("Squares not aligned with the grid give double score");
			break;
		case 9:
			ui->onMessage("Time is limited\nEach square you select refills time based on its score");
			_timer.unPause();
			break;
		case 10:
			ui->onMessage("Total amount of available time gets shorter as you progress");
			_selection = squarez::Selection();
		default:
			if (_selection.getPoints().empty())
			{
				auto const & transitions = board.findTransitions();
				_selection = transitions.at(std::rand() % transitions.size())._selection;
				ui->onSelectionAccepted(_selection);
			}
			else
			{
				ui->onMessage("");
				auto const & transition = board.selectSquare(_selection, false);
				_score += transition._score;
				_timer.refill(transition._score * 2);
				ui->onScoreChanged(_score);
				ui->onTransition(transition);
				board.applyTransition(transition);
				_selection = squarez::Selection();
			}
	}
}

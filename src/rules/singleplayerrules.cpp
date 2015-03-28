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

#include "singleplayerrules.h"
#include "game/highscores.h"

squarez::SinglePlayerRules::SinglePlayerRules(Proxy & proxy, int board_size, int nb_symbols, int long_term, int short_term, int duration) :
	Rules(proxy, board_size, nb_symbols, std::random_device()(), Timer(std::chrono::seconds(long_term), std::chrono::seconds(short_term), std::chrono::seconds(duration))),
	_scoreSaved(false)
{}


bool squarez::SinglePlayerRules::gameOver()
{
	if (squarez::Rules::gameOver())
		return true;

	if (this->percentageLeft() > 0)
		return false;

	setGameOver(true);
	saveScore();

	return true;
}

void squarez::SinglePlayerRules::saveScore()
{
    HighScores & scores = accessHighScores();
    if (_scoreSaved or not scores.mayBeSaved(score()))
		return;

	if (_playerName.empty())
	{
		_proxy.nameRequired();
		return;
	}

	if (scores.save(score(), _playerName))
	{
		//FIXME: notify UI
	}
	_scoreSaved = true;
}

void squarez::SinglePlayerRules::setPlayerName(const std::string& name)
{
	if (not name.empty())
	{
		_playerName = name;
	}
	if (gameOver())
		saveScore();
}

void squarez::SinglePlayerRules::resetSelection()
{
	for(auto& i: _selection)
		_board->access(i).setSelected(false);

	_selection = Selection();
}


void squarez::SinglePlayerRules::onClick(squarez::Cell& cell)
{
	if (gameOver())
		return;
	cell.setSelected(_selection.togglePoint(cell.x(), cell.y()));
	Transition const& tr = _board->selectSquare(_selection, _random_generator, false);
	if (tr._score)
	{
		this->refillTimer(tr._score * 2);
		setScore(score() + tr._score);

		this->applyTransition(tr);
		resetSelection();
	}
}

void squarez::SinglePlayerRules::setPause(bool state)
{
	if (state == pause())
		return;
	pauseTimer(state);
}

squarez::HighScores & squarez::SinglePlayerRules::accessHighScores()
{
	static HighScores * scores = new HighScores("singlePlayer", 10);
	return *scores;
}

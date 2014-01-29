/*
 * Squarez puzzle game
 * Copyright (C) 2013  Guillaume Meunier <guillaume.meunier@centraliens.net>
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

#include "singleplayerrules.h"
#include "client/highscores.h"
#ifndef SQUAREZ_QT
#include "client/ui.h"
#endif
#include "shared/timer.h"

squarez::SinglePlayerRules::SinglePlayerRules(int board_size, int nb_symbols, int long_term, int short_term, int duration) :
	Rules(board_size, nb_symbols),
	timer(std::chrono::seconds(long_term), std::chrono::seconds(short_term), std::chrono::seconds(duration)),
	_scoreSaved(false)
{}

#ifndef SQUAREZ_QT
void squarez::SinglePlayerRules::setUI(squarez::UI* ui)
{
	squarez::Rules::setUI(ui);

	auto const& scores = accessHighScores().getScores();
	if (_ui and not scores.empty())
	{
		ui->onScoreListChanged(scores);
	}
}
#endif

bool squarez::SinglePlayerRules::checkGameOver()
{
	if (gameOver())
		return true;

	if (timer.percentageLeft() > 0)
		return false;

	setGameOver(true);
	saveScore();

	return true;
}

void squarez::SinglePlayerRules::saveScore()
{
    HighScores & scores = accessHighScores();
    if (_scoreSaved or not scores.mayBeSaved(getScore()))
		return;

	if (_playerName.empty())
	{
#ifdef SQUAREZ_QT
		emit nameRequired("");
#else
		_ui->nameRequired("");
#endif
		return;
	}

	if (scores.save(getScore(), _playerName))
	{
#ifndef SQUAREZ_QT
		_ui->onScoreListChanged(scores.getScores());
#endif
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

void squarez::SinglePlayerRules::onSelect(const squarez::Selection& selection)
{
	if (checkGameOver())
		return;
	Transition const& tr = _board->selectSquare(selection, false);
	if (tr._score)
	{
		timer.refill(tr._score * 2);
		setScore(getScore() + tr._score);

		this->applyTransition(tr);
	}
}

bool squarez::SinglePlayerRules::pause()
{
	return timer.paused();
}

void squarez::SinglePlayerRules::setPause(bool state)
{
	if (state == pause())
		return;
	timer.setPause(state);
#ifdef SQUAREZ_QT
	emit pauseChanged(state);
#endif
}

const squarez::Timer& squarez::SinglePlayerRules::getTimer()
{
	checkGameOver();
	return timer;
}

squarez::HighScores & squarez::SinglePlayerRules::accessHighScores()
{
	static HighScores * scores = new HighScores("singlePlayer", 10);
	return *scores;
}

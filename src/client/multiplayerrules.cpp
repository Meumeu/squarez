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

#include "multiplayerrules.h"
#include "ui.h"

#include "shared/network/methods.h"
#include "shared/serializer.h"

#include <climits>
#include <cmath>
#define INVALID_TOKEN UINT_MAX

#define NOP [](){}

bool squarez::MultiPlayerRules::gameOver()
{
	return false;
}

void squarez::MultiPlayerRules::onSelect(const squarez::Selection& selection)
{
	Transition tr = _board->selectSquare(selection, true);
	
	if (tr._score == 0)
		return;
	
	_xhr.request(_url + PushSelection::encodeRequest(selection, _token), std::bind(&MultiPlayerRules::onSelectionPushed, this, selection, std::placeholders::_1), NOP);
}

void squarez::MultiPlayerRules::initGame()
{
	if (_token != INVALID_TOKEN)
	{
		// Game is already initialized
		return;
	}
	if (_playerName.empty())
	{
		// Required parameters are missing
		return;
	}

	//Retrieve the game parameters
	StringDeSerializer ser(_xhr.request(_url + GameInit::encodeRequest(_playerName)));
	GameInit gameinit(ser);

	_token = gameinit._token;
	// Swap pointers to make sure original board is destroyed after notification
	_board.swap(gameinit._board);

	this->setTimer(squarez::Timer(gameinit._roundDuration, gameinit._roundProgress));

	_numberOfRounds = gameinit._numberOfRounds;
	_currentRound = gameinit._currentRound + 1;

#ifdef SQUAREZ_QT
	emit boardChanged(_board.get());
	emit numberOfRoundsChanged(_numberOfRounds);
	emit currentRoundChanged(_currentRound);
#endif

	// Start the round polling loop
	onTransitionPoll("");

	// Start the score polling loop
	onScoreListPoll("");
}

squarez::MultiPlayerRules::MultiPlayerRules(const std::string& url, const std::string& username): Rules(0,0, Timer(std::chrono::seconds(1)), username),
#ifdef SQUAREZ_QT
	_highScores(new HighScores(std::vector<Score>())),
#endif
#ifndef EMSCRIPTEN
	_mutex(new std::mutex()), _xhr(_mutex),
#endif
	_url(url), _token(INVALID_TOKEN), _currentRound(1)
{
	initGame();
}

void squarez::MultiPlayerRules::onTransitionPoll(const std::string& serializedTransition)
{
	if (not serializedTransition.empty())
	{
		StringDeSerializer ser(serializedTransition);
		TransitionPoll transitionPoll(ser);
		this->applyTransition(transitionPoll._transition);
		_currentRound = transitionPoll._round + 1;
		if (_currentRound == 1)
		{
			this->setScore(0);
		}
		this->refillTimer(200);
#ifdef SQUAREZ_QT
		emit currentRoundChanged(_currentRound);
#endif
	}

	_xhr.request(_url + TransitionPoll::encodeRequest(_token), std::bind(&MultiPlayerRules::onTransitionPoll, this, std::placeholders::_1), NOP);
}

void squarez::MultiPlayerRules::onSelectionPushed(Selection const &selection, const std::string& res)
{
	StringDeSerializer ser(res);
	unsigned int score;
	ser >> score;
	if (score > this->getScore())
		this->acceptSelection(selection);
	else
		this->rejectSelection(selection);
	this->setScore(score);
}

void squarez::MultiPlayerRules::setPlayerName(const std::string& name)
{
	if (_token != INVALID_TOKEN)
	{
		// Game is already initialized, ignore property change
		return;
	}
	_playerName = name;
	initGame();
}

void squarez::MultiPlayerRules::onScoreListPoll(const std::string& scoreList)
{
	if (not scoreList.empty())
	{
		StringDeSerializer ser(scoreList);
		ScoreList scores(ser);
#ifdef SQUAREZ_QT
		_highScores.reset(new HighScores(scores._scores));
		emit highScoresChanged(_highScores.get());
#else
		if (_ui)
			_ui->onScoreListChanged(scores._scores);
#endif
	}

	_xhr.request(_url + ScoreList::encodeRequest(),
		std::bind(&MultiPlayerRules::onScoreListPoll, this, std::placeholders::_1),
		std::bind(&MultiPlayerRules::onScoreListPoll, this, ""));
}

#ifdef SQUAREZ_QT
void squarez::MultiPlayerRules::setUrl(QString url)
{
	_url = url.toStdString();
	initGame();
}

#endif

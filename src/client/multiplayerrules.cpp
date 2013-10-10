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

#define NOP [](){}

bool squarez::MultiPlayerRules::gameOver()
{
	return false;
}

void squarez::MultiPlayerRules::onSelect(const squarez::Selection& selection)
{
	Transition tr = board.selectSquare(selection, true);
	
	if (tr._score == 0)
		return;
	
	_xhr.request(_url + PushSelection::encodeRequest(selection, _token), std::bind(&MultiPlayerRules::onSelectionPushed, this, selection, std::placeholders::_1), NOP);
}

void squarez::MultiPlayerRules::timeTick(std::chrono::duration<float>)
{
}

squarez::MultiPlayerRules::MultiPlayerRules(const std::string& url, const std::string& username): Rules(0,0, username), _timer(std::chrono::seconds(0)),
#ifndef EMSCRIPTEN
_xhr(_mutex),
#endif
_url(url), _score(0)
{
	//Retrieve the game parameters
	StringDeSerializer ser(_xhr.request(url + GameInit::encodeRequest(username)));
	GameInit gameinit(ser);
	
	_token = std::move(gameinit._token);
	board = std::move(gameinit._board);
	
	_timer = squarez::Timer(gameinit._roundDuration * gameinit._numberOfRounds,
		(gameinit._numberOfRounds - gameinit._currentRound - 1 + gameinit._roundProgress) / gameinit._numberOfRounds);

	_numberOfRounds = gameinit._numberOfRounds;

	// Start the round polling loop
	_xhr.request(url + TransitionPoll::encodeRequest(_token), std::bind(&MultiPlayerRules::onTransitionPoll, this, std::placeholders::_1), NOP);

}

void squarez::MultiPlayerRules::onTransitionPoll(const std::string& serializedTransition)
{
	StringDeSerializer ser(serializedTransition);
	TransitionPoll transitionPoll(ser);
	ui->onTransition(transitionPoll._transition);
	board.applyTransition(transitionPoll._transition);
	if (transitionPoll._round == 0)
	{
		ui->onScoreChanged(0);
		_score = 0;
		_timer.refill(200);
	}

	_xhr.request(_url + TransitionPoll::encodeRequest(_token), std::bind(&MultiPlayerRules::onTransitionPoll, this, std::placeholders::_1), NOP);
}

void squarez::MultiPlayerRules::onSelectionPushed(Selection const &selection, const std::string& res)
{
	StringDeSerializer ser(res);
	unsigned int score;
	ser >> score;
	if (score > _score)
		ui->onSelectionAccepted(selection);
	else
		ui->onSelectionRejected(selection);
	ui->onScoreChanged(score);
	_score = score;
}

void squarez::MultiPlayerRules::setPlayerName(const std::string& /*name*/)
{
	throw std::runtime_error("Name can not be changed in multiplayer mode");
}

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

#include "methods.h"

#include <sstream>

#include "shared/network/urltools.h"
#include "shared/selection.h"
#include "shared/serializer.h"

static const std::string request_path = "squarez/";

static const std::string gameinit_method = "game_init";
static const std::string pushselection_method = "push_selection";
static const std::string transitionpoll_method = "transition_poll";

namespace squarez
{

const std::string& GameInit::method()
{
	return gameinit_method;
}

std::string GameInit::encodeRequest(const std::string& playerName)
{
	return request_path + method() + "?name=" + urlTools::urlencode(playerName);
}

void GameInit::serialize(Serializer& serialized, const GameBoard& board, unsigned int token, std::chrono::seconds roundDuration, float roundProgress, unsigned int numberOfRounds, unsigned int currentRound)
{
	serialized << board << token << roundDuration.count() << roundProgress << numberOfRounds << currentRound;
}

GameInit::GameInit(Serializer& serialized): _board(serialized)
{
	std::int64_t roundDuration;
	serialized >> _token >> roundDuration >> _roundProgress >> _numberOfRounds >> _currentRound;
	_roundDuration = std::chrono::seconds(roundDuration);
}

std::string PushSelection::encodeRequest(const Selection& selection, unsigned int token)
{
	std::stringstream str;
	Serializer ser;
	ser << selection;

	str << request_path << method() << "?selection=" << ser.get() << "&token=" << token;
	return str.str();
}

const std::string& PushSelection::method()
{
	return pushselection_method;
}

const std::string& TransitionPoll::method()
{
	return transitionpoll_method;
}

std::string TransitionPoll::encodeRequest(unsigned int token)
{
	std::stringstream str;
	str << request_path << method() << "?token=" << token;
	return str.str();
}

TransitionPoll::TransitionPoll(Serializer& serialized): _transition(serialized)
{
	serialized >> _round;
}

void TransitionPoll::serialize(Serializer& serialized, unsigned int round, const Transition& transition)
{
	serialized << transition << round;
}

}

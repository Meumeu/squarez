/*
 * Squarez puzzle game server binary
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

#include "requesthandler.h"
#include "gamestatus.h"

#include "shared/serializer.h"

#include "shared/network/methods.h"

#include <boost/lexical_cast.hpp>
#include <functional>

namespace squarez
{

namespace {
unsigned int getToken(Fastcgipp::Http::Environment<char> const& env)
{
	return boost::lexical_cast<unsigned int>(env.findGet("token"));
}
}

bool RequestHandler::response()
{
	switch (_state)
	{
		case Init:
		{
			auto const& uri = environment().requestUri;
			std::string method = uri.substr(0, uri.find_first_of('?'));
			method = method.substr(method.find_last_of('/'));
			
			out << "Content-Type: text/plain\r\n\r\n";

			if (method == "/" + GameInit::method())
				return this->getBoard();

			if (method == "/" + ScoreList::method())
			{
				_state = GetScore;
				RWGameStatus status;
				status().registerWaitScore(callback());
				return false;
			}

			if (method == "/" + PushSelection::method())
				return this->pushSelection();

			if (method == "/" + TransitionPoll::method())
			{
				_state = GetTransition;
				RWGameStatus status;
				status().registerWaitTransition(callback(), getToken(environment()));
				return false;
			}

			if (method == "/num_players")
			{
				ROGameStatus status;
				out << status().getPlayersByScore().size();
				return true;
			}

			//FIXME: Unkown method, return something ?
			std::cerr << "Unkown method [" << method << "]" << std::endl;
			return true;
		}
		case GetTransition:
			return this->getTransition();
		case GetScore:
			return this->getScores();
	}
	return true;
}

bool RequestHandler::getBoard()
{
	unsigned int token = 0;
	std::string const& name = environment().findGet("name");

	RWGameStatus status;
	if (not name.empty())
	{
		token = status().registerPlayer(Player(name));
	}

	Serializer ser(out);
	GameInit::serialize(ser, status().getBoard(), token, status().getRoundDuration(), status().getRoundTimeAdvancement(), status()._roundsPerGame, status().getRound());

	return true;
}

bool RequestHandler::pushSelection()
{
	// Read the selection from parameters
	std::stringstream selectionString(environment().findGet("selection"));
	DeSerializer ser(selectionString);
	Selection selection(ser);

	unsigned int token = getToken(environment());

	out << RWGameStatus()().pushSelection(selection, token);
	return true;
}

bool RequestHandler::getScores()
{
	ROGameStatus status;
	auto const& players = status().getPlayersByScore();
	ScoreList scores;
	for (Player const & player: players)
	{
		scores._scores.push_back(Score(player.getScore(), player.getName()));
	}
	Serializer ser(out);
	ser << scores;
	return true;
}

bool RequestHandler::getTransition()
{
	Serializer ser(out);
	{
		ROGameStatus status;
		TransitionPoll::serialize(ser, status().getRound(), status().getLastRoundTransition());
	}
	return true;
}

}

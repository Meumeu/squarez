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

#include <boost/lexical_cast.hpp>

namespace squarez
{

bool RequestHandler::response()
{
	switch (_state)
	{
		case Init:
		{
			auto const& path = environment().pathInfo;
			if (path.empty())
			{
				std::cerr << "No path information" << std::endl;
				return true;
			}
			out << "Content-Type: text/plain\r\n\r\n";
			std::string method = path.front();

			if (method == "get_board")
				return this->getBoard();

			if (method == "get_scores")
				return this->getScores();

			if (method == "push_selection")
				return this->pushSelection();

			if (method == "get_transition")
			{
				_state = GetTransition;
				RWGameStatus()().registerWait(callback());
				return false;
			}

			//FIXME: Unkown method, return something ?
			std::cerr << "Unkown method [" << method << "]" << std::endl;
			return true;
		}
		case GetTransition:
		{
			return this->getTransition();
		}
	}
	return true;
}

bool RequestHandler::getBoard()
{
	RWGameStatus status;
	out << "{";

	out << "\"board\":\"";
	status().getBoard().serialize(out);

	out << "\",\"timer\":";
	out << status().getRoundDuration().count() << ",";

	out << "\"progress\":" << status().getRoundTimeAdvancement() << ",";

	out << "\"round\":" << status().getRound() << ",";
	out << "\"gameRounds\":" << status()._roundsPerGame;

	std::string const& name = environment().findGet("name");
	if (not name.empty())
	{
		auto token = status().registerPlayer(Player(name));
		out << ",\"token\":" << token;
	}
	out << "}";
	return true;
}

bool RequestHandler::pushSelection()
{
	// Read the selection from parameters
	std::string const& selectionString = environment().findGet("selection");
	std::stringstream stream(selectionString);
	Selection selection(stream);

	unsigned int token = boost::lexical_cast<unsigned int>(environment().findGet("token"));

	out << RWGameStatus()().pushSelection(selection, token);
	return true;
}

bool RequestHandler::getScores()
{
	ROGameStatus status;
	auto const& players = status().getPlayersByScore();
	out << "{\"scores\":[";
	bool first = true;
	for (auto const & player: players)
	{
		if (first) first = false;
		else out << ",";
		out << "{\"name\":\"" << player.get().getName() << "\",";
		out << "\"score\":" << player.get().getScore() << "}";
	}
	out << "]}";
	return true;
}

bool RequestHandler::getTransition()
{
	out << "{\"transition\":\"";
	ROGameStatus status;
	status().getLastRoundTransition().serialize(out);
	out << "\",\"round\":" << status().getRound() << "}";
	return true;
}

}

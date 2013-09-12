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

namespace squarez
{

bool RequestHandler::response()
{
	switch (_state)
	{
		case Init:
		{
			out << "Content-Type: text/plain\r\n\r\n";
			auto const& scriptName = environment().scriptName;
			std::string method = scriptName.substr(scriptName.find_last_of('/'));

			if (method == "/get_board")
				return this->getBoard();

			if (method == "/push_selection")
				return this->pushSelection();

			if (method == "/get_transition")
			{
				_state = LongPoll;
				RWGameStatus()().registerWait(callback());
				return false;
			}

			//FIXME: Unkown method, return something ?
			return true;
		}
		case LongPoll:
			out << "Waked after timer\r\n";
			return true;
	}
	return true;
}

bool RequestHandler::getBoard()
{
	ROGameStatus()().getBoard().serialize(out);
	return true;
}

bool RequestHandler::pushSelection()
{
	// Read the selection from parameters
	std::string const& selectionString = environment().findGet("selection");
	std::stringstream stream(selectionString);
	Selection selection(stream);

	out << RWGameStatus()().pushSelection(selection);
	return true;
}


}

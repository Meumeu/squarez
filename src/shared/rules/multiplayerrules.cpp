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
	
	_xhr.request(_url + "squarez/push_selection?selection=" + selection.serialize() + "&token=" + _token, NOP, NOP);
}

void squarez::MultiPlayerRules::timeTick(std::chrono::duration<float>)
{
}

squarez::MultiPlayerRules::MultiPlayerRules(const std::string& url, const std::string& username): Rules(0,0), _xhr(_mutex), _url(url), _username(username)
{
	Settings settings(squarez::Serializer(_xhr.request(url + "squarez/register?name=" + _xhr.urlencode(username))));
	
	_token = std::move(settings._token);
	board = std::move(settings._board);
	
	_timer = squarez::Timer(settings._roundDuration * settings._numberOfRounds,
		(settings._numberOfRounds - settings._currentRound - 1 + settings._roundProgress) / settings._numberOfRounds);

}

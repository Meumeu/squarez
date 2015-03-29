/*
 * Squarez puzzle game
 * Copyright (C) 2013  Guillaume Meunier <guillaume.meunier@centraliens.net>
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

#include "methods.h"
#include "urltools.h"
#include "game/selection.h"
#include "utils/serializer.h"

#include <sstream>

static const std::string request_path = "squarez/";

static const std::string sp_gameinit_method = "0";
static const std::string sp_pushselection_method = "1";

const std::string& squarez::onlineSinglePlayer::GameInit::method()
{
	return sp_gameinit_method;
}

std::string squarez::onlineSinglePlayer::GameInit::encodeRequest(const std::string& playerName, unsigned int size, unsigned int symbols)
{
	std::stringstream stream;
	stream << request_path << method()
		<< "?name=" << urlTools::urlencode(playerName)
		<< "&size=" << size
		<< "&symbols=" << symbols;
	return stream.str();
}

squarez::onlineSinglePlayer::GameInit::GameInit(squarez::DeSerializer& serialized):
	_token(serialized.read<decltype(_token)>()), _seed(serialized.read<decltype(_seed)>())
{
}

void squarez::onlineSinglePlayer::GameInit::serialize(squarez::Serializer& serialized, unsigned int token, uint_fast32_t seed)
{
	serialized << token << seed;
}

const std::string& squarez::onlineSinglePlayer::PushSelection::method()
{
	return sp_pushselection_method;
}

std::string squarez::onlineSinglePlayer::PushSelection::encodeRequest(const squarez::Selection& selection, unsigned int token, std::chrono::milliseconds msSinceEpoch)
{
	StringSerializer sel;
	sel << selection;
	std::stringstream stream;
	stream << request_path << method()
	<< "?token=" << token
	<< "&selection="
	<< urlTools::urlencode(sel.get())
	<< "&msSinceEpoch=" << msSinceEpoch.count();
	return stream.str();
}

squarez::onlineSinglePlayer::PushSelection::PushSelection(squarez::DeSerializer& serialized):
_gameOver(serialized.read<decltype(_gameOver)>())
{
}

void squarez::onlineSinglePlayer::PushSelection::serialize(squarez::Serializer& serialized, bool gameOver)
{
	serialized << gameOver;
}
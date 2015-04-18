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
static const std::string sp_pause_method = "2";
static const std::string get_scores = "3";

const std::string& squarez::onlineSinglePlayer::GameInit::method()
{
	return sp_gameinit_method;
}

std::string squarez::onlineSinglePlayer::GameInit::encodeRequest(const std::string& playerName, std::uint32_t size, unsigned int symbols)
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

void squarez::onlineSinglePlayer::GameInit::serialize(squarez::Serializer& serialized, std::uint32_t token, uint_fast32_t seed)
{
	serialized << token << seed;
}

const std::string& squarez::onlineSinglePlayer::PushSelection::method()
{
	return sp_pushselection_method;
}

std::string squarez::onlineSinglePlayer::PushSelection::encodeRequest(const squarez::Selection& selection, std::uint32_t token, std::chrono::milliseconds msSinceEpoch)
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

const std::string& squarez::onlineSinglePlayer::Pause::method()
{
	return sp_pause_method;
}

std::string squarez::onlineSinglePlayer::Pause::encodeRequest(std::uint32_t token, bool pause, std::chrono::milliseconds msSinceEpoch)
{
	std::stringstream stream;
	stream << request_path << method()
	<< "?token=" << token
	<< "&pause=" << (pause ? "1" : "0")
	<< "&msSinceEpoch=" << msSinceEpoch.count();
	return stream.str();
}

namespace squarez
{
static squarez::Serializer& operator<<(squarez::Serializer& ser, const squarez::onlineSinglePlayer::GetScores::Score& score)
{
	return ser << score._playerName << score._score << score._date;
}

static squarez::DeSerializer& operator>>(squarez::DeSerializer& ser, squarez::onlineSinglePlayer::GetScores::Score& score)
{
	return ser >> score._playerName >> score._score >> score._date;
}
}

const std::string& squarez::onlineSinglePlayer::GetScores::method()
{
	return get_scores;
}

std::string squarez::onlineSinglePlayer::GetScores::encodeRequest(int maxAge, int count)
{
	std::stringstream stream;
	stream << request_path << method()
	<< "?age=" << maxAge
	<< "&count=" << count;
	return stream.str();
}

squarez::onlineSinglePlayer::GetScores::GetScores(squarez::DeSerializer& serialized)
{
	serialized >> _scores;
}

void squarez::onlineSinglePlayer::GetScores::serialize(squarez::Serializer& serialized, std::vector<squarez::onlineSinglePlayer::GetScores::Score> scores)
{
	serialized << scores;
}

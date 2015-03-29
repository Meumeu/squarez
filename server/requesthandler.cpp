/*
 * Squarez puzzle game server binary
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

#include "requesthandler.h"
#include "serverrules.h"
#include "network/methods.h"
#include "utils/serializer.h"

#include <memory>
#include <random>
#include <unordered_map>

#ifndef MAX_CONCURRENT_GAMES
#define MAX_CONCURRENT_GAMES 1024
#endif

typedef std::unordered_map<unsigned int, std::unique_ptr<squarez::ServerRules>> games_t;

static unsigned int last_token = 0;
static games_t games;

namespace
{
	unsigned int getToken(Fastcgipp::Http::Environment<char> const& env)
	{
		return boost::lexical_cast<unsigned int>(env.findGet("token"));
	}
	squarez::ServerRules & getGame(unsigned int token)
	{
		// Cleanup finished games
		if (games.size() > MAX_CONCURRENT_GAMES)
		{
			std::vector<unsigned int> endedGames;
			for (const auto & game : games)
			{
				if (game.second->msLeft() < -60000)
					endedGames.push_back(game.first);
			}
			for (auto i : endedGames)
				games.erase(i);
		}
		auto game = games.find(token);
		if (game == games.end())
			throw std::runtime_error("No game for token");
		return *game->second;
	}

	std::mt19937::result_type getSeed()
	{
		static std::mt19937 generator = std::mt19937(std::random_device()());
		std::uniform_int_distribution<std::mt19937::result_type> distribution;
		return distribution(generator);
	}
}

bool squarez::RequestHandler::response()
{
	auto const& uri = environment().requestUri;
	std::string method = uri.substr(0, uri.find_first_of('?'));
	method = method.substr(method.find_last_of('/'));

	out << "Content-Type: text/plain\r\n\r\n";

	if (method == "/" + onlineSinglePlayer::GameInit::method())
	{
		std::string const& name = environment().findGet("name");
		unsigned int size = boost::lexical_cast<unsigned int>(environment().findGet("size"));
		unsigned int symbols = boost::lexical_cast<unsigned int>(environment().findGet("symbols"));
		unsigned int token = ++last_token;
		auto seed = getSeed();
		games[token] = std::unique_ptr<ServerRules>(new ServerRules(name, seed, size, symbols));
		Serializer ser(out);
		onlineSinglePlayer::GameInit::serialize(ser, token, seed);
		return true;
	}
	else if (method == "/" + onlineSinglePlayer::PushSelection::method())
	{
		auto token = getToken(environment());
		auto & game = getGame(token);

		// Read the selection from parameters
		std::stringstream selectionString(environment().findGet("selection"));
		DeSerializer deSer(selectionString);
		Selection selection(deSer);
		std::chrono::milliseconds msSinceEpoch{boost::lexical_cast<int>(environment().findGet("msSinceEpoch"))};
		bool gameOver = game.playSelection(selection, msSinceEpoch);

		if (gameOver)
		{
			games.erase(token);
		}

		Serializer ser(out);
		onlineSinglePlayer::PushSelection::serialize(ser, gameOver);
	}
	// Unkown method, return something ?
	err << "Unkown method [" << method << "]" << std::endl;
	return true;
}

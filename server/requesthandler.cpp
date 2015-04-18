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
#include "game/constants.h"
#include "network/methods.h"
#include "utils/serializer.h"

#include <condition_variable>
#include <memory>
#include <mutex>
#include <random>
#include <thread>
#include <unordered_map>


namespace
{
	class Games
	{
		std::mutex _mutex;
		std::uint32_t _last_token;
		std::unordered_map<std::uint32_t, std::shared_ptr<squarez::ServerRules>> _games;
		bool _alive;
		std::condition_variable _gcWait;
		std::thread _gc;

	public:
		Games(): _last_token(0), _alive(true), _gc([this](){return gcThread();}) {}
		~Games()
		{
			std::unique_lock<std::mutex> lock(_mutex);
			_alive = false;
			lock.unlock();
			_gcWait.notify_all();

			_gc.join();
		}

		void eraseGame(std::uint32_t token)
		{
			std::unique_lock<std::mutex> lock(_mutex);
			_games.erase(token);
		}

		std::shared_ptr<squarez::ServerRules> getGame(std::uint32_t token)
		{
			std::unique_lock<std::mutex> lock(_mutex);
			return _games.at(token);
		}

		std::uint32_t storeGame(std::shared_ptr<squarez::ServerRules> game)
		{
			std::unique_lock<std::mutex> lock(_mutex);
			_games[++_last_token] = game;
			return _last_token;
		}
	private:
		void gcThread()
		{
			std::unique_lock<std::mutex> lock(_mutex);
			while(_alive)
			{
				garbageCollect();
				_gcWait.wait_for(lock, std::chrono::minutes(1));
			}
		}
		void garbageCollect()
		{
			std::vector<std::uint32_t> endedGames;
			for (const auto & game : _games)
			{
				if (game.second->msLeft() < -60000 or std::chrono::steady_clock::now() - game.second->_epoch > std::chrono::hours(24))
					endedGames.push_back(game.first);
			}
			for (auto i : endedGames)
				_games.erase(i);
		}
	};

	std::uint32_t getToken(Fastcgipp::Http::Environment<char> const& env)
	{
		return boost::lexical_cast<std::uint32_t>(env.findGet("token"));
	}

	std::mt19937::result_type getSeed()
	{
		static std::mt19937 generator = std::mt19937(std::random_device()());
		return generator();
	}
}

static Games games;
std::shared_ptr<squarez::HighScores> squarez::RequestHandler::highScores;

bool squarez::RequestHandler::response()
{
	auto const& uri = environment().requestUri;
	std::string method = uri.substr(0, uri.find_first_of('?'));
	method = method.substr(method.find_last_of('/'));

	if (method == "/" + onlineSinglePlayer::GameInit::method())
	{
		std::string const& name = environment().findGet("name");
		unsigned int size = boost::lexical_cast<unsigned int>(environment().findGet("size"));
		unsigned int symbols = boost::lexical_cast<unsigned int>(environment().findGet("symbols"));
		auto seed = getSeed();
		auto token = games.storeGame(std::make_shared<ServerRules>(
			name, seed, size, symbols, constants::default_timer(), highScores));

		out << "Content-Type: text/plain\r\n\r\n";
		Serializer ser(out);
		onlineSinglePlayer::GameInit::serialize(ser, token, seed);
	}
	else if (method == "/" + onlineSinglePlayer::PushSelection::method())
	{
		auto token = getToken(environment());
		auto game = games.getGame(token);

		// Read the selection from parameters
		std::chrono::milliseconds msSinceEpoch{boost::lexical_cast<int>(environment().findGet("msSinceEpoch"))};
		bool gameOver = game->playSelection(environment().findGet("selection"), msSinceEpoch);

		if (gameOver)
		{
			games.eraseGame(token);
		}

		out << "Content-Type: text/plain\r\n\r\n";
		Serializer ser(out);
		onlineSinglePlayer::PushSelection::serialize(ser, gameOver);
	}
	else if (method == "/" + onlineSinglePlayer::Pause::method())
	{
		auto token = getToken(environment());
		auto game = games.getGame(token);
		bool pause = boost::lexical_cast<bool>(environment().findGet("pause"));
		std::chrono::milliseconds msSinceEpoch{boost::lexical_cast<int>(environment().findGet("msSinceEpoch"))};
		game->setPause(pause, msSinceEpoch);

		out << "Content-Type: text/plain\r\n\r\n";
	}
	else if (method == "/" + onlineSinglePlayer::GetScores::method())
	{
		out << "Content-Type: text/plain\r\n\r\n";

		int count = environment().checkForGet("count") ? std::min(20, boost::lexical_cast<int>(environment().findGet("count"))) : 20;
		int age = environment().checkForGet("age") ? boost::lexical_cast<int>(environment().findGet("age")) : 0;

		Serializer ser(out);
		onlineSinglePlayer::GetScores::serialize(ser, highScores->getScores(age, count));
	}
	else
	{
		std::stringstream ss;
		ss << "Unknown method [" << method << "]";
		throw std::runtime_error(ss.str());
	}
	return true;
}

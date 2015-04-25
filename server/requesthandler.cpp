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

#include "config.h"
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
#include <typeinfo>

#ifdef HAVE_CXXABI_H
#include <cxxabi.h>

namespace
{
std::string demangle(const std::string& mangled)
{
	int status;
	char * realname = abi::__cxa_demangle(mangled.c_str(), nullptr, nullptr, &status);

	if (realname)
	{
		std::string ret(realname);
		free(realname);
		return ret;
	}
	else
	{
		return mangled;
	}
}
}
#else
namespace
{
std::string demangle(const std::string& mangled)
{
	return mangled;
}
}
#endif

namespace
{
	class LockedGame
	{
		std::unique_lock<std::mutex> _lock;
		std::shared_ptr<squarez::ServerRules> _game;
	public:
		LockedGame(std::shared_ptr<squarez::ServerRules> game):
			_lock(game->_mutex), _game(game) {}

		squarez::ServerRules & game() { return *_game; }
	};

	class Games
	{
		std::mutex _mutex;
		std::uint32_t _last_token;
		std::unordered_map<std::uint32_t, std::shared_ptr<squarez::ServerRules>> _games;
		bool _alive;
		std::condition_variable _gcWait;
		std::thread _gc;

	public:
		Games(): _last_token(0), _alive(true), _gc([this](){ gcThread(); }) {}
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

		LockedGame getGame(std::uint32_t token)
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
				try
				{
					garbageCollect();
					_gcWait.wait_for(lock, std::chrono::minutes(1));
				}
				catch(std::exception& e)
				{
					std::cerr << "Exception " << demangle(typeid(e).name()) << " in garbage collector thread" << std::endl;
					std::cerr << e.what() << std::endl;
				}
				catch(...)
				{
					std::cerr << "Unexpected exception in garbage collector thread" << std::endl;
				}
			}
		}
		void garbageCollect()
		{
			std::vector<std::uint32_t> endedGames;
			for (const auto & pair : _games)
			{
				LockedGame game(pair.second);
				if (game.game().msLeft() < -60000 or std::chrono::steady_clock::now() - game.game()._epoch > std::chrono::hours(24))
					endedGames.push_back(pair.first);
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

	try
	{
		if (method == "/" + onlineSinglePlayer::GameInit::method())
		{
			std::string const& name = environment().findGet("name");
			unsigned int size = boost::lexical_cast<unsigned int>(environment().findGet("size"));
			unsigned int symbols = boost::lexical_cast<unsigned int>(environment().findGet("symbols"));
			auto seed = getSeed();
			auto token = games.storeGame(std::make_shared<ServerRules>(
				name, seed, size, symbols, constants::default_timer(), highScores));

			out << "Content-Type: text/plain; charset=utf-8\r\n\r\n";
			Serializer ser(out);
			onlineSinglePlayer::GameInit::serialize(ser, token, seed);
		}
		else if (method == "/" + onlineSinglePlayer::PushSelection::method())
		{
			auto token = getToken(environment());
			auto game = games.getGame(token);

			// Read the selection from parameters
			std::chrono::milliseconds msSinceEpoch{boost::lexical_cast<int>(environment().findGet("msSinceEpoch"))};
			bool gameOver = game.game().playSelection(environment().findGet("selection"), msSinceEpoch);

			if (gameOver)
			{
				games.eraseGame(token);
			}

			out << "Content-Type: text/plain; charset=utf-8\r\n\r\n";
			Serializer ser(out);
			onlineSinglePlayer::PushSelection::serialize(ser, gameOver);
		}
		else if (method == "/" + onlineSinglePlayer::Pause::method())
		{
			auto token = getToken(environment());
			auto game = games.getGame(token);
			bool pause = boost::lexical_cast<bool>(environment().findGet("pause"));
			std::chrono::milliseconds msSinceEpoch{boost::lexical_cast<int>(environment().findGet("msSinceEpoch"))};
			game.game().setPause(pause, msSinceEpoch);

			out << "Content-Type: text/plain; charset=utf-8\r\n\r\n";
		}
		else if (method == "/" + onlineSinglePlayer::GetScores::method())
		{
			out << "Content-Type: text/plain; charset=utf-8\r\n\r\n";

			int count = environment().checkForGet("count") ? std::min(20, boost::lexical_cast<int>(environment().findGet("count"))) : 20;
			std::time_t min_date = 0;
			std::time_t max_date = std::time(nullptr);

			if (environment().checkForGet("age"))
			{
				int age = boost::lexical_cast<int>(environment().findGet("age"));
				if (age) min_date = max_date - age;
			}

			if (environment().checkForGet("min_date"))
				min_date = boost::lexical_cast<std::time_t>(environment().findGet("min_date"));

			if (environment().checkForGet("max_date"))
				max_date = boost::lexical_cast<std::time_t>(environment().findGet("max_date"));

			Serializer ser(out);
			onlineSinglePlayer::GetScores::serialize(ser, highScores->getScores(min_date, max_date, count));
		}
		else
		{
			std::stringstream ss;
			ss << "Unknown method [" << method << "]";
			throw std::runtime_error(ss.str());
		}
	}
	catch(std::exception& e)
	{
		std::cerr << "Exception " << demangle(typeid(e).name()) << " while processing " << uri << ": " << std::endl;
		std::cerr << e.what() << std::endl;

		throw;
	}
	return true;
}

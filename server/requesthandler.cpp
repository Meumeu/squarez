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
#include "network/urltools.h"
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

const std::string error_400 =
	"HTTP/1.1 400 Bad Request\r\n"
	"Content-Type: text/plain; charset=utf-8\r\n"
	"Content-Length: 13\r\n"
	"\r\n"
	"Bad request\r\n";

const std::string error_500 =
	"HTTP/1.1 500 Internal Server Error\r\n"
	"Content-Type: text/plain; charset=utf-8\r\n"
	"Content-Length: 23\r\n"
	"\r\n"
	"Internal Server Error\r\n";

namespace
{
	class LockedGame
	{
		std::unique_lock<std::mutex> _lock;
		std::shared_ptr<squarez::ServerRules> _game;
	public:
		LockedGame() {}

		LockedGame(LockedGame&& rhs):
			_lock(std::move(rhs._lock)), _game(std::move(rhs._game)) {}

		LockedGame(const LockedGame&) = delete;

		LockedGame& operator=(LockedGame&& rhs)
		{
			_lock = std::move(rhs._lock);
			_game = std::move(rhs._game);
			return *this;
		}

		LockedGame& operator=(const LockedGame& rhs) = delete;

		LockedGame(std::shared_ptr<squarez::ServerRules> game):
			_lock(game->_mutex), _game(game) {}

		squarez::ServerRules & game()
		{
			if (_game)
				return *_game;
			else
				throw std::runtime_error("null reference");
		}
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

	std::uint32_t getToken(std::unordered_map<std::string, std::string>& env)
	{
		if (env.find("token") == env.end())
			throw std::runtime_error("No token in arguments");

		return boost::lexical_cast<std::uint32_t>(env["token"]);
	}

	std::mt19937::result_type getSeed()
	{
		static std::mt19937 generator = std::mt19937(std::random_device()());
		return generator();
	}
}

static Games games;

squarez::RequestHandler::RequestHandler(squarez::HighScores& highScores) : _highScores(highScores)
{
}

void squarez::RequestHandler::gameInit(Response& response, std::shared_ptr<Request> request)
{
	std::stringstream out;
	std::string name;
	unsigned int size;
	unsigned int symbols;
	try
	{
		std::unordered_map<std::string, std::string> get = squarez::urlTools::parseGet(request->path);

		name = get["name"];
		size = boost::lexical_cast<unsigned int>(get["size"]);
		symbols = boost::lexical_cast<unsigned int>(get["symbols"]);
	}
	catch(std::exception& e)
	{
		response << error_400;
		return;
	}

	try
	{
		auto seed = getSeed();
		auto token = games.storeGame(std::make_shared<ServerRules>(
			name, seed, size, symbols, constants::default_timer(), _highScores));

		Serializer ser(out);
		onlineSinglePlayer::GameInit::serialize(ser, token, seed);

		response << "HTTP/1.1 200 OK\r\nContent-Type: text/plain; charset=utf-8\r\nContent-Length: " << out.str().size() << "\r\n\r\n" << out.str();
	}
	catch(std::exception& e)
	{
		response << error_500;

		std::cerr << "Exception " << demangle(typeid(e).name()) << " while processing " << request->path << ": " << std::endl;
		std::cerr << e.what() << std::endl;
	}
}

void squarez::RequestHandler::pushSelection(Response& response, std::shared_ptr<Request> request)
{
	std::stringstream out;
	uint32_t token;
	LockedGame game;
	std::chrono::milliseconds msSinceEpoch;
	std::string selection;

	try
	{
		std::unordered_map<std::string, std::string> get = squarez::urlTools::parseGet(request->path);

		token = getToken(get);
		game = games.getGame(token);
		selection = get["selection"];
		msSinceEpoch = std::chrono::milliseconds{boost::lexical_cast<int>(get["msSinceEpoch"])};
	}
	catch(std::exception& e)
	{
		response << error_400;
		return;
	}

	try
	{
		bool gameOver = game.game().playSelection(selection, msSinceEpoch);

		if (gameOver)
		{
			games.eraseGame(token);
		}

		Serializer ser(out);
		onlineSinglePlayer::PushSelection::serialize(ser, gameOver);
		response << "HTTP/1.1 200 OK\r\nContent-Type: text/plain; charset=utf-8\r\nContent-Length: " << out.str().size() << "\r\n\r\n" << out.str();
	}
	catch(std::exception& e)
	{
		response << error_500;

		std::cerr << "Exception " << demangle(typeid(e).name()) << " while processing " << request->path << ": " << std::endl;
		std::cerr << e.what() << std::endl;
	}
}

void squarez::RequestHandler::pause(Response& response, std::shared_ptr<Request> request)
{
	std::stringstream out;
	uint32_t token;
	LockedGame game;
	std::chrono::milliseconds msSinceEpoch;
	bool pause;
	try
	{
		std::unordered_map<std::string, std::string> get = squarez::urlTools::parseGet(request->path);

		token = getToken(get);
		game = games.getGame(token);
		pause = boost::lexical_cast<bool>(get["pause"]);
		msSinceEpoch = std::chrono::milliseconds{boost::lexical_cast<int>(get["msSinceEpoch"])};
	}
	catch(std::exception& e)
	{
		response << error_400;
		return;
	}

	try
	{
		game.game().setPause(pause, msSinceEpoch);

		Serializer ser(out);
		response << "HTTP/1.1 200 OK\r\nContent-Type: text/plain; charset=utf-8\r\nContent-Length: " << out.str().size() << "\r\n\r\n" << out.str();
	}
	catch(std::exception& e)
	{
		response << error_500;

		std::cerr << "Exception " << demangle(typeid(e).name()) << " while processing " << request->path << ": " << std::endl;
		std::cerr << e.what() << std::endl;
	}
}

void squarez::RequestHandler::getScores(Response& response, std::shared_ptr<Request> request)
{
	std::stringstream out;
	int count;
	std::time_t min_date = 0;
	std::time_t max_date = std::time(nullptr);
	try
	{
		std::unordered_map<std::string, std::string> get = squarez::urlTools::parseGet(request->path);

		count = get.find("count") == get.end() ? 20 : std::min(20, boost::lexical_cast<int>(get["count"]));

		if (get.find("age") != get.end())
		{
			int age = boost::lexical_cast<int>(get["age"]);
			if (age) min_date = max_date - age;
		}

		if (get.find("min_date") != get.end())
			min_date = boost::lexical_cast<std::time_t>(get["min_date"]);

		if (get.find("max_date") != get.end())
			max_date = boost::lexical_cast<std::time_t>(get["max_date"]);
	}
	catch(std::exception& e)
	{
		response << error_400;
		return;
	}

	try
	{
		Serializer ser(out);
		onlineSinglePlayer::GetScores::serialize(ser, _highScores.getScores(min_date, max_date, count));
		response << "HTTP/1.1 200 OK\r\nContent-Type: text/plain; charset=utf-8\r\nContent-Length: " << out.str().size() << "\r\n\r\n" << out.str();
	}
	catch(std::exception& e)
	{
		response << error_500;

		std::cerr << "Exception " << demangle(typeid(e).name()) << " while processing " << request->path << ": " << std::endl;
		std::cerr << e.what() << std::endl;
	}

}


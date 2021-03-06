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

#ifndef SQUAREZ_METHODS_H
#define SQUAREZ_METHODS_H

#include <chrono>
#include <random>
#include <string>

namespace squarez
{
class DeSerializer;
class Serializer;
class Selection;

namespace onlineSinglePlayer
{

// Get the parameters of a ranked name
struct GameInit
{
	static std::string const& method();
	static std::string encodeRequest(std::string const& playerName, unsigned int size, std::uint32_t symbols);

	GameInit(DeSerializer & serialized);

	static void serialize(Serializer & serialized,
		std::uint32_t token,
		std::mt19937::result_type seed);

	const std::uint32_t _token;
	const std::mt19937::result_type _seed;

};

struct PushSelection
{
	static std::string const& method();
	static std::string encodeRequest(Selection const& selection, std::uint32_t token, std::chrono::milliseconds msSinceEpoch);

	PushSelection(DeSerializer & serialized);

	static void serialize(Serializer & serialized,
		bool gameOver);
	const bool _gameOver;
};

struct Pause
{
	static std::string const & method();
	static std::string encodeRequest(std::uint32_t token, bool pause, std::chrono::milliseconds msSinceEpoch);
};


struct GetScores
{
	struct Score
	{
		std::string _playerName;
		int _score;
		std::string _date;
	};

	static std::string const& method();
	static std::string encodeRequest(int maxAge, int count);
	static std::string encodeRequest(std::time_t minDate, std::time_t maxDate, int count);

	GetScores(DeSerializer & serialized);

	static void serialize(Serializer & serialized, std::vector<Score> scores);
	std::vector<Score> _scores;
};

}
}

#endif // SQUAREZ_METHODS_H

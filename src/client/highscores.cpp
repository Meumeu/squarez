/*
 * Squarez puzzle game
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

#include "highscores.h"
#include <shared/serializer.h>
#include <cstdlib>

#ifdef EMSCRIPTEN
#include <sstream>
#include <emscripten/val.h>
class LocalStorage
{
public:
	LocalStorage(std::string const& fileName, std::ios_base::openmode mode):
		_fileName(fileName), _mode(mode)
	{
		if (_mode | std::ios_base::in)
		{
			emscripten::val localStorage = emscripten::val::global("localStorage");
			auto const & file = localStorage.call<emscripten::val>("getItem", fileName);
			if (file.as<bool>())
			{
				_stream = std::stringstream(file.as<std::string>());
			}
		}
	}

	~LocalStorage()
	{
		if (_mode | std::ios_base::out)
		{
			emscripten::val localStorage = emscripten::val::global("localStorage");
			localStorage.call<void>("setItem", _fileName, _stream.str());
		}
	}

	operator std::stringstream& () { return _stream; }
private:
	std::string _fileName;
	std::ios_base::openmode _mode;
	std::stringstream _stream;
};
typedef LocalStorage persistent_t;
#else
#include <fstream>
typedef std::fstream persistent_t;
#endif

#include "config.h"

static const std::string fileName = "scores";

namespace {
std::string getenv(const char * variable, std::string const& defaultValue)
{
	const char * value = std::getenv(variable);
	if (value and *value != '\0')
		return value;
	return defaultValue;
}

std::string getFileName()
{
	return getenv("XDG_DATA_HOME", getenv("HOME", "") + "/.local/share") + "/" PACKAGE "/" + fileName;
}

}

squarez::HighScores::HighScores(unsigned int maxScores) : _maxScores(maxScores)
{
	// Try to deserialize scores from "file"
	persistent_t f(getFileName(), std::ios::in);
	try
	{
		DeSerializer ser(f);
		ser >> _scores;
	}
	catch (...)
	{
		_scores.clear();
	}

}

bool squarez::HighScores::save(unsigned int score, const std::string& name)
{
	if (not mayBeSaved(score))
		return false;

	_scores.insert(Score(score, name));

	persistent_t f(getFileName(), std::ios_base::out | std::ios_base::trunc);
	try
	{
		Serializer ser(f);
		ser << _scores;
	}
	catch (...)
	{
		std::cerr << "Failed to save high scores" << std::endl;
	}
	return true;
}

bool squarez::HighScores::mayBeSaved(unsigned int score)
{
	return score > 0 and (_scores.size() < _maxScores or score > _scores.rbegin()->_score);
}

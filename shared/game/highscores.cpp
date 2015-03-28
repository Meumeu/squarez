/*
 * Squarez puzzle game
 * Copyright (C) 2015  Guillaume Meunier <guillaume.meunier@centraliens.net>
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



#include "highscores.h"
#include "utils/serializer.h"
#include <cstdlib>
#include <algorithm>

#ifdef SQUAREZ_QT
#include <QDateTime>
#include <QDir>
#endif

#ifndef PACKAGE
#include "config.h"
#endif

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

	operator bool() { return not _stream.str().empty();}
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

namespace {
std::string getenv(const char * variable, std::string const& defaultValue)
{
	const char * value = std::getenv(variable);
	if (value and *value != '\0')
		return value;
	return defaultValue;
}

std::string getDir()
{
#ifdef _WIN32
	return getenv("APPDATA", ".") + "/" PACKAGE "/";
#else
	return getenv("XDG_DATA_HOME", getenv("HOME", "") + "/.local/share") + "/" PACKAGE "/";
#endif
}

void mkdir(const std::string & dir)
{
#if defined(EMSCRIPTEN)
	// mkdir is not required for javascript, just silence the warning
	(void)dir;
#elif defined(SQUAREZ_BOT)
	// mkdir is not required for testing, just silence the warning
	(void)dir;
#elif defined(SQUAREZ_QT)
	QDir().mkpath(QString::fromStdString(dir));
#else
#error mkdir not implemented
#endif
}

}

squarez::HighScores::HighScores(std::string saveName, unsigned int maxScores):
	_saveName(std::move(saveName)), _maxScores(maxScores)
{
#ifdef EMSCRIPTEN
	// Check if scores have been saved before naming changes
	emscripten::val localStorage = emscripten::val::global("localStorage");
	const std::string oldName = "/home/emscripten/.local/share/Squarez/scores";
	auto const & file = localStorage.call<emscripten::val>("getItem", oldName);
	if (file.as<bool>())
	{
		localStorage.call<void>("setItem", getDir() + _saveName, file.as<std::string>());
		localStorage.call<void>("removeItem", oldName);
	}
#endif
	// Try to deserialize scores from "file"
	try
	{
		persistent_t f(getDir() + _saveName, std::ios::in);

		if (not f)
			return;

		DeSerializer ser(f);
		ser >> _scores;
		// Make sure scores are correctly sorted
		if (not _scores.empty())
			std::sort(_scores.begin(), _scores.end(),
				[](const Score & left, const Score & right)
				{ return left._score > right._score;});
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

	auto it = std::find_if(_scores.begin(), _scores.end(),
		[score](const Score & other){ return score >= other._score;});

	_scores.insert(it, Score(score, name));
	// If we have reached the maximum number of elements, erase the lowest score
	if(_scores.size() > _maxScores)
	{
		_scores.erase(_scores.begin() + _maxScores, _scores.end());
	}

	this->persist();

	return true;
}

bool squarez::HighScores::mayBeSaved(unsigned int score)
{
	return score > 0 and (_scores.size() < _maxScores or score > _scores.back()._score);
}

void squarez::HighScores::persist()
{
	if (_saveName.empty())
		return;

	try
	{
		mkdir(getDir());
		persistent_t f(getDir() + _saveName, std::ios_base::out | std::ios_base::trunc);
		Serializer ser(f);
		ser << _scores;
	}
	catch (...)
	{
		std::cerr << "Failed to save high scores" << std::endl;
	}
}


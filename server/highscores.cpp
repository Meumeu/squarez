/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2015  Guillaume Meunier <guillaume.meunier@centraliens.net>
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

#include <cstdlib>
#include <ctime>
#include <string>

namespace
{
std::string getenv(const char * variable, std::string const& defaultValue)
{
	const char * value = std::getenv(variable);
	if (value and *value != '\0')
		return value;
	return defaultValue;
}

std::string xdg_data_home()
{
	// FIXME: create the directory if it does not exist
	return getenv("XDG_DATA_HOME", getenv("HOME", "") + "/.local/share") + "/" PACKAGE;
}
}

squarez::HighScores::HighScores(): db(xdg_data_home() + "/scores.db")
{
	initDatabase();
}

squarez::HighScores::HighScores(std::string filename) : db(filename)
{
	initDatabase();
}

void squarez::HighScores::initDatabase()
{
	db.execute("CREATE TABLE IF NOT EXISTS scores ("
		"id INTEGER PRIMARY KEY,"
		"name VARCHAR(100),"
		"score INTEGER,"
		"timestamp INTEGER"
		")"
	);
}

void squarez::HighScores::addScore(std::string playerName, int score)
{
	db.execute("INSERT INTO scores(name, score, timestamp) VALUES(?,?,?)", playerName, score, (uint64_t)std::time(nullptr));
}

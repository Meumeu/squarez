/*
 * Squarez puzzle game
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
	db.execute("CREATE TABLE IF NOT EXISTS config ("
		"key VARCHAR(50) UNIQUE,"
		"value VARCHAR(50)"
		")"
	);

	switch(getConfig("version", 0))
	{
	case 0: // new database
		db.execute("CREATE TABLE scores ("
			"id INTEGER PRIMARY KEY,"
			"name VARCHAR(100),"
			"score INTEGER,"
			"timestamp VARCHAR(20)"
			")"
		);

		setConfig("version", 1);
		break;

	case 1: // current version
		break;

	default: // newer version
		throw std::runtime_error("Database too new");
		break;
	}
}

namespace {
	std::string time_put(std::time_t t = std::time(nullptr))
	{
		std::tm tmp = *std::gmtime(&t);
		char buf[21];
		std::strftime(buf, sizeof(buf), "%F %T", &tmp);
		return buf;
	}
}

int64_t squarez::HighScores::addScore(std::string playerName, int score)
{
	return db.execute("INSERT INTO scores(name, score, timestamp) VALUES(?,?,?)", playerName, score, time_put()).insert_id();
}

void squarez::HighScores::updateScore (int score, int64_t rowId)
{
	db.execute("UPDATE scores SET score = ? where id = ?", score, rowId);
}

std::vector<squarez::onlineSinglePlayer::GetScores::Score> squarez::HighScores::getScores()
{
	std::vector<squarez::onlineSinglePlayer::GetScores::Score> ret;
	for(auto& i: db.execute("SELECT name, score, timestamp FROM scores ORDER BY score DESC LIMIT 20"))
	{
		squarez::onlineSinglePlayer::GetScores::Score tmp;
		tmp._playerName = i.fetch<std::string>(0);
		tmp._score = i.fetch<int>(1);
		tmp._date = i.fetch<std::string>(2);
		ret.push_back(tmp);
	}
	return ret;
}

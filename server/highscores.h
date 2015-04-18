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

#ifndef SQUAREZ_HIGHSCORES_H
#define SQUAREZ_HIGHSCORES_H

#include "database/database.h"
#include "network/methods.h"

#include <string>

namespace squarez {

class HighScores
{
	database db;
	void initDatabase();

public:
	HighScores();
	HighScores(std::string filename);

	template<typename T> T getConfig(const std::string& key, const T& default_value)
	{
		auto res = db.execute("SELECT value FROM config WHERE key=?", key);
		auto it = res.begin();

		if (it == res.end())
			return default_value;
		else
			return it->fetch<T>(0);
	}

	template<typename T> void setConfig(const std::string& key, const T& value)
	{
		db.execute("INSERT OR REPLACE INTO config (key, value) VALUES (?, ?)", key, value);
	}

	int64_t addScore(std::string playerName, int score);
	void updateScore(int score, int64_t rowId);
	std::vector<onlineSinglePlayer::GetScores::Score> getScores(int maxAge = 0, int count = 20);
};

}

#endif // SQUAREZ_HIGHSCORES_H

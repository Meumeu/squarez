/*
 * Squarez puzzle game server binary
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

#include "network/methods.h"

#include <string>
#include <unordered_map>
#include <boost/lexical_cast.hpp>

#include <mysql_driver.h>
#include <cppconn/prepared_statement.h>

namespace squarez {

class HighScores
{
	std::unique_ptr<sql::Connection> db;
	const std::string db_uri, db_username, db_password, db_name;

	void initDatabase();
	void createDatabase();
	void reconnect();

	std::unordered_map<std::string, std::shared_ptr<sql::PreparedStatement>> statementCache;

	std::shared_ptr<sql::PreparedStatement> statement(const std::string& sql);

public:
	HighScores(const std::string& db_uri, const std::string& db_username, const std::string& db_password, const std::string& db_name);

	template<typename T> T getConfig(const std::string& key, const T& default_value)
	{
		auto _getConfigStatement = statement("SELECT value FROM config WHERE name=?");
		_getConfigStatement->setString(1, key);
		std::unique_ptr<sql::ResultSet> res(_getConfigStatement->executeQuery());

		T ret;
		if (res->next())
			ret = boost::lexical_cast<T>(res->getString("value"));
		else
			ret = default_value;

		return ret;
	}

	template<typename T> void setConfig(const std::string& key, const T& value)
	{
		auto _setConfigStatement = statement("INSERT INTO config (name, value) VALUES (?, ?) ON DUPLICATE KEY UPDATE value=?");
		std::string str(boost::lexical_cast<std::string>(value));
		_setConfigStatement->setString(1, key);
		_setConfigStatement->setString(2, str);
		_setConfigStatement->setString(3, str);
		_setConfigStatement->execute();
	}

	int64_t addScore(std::string playerName, int score);
	void updateScore(int score, int64_t rowId);
	std::vector<onlineSinglePlayer::GetScores::Score> getScores(std::time_t min_date, std::time_t max_date, int count);
};

}

#endif // SQUAREZ_HIGHSCORES_H

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

	void bind(std::shared_ptr<sql::PreparedStatement>&, int) {}

	void bind_aux(std::shared_ptr<sql::PreparedStatement>& stmt, int n, int arg)
	{
		stmt->setInt(n, arg);
	}

	void bind_aux(std::shared_ptr<sql::PreparedStatement>& stmt, int n, int64_t arg)
	{
		stmt->setInt64(n, arg);
	}

	void bind_aux(std::shared_ptr<sql::PreparedStatement>& stmt, int n, unsigned int arg)
	{
		stmt->setUInt(n, arg);
	}

	void bind_aux(std::shared_ptr<sql::PreparedStatement>& stmt, int n, uint64_t arg)
	{
		stmt->setUInt64(n, arg);
	}

	void bind_aux(std::shared_ptr<sql::PreparedStatement>& stmt, int n, const std::string& arg)
	{
		stmt->setString(n, arg);
	}

	template<typename... Args, typename T> void bind(std::shared_ptr<sql::PreparedStatement>& stmt, int n, T arg1, Args... args)
	{
		bind_aux(stmt, n, arg1);
		bind(stmt, n + 1, args...);
	}

	template<typename... Args> void executeNoRetry(const std::string& sql, Args... args)
	{
		auto stmt = statement(sql);
		bind(stmt, 1, args...);
		stmt->execute();
	}

	template<typename... Args> void execute(const std::string& sql, Args... args)
	{
		try
		{
			executeNoRetry(sql, args...);
		}
		catch(sql::SQLException& e)
		{
			std::cerr << "SQL exception (" << e.getErrorCode() << ", " << e.getSQLState() << "): " << e.what() << std::endl;
			if (e.getErrorCode() == 2013 || // Lost connection to MySQL server during query
			    e.getErrorCode() == 2006)   // MySQL server has gone away
			{
				reconnect();
				executeNoRetry(sql, args...);
			}
			else
				throw;
		}
	}

	template<typename... Args> std::unique_ptr<sql::ResultSet> executeQueryNoRetry(const std::string& sql, Args... args)
	{
		auto stmt = statement(sql);
		bind(stmt, 1, args...);
		return std::unique_ptr<sql::ResultSet>(stmt->executeQuery());
	}

	template<typename... Args> std::unique_ptr<sql::ResultSet> executeQuery(const std::string& sql, Args... args)
	{
		try
		{
			return executeQueryNoRetry(sql, args...);
		}
		catch(sql::SQLException& e)
		{
			std::cerr << "SQL exception (" << e.getErrorCode() << ", " << e.getSQLState() << "): " << e.what() << std::endl;
			if (e.getErrorCode() == 2013 || // Lost connection to MySQL server during query
			    e.getErrorCode() == 2006)   // MySQL server has gone away
			{
				reconnect();
				return executeQueryNoRetry(sql, args...);
			}
			else
				throw;
		}
	}

public:
	HighScores(const std::string& db_uri, const std::string& db_username, const std::string& db_password, const std::string& db_name);

	template<typename T> T getConfig(const std::string& key, const T& default_value)
	{
		std::unique_ptr<sql::ResultSet> res(executeQueryNoRetry("SELECT value FROM config WHERE name=?", key));
		T ret;
		if (res->next())
			ret = boost::lexical_cast<T>(res->getString("value"));
		else
			ret = default_value;

		return ret;
	}

	template<typename T> void setConfig(const std::string& key, const T& value)
	{
		std::string str = boost::lexical_cast<std::string>(value);
		executeNoRetry("INSERT INTO config (name, value) VALUES (?, ?) ON DUPLICATE KEY UPDATE value=?", key, str, str);
	}

	int64_t addScore(std::string playerName, int score);
	void updateScore(int score, int64_t rowId);
	std::vector<onlineSinglePlayer::GetScores::Score> getScores(std::time_t min_date, std::time_t max_date, int count);
};

}

#endif // SQUAREZ_HIGHSCORES_H

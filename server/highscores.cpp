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

#include "highscores.h"

#include <cstdlib>
#include <ctime>
#include <string>
#include <cppconn/exception.h>

squarez::HighScores::HighScores(const std::string& db_uri, const std::string& db_username, const std::string& db_password, const std::string& db_name):
db_uri(db_uri), db_username(db_username), db_password(db_password), db_name(db_name)
{
	initDatabase();
	createDatabase();
}

std::shared_ptr<sql::PreparedStatement> squarez::HighScores::statement(const std::string& sql)
{
	auto it = statementCache.find(sql);
	if (it == statementCache.end())
	{
		auto stmt = std::shared_ptr<sql::PreparedStatement>(db->prepareStatement(sql));
		statementCache[sql] = stmt;

		return stmt;
	}
	else
	{
		return it->second;
	}
}

void squarez::HighScores::createDatabase()
{
	std::unique_ptr<sql::Statement> stmt(db->createStatement());

	stmt->execute("CREATE TABLE IF NOT EXISTS config ("
		"name VARCHAR(30) PRIMARY KEY,"
		"value VARCHAR(255)"
		")");

	switch(getConfig("version", 0))
	{
	case 0: // new database
		stmt->execute("CREATE TABLE scores ("
			"id INTEGER AUTO_INCREMENT PRIMARY KEY,"
			"name VARCHAR(255),"
			"score INTEGER,"
			"date VARCHAR(30)"
			")");

		setConfig("version", 1);
		break;

	case 1: // current version
		break;

	default: // newer version
		throw std::runtime_error("Database too new");
		break;
	}
}

void squarez::HighScores::initDatabase()
{
	sql::Driver * driver = sql::mysql::get_driver_instance();
	db.reset(driver->connect(db_uri, db_username, db_password));

	std::unique_ptr<sql::Statement> stmt(db->createStatement());
	stmt->execute("USE " + db_name); // FIXME: SQL injection
}

void squarez::HighScores::reconnect()
{
	statementCache.clear();
	initDatabase();

	std::cerr << "Reconnected to database" << std::endl;
}

namespace {
	std::string time_put(std::time_t t = std::time(nullptr))
	{
		std::tm tmp = *std::gmtime(&t);
		char buf[30];
		std::strftime(buf, sizeof(buf), "%FT%TZ", &tmp);
		return buf;
	}
}

int64_t squarez::HighScores::addScore(std::string playerName, int score)
{
	std::unique_lock<std::mutex> _(lock);

	execute("INSERT INTO scores(name, score, date) VALUES(?,?,?)", playerName, score, time_put());

	auto res(executeQuery("SELECT @@identity AS id"));
	if (res->next())
			return res->getInt("id");
		else
			throw std::runtime_error("No insert id");
}

void squarez::HighScores::updateScore (int score, int64_t rowId)
{
	std::unique_lock<std::mutex> _(lock);

	execute("UPDATE scores SET score = ? where id = ?", score, rowId);
}

std::vector<squarez::onlineSinglePlayer::GetScores::Score> squarez::HighScores::getScores(std::time_t min_date, std::time_t max_date, int count)
{
	std::unique_lock<std::mutex> _(lock);

	auto res(executeQuery("SELECT name, score, date FROM scores WHERE date >= ? AND date < ? ORDER BY score DESC LIMIT ?", time_put(min_date), time_put(max_date), count));

	std::vector<squarez::onlineSinglePlayer::GetScores::Score> ret;
	while(res->next())
	{
		squarez::onlineSinglePlayer::GetScores::Score tmp;
		tmp._playerName = res->getString("name");
		tmp._score = res->getInt("score");
		tmp._date = res->getString("date");
		ret.push_back(tmp);
	}

	return ret;
}

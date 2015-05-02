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

squarez::HighScores::HighScores(std::unique_ptr<sql::Connection>&& db): db(std::move(db))
{
	initDatabase();
}


void squarez::HighScores::initDatabase()
{
	_getConfigStatement.reset(db->prepareStatement("SELECT value FROM config WHERE name=?"));
	_setConfigStatement.reset(db->prepareStatement("INSERT INTO config (name, value) VALUES (?, ?) ON DUPLICATE KEY UPDATE value=?"));
	_addScoreStatement.reset(db->prepareStatement("INSERT INTO scores(name, score, timestamp) VALUES(?,?,?)"));
	_updateScoreStatement.reset(db->prepareStatement("UPDATE scores SET score = ? where id = ?"));
	_getScoreStatement.reset(db->prepareStatement("SELECT name, score, timestamp FROM scores WHERE timestamp >= ?  AND timestamp < ? ORDER BY score DESC LIMIT ?"));
	_lastInsertIdStatement.reset(db->prepareStatement("SELECT @@identity AS id"));

	std::unique_ptr<sql::Statement> stmt(db->createStatement());

	stmt->execute("CREATE TABLE IF NOT EXISTS config ("
		"name VARCHAR(30) PRIMARY KEY,"
		"value VARCHAR(255)"
		")");

	switch(getConfig("version", 0))
	{
	case 0: // new database
		stmt->execute("CREATE TABLE scores ("
			"id INTEGER PRIMARY KEY,"
			"name VARCHAR(255),"
			"score INTEGER,"
			"timestamp VARCHAR(30)"
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
	_addScoreStatement->setString(1, playerName);
	_addScoreStatement->setInt(2, score);
	_addScoreStatement->setString(3, time_put());
	_addScoreStatement->execute();

	std::unique_ptr<sql::ResultSet> res(_lastInsertIdStatement->executeQuery());
	if (res->next())
		return res->getInt("id");
	else
		throw std::runtime_error("No insert id");
}

void squarez::HighScores::updateScore (int score, int64_t rowId)
{
	_updateScoreStatement->setInt(1, score);
	_updateScoreStatement->setInt(2, rowId);
	_updateScoreStatement->execute();
}

std::vector<squarez::onlineSinglePlayer::GetScores::Score> squarez::HighScores::getScores(std::time_t min_date, std::time_t max_date, int count)
{
	std::vector<squarez::onlineSinglePlayer::GetScores::Score> ret;

	_getScoreStatement->setString(1, time_put(min_date));
	_getScoreStatement->setString(1, time_put(max_date));
	_getScoreStatement->setInt(1, count);
	std::unique_ptr<sql::ResultSet> res(_getScoreStatement->executeQuery());

	while(res->next())
	{
		squarez::onlineSinglePlayer::GetScores::Score tmp;
		tmp._playerName = res->getString("name");
		tmp._score = res->getInt("score");
		tmp._date = res->getString("timestamp");
	}

	return ret;
}

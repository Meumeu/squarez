/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2012 Guillaume Meunier

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "database.h"
#include <stdexcept>
#include <sstream>
#include <iostream>
#include <chrono>
#include <thread>
#include <boost/lexical_cast.hpp>

int database::check_sqlite_return_code(std::string msg, std::shared_ptr<sqlite3> db, int return_code)
{
	if (return_code == SQLITE_OK || return_code == SQLITE_ROW || return_code == SQLITE_DONE)
		return return_code;
	
	std::stringstream ss;
	ss << msg << std::endl << sqlite3_errmsg(db.get());
	throw std::runtime_error(ss.str());
}

database::database(const std::string& filename) : _db()
{
	int rc;
	sqlite3 * tmp = nullptr;

	rc = sqlite3_open_v2(filename.c_str(), &tmp, SQLITE_OPEN_READWRITE, NULL);

	if (rc == SQLITE_CANTOPEN)
	{
		check_sqlite_return_code("Cannot create database", _db, sqlite3_open_v2(filename.c_str(), &tmp, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL));
	}
	else if (rc != SQLITE_OK)
	{
		check_sqlite_return_code("Cannot open database", _db, rc);
	}

	_db = std::shared_ptr<sqlite3>(tmp, [](sqlite3 * db){ sqlite3_close(db); });
}

database::result::result(std::shared_ptr<sqlite3> db, const std::string& sql) : _db(db), _statement(), begin_called(false)
{
	sqlite3_stmt * tmp = nullptr;
	check_sqlite_return_code("prepare \"" + sql + "\"", _db, sqlite3_prepare_v2(db.get(), sql.c_str(), -1, &tmp, nullptr));
	_statement = std::shared_ptr<sqlite3_stmt>(tmp, [](sqlite3_stmt * stmt){ sqlite3_finalize(stmt); });
}

database::row::row(std::shared_ptr<sqlite3> db, std::shared_ptr<sqlite3_stmt> stmt): _db(db), _statement(stmt)
{
}

database::row::row()
{
}

bool database::row::step()
{
	int rc = sqlite3_step(_statement.get());

	while(rc == SQLITE_BUSY)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		rc = sqlite3_step(_statement.get());
	}

	check_sqlite_return_code("step", _db, rc);

	return rc == SQLITE_ROW;
}


bool database::result::step()
{
	int rc = sqlite3_step(_statement.get());

	while(rc == SQLITE_BUSY)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		rc = sqlite3_step(_statement.get());
	}

	check_sqlite_return_code("step", _db, rc);

	return rc == SQLITE_ROW;
}

database::iterator& database::iterator::operator++()
{
	_finished = not current_row.step();

	return *this;
}

database::iterator database::result::begin()
{
	if (begin_called)
		throw std::runtime_error("database::result::begin() can only be called once");

	begin_called = true;
	if (has_result)
		return database::iterator(_db, _statement);
	else
		return end();
}

database::iterator database::result::end()
{
	return database::iterator();
}

database::iterator::iterator() : _finished(true)
{
}

database::iterator::iterator(std::shared_ptr<sqlite3> db, std::shared_ptr<sqlite3_stmt> stmt) : _finished(false), current_row(db, stmt)
{
}

database::row& database::iterator::operator*()
{
	return current_row;
}

void database::result::bind(int n, int value)
{
	check_sqlite_return_code("bind_int", _db, sqlite3_bind_int(_statement.get(), n, value));
}

void database::result::bind(int n, uint64_t value)
{
	check_sqlite_return_code("bind_int64", _db, sqlite3_bind_int64(_statement.get(), n, value));
}

void database::result::bind(int n, int64_t value)
{
	check_sqlite_return_code("bind_int64", _db, sqlite3_bind_int64(_statement.get(), n, value));
}

void database::result::bind(int n, const std::string& value)
{
	check_sqlite_return_code("bind_text", _db, sqlite3_bind_text(_statement.get(), n, value.c_str(), -1, SQLITE_TRANSIENT));
}

void database::result::bind(int n, const char* value)
{
	check_sqlite_return_code("bind_text", _db, sqlite3_bind_text(_statement.get(), n, value, -1, SQLITE_TRANSIENT));
}

void database::result::bind(int n, double value)
{
	check_sqlite_return_code("bind_double", _db, sqlite3_bind_double(_statement.get(), n, value));
}

template<> int database::row::fetch(int n)
{
	return sqlite3_column_int(_statement.get(), n);
}

template<> int64_t database::row::fetch(int n)
{
	return sqlite3_column_int64(_statement.get(), n);
}

template<> uint64_t database::row::fetch(int n)
{
	return sqlite3_column_int64(_statement.get(), n);
}

template<> double database::row::fetch(int n)
{
	return sqlite3_column_double(_statement.get(), n);
}

template<> float database::row::fetch(int n)
{
	return sqlite3_column_double(_statement.get(), n);
}

template<> std::string database::row::fetch(int n)
{
	const char * tmp = (const char *)sqlite3_column_text(_statement.get(), n);
	return std::string(tmp ? tmp : "");
}

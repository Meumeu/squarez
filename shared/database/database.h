/*
 * Squarez puzzle game
 * Copyright (C) 2012-2015  Guillaume Meunier <guillaume.meunier@centraliens.net>
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

#ifndef DATABASE_H
#define DATABASE_H

#include <memory>
#include <string>
#include <sqlite3.h>

namespace squarez
{

class database
{
	database& operator=(const database &) = delete;
	database(const database&) = delete;

	static int check_sqlite_return_code(std::string msg, std::shared_ptr<sqlite3> db, int return_code);

	std::shared_ptr<sqlite3> _db;

public:
	class iterator;
	class result;
	
	class exception : public std::exception
	{
		std::string _msg;
	public:
		exception(const std::string& msg);
		const char * what() const noexcept override;
	};

	class row
	{
		friend class iterator;
		row();
		row(std::shared_ptr<sqlite3> db, std::shared_ptr<sqlite3_stmt> stmt);
		row(const row& rhs) = delete;
		row& operator=(const row& rhs) = delete;

		std::shared_ptr<sqlite3> _db;
		std::shared_ptr<sqlite3_stmt> _statement;
		bool step();

	public:
		row(row&& rhs) = default;
		row& operator=(row&& rhs) = default;
		template<typename T> T fetch(int n);
	};

	class iterator
	{
		friend class result;
		bool _finished;
		row current_row;

		iterator();
		iterator(std::shared_ptr<sqlite3> db, std::shared_ptr<sqlite3_stmt> stmt);
		iterator(const iterator& rhs) = delete;
		iterator& operator=(const iterator& rhs) = delete;

	public:
		iterator(iterator&& rhs) = default;
		iterator& operator=(iterator&& rhs) = default;

		bool operator==(const iterator& rhs)
		{
			return (current_row._statement == rhs.current_row._statement) || (_finished && rhs._finished);
		}
		bool operator!=(const iterator& rhs)
		{
			return not (*this == rhs);
		}
		row& operator*();
		iterator& operator++(); // prefix
	};

	class result
	{
		friend class database;
		std::shared_ptr<sqlite3> _db;
		std::shared_ptr<sqlite3_stmt> _statement;
		bool begin_called;
		bool has_result;
		int64_t _insert_id;

		result(std::shared_ptr<sqlite3> db, const std::string & sql);
		result(const result& rhs) = delete;
		result operator=(const result& rhs) = delete;

		void bind(int n, int value);
		void bind(int n, unsigned int value);
		void bind(int n, int64_t value);
		void bind(int n, uint64_t value);
		void bind(int n, const std::string& value);
		void bind(int n, const char * value);
		void bind(int n, double value);
		bool step();

	public:
		result(result&& rhs) = default;
		result& operator=(result&& rhs) = default;

		template<typename T> T fetch(int n);

		template<int n = 1> void execute()
		{
			has_result = step();
			_insert_id = sqlite3_last_insert_rowid(_db.get());
		}
		template<int n = 1, typename Arg1, typename... Args> void execute(const Arg1& arg1, const Args&... args)
		{
			bind(n, arg1);
			execute<n+1>(args...);
		}

		iterator begin();
		iterator end();
		int64_t insert_id() { return _insert_id; }
	};
	
	template<typename... Args> result execute(const std::string& request, const Args&... args)
	{
		result res(_db, request);
		res.execute(args...);
		
		return res;
	}

	database(const std::string & filename);
};
}

#endif // DATABASE_H


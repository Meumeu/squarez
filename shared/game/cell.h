/*
 * Squarez puzzle game
 * Copyright (C) 2015  Guillaume Meunier <guillaume.meunier@centraliens.net>
 * Copyright (C) 2015  Patrick Nicolas <patricknicolas@laposte.net>
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

#ifndef CELL_H
#define CELL_H

#include <memory>
#include <functional>

namespace squarez
{
class Rules;

class Cell: public std::enable_shared_from_this<Cell>
{
	public:
		struct Compare
		{
			bool operator() (const Cell & left, const Cell & right) const;
			bool operator() (const std::shared_ptr<const Cell> left, const std::shared_ptr<const Cell> right) const { return (*this)(*left, *right); }
		};
		class Proxy
		{
			public:
				virtual ~Proxy();
				Proxy(Cell & owner): _owner(owner) {}
				Cell & _owner;

				virtual void moved(int /*x*/, int /*y*/) {}
				virtual void selectChanged(bool /*status*/) {}
		};
		typedef std::function<std::unique_ptr<Proxy>(Cell &)> proxy_factory;

		Cell(unsigned int symbol, int x, int y, Rules & rules, proxy_factory factory):
			symbol(symbol), _x(x), _y(y), _selected(false), _rules(rules), _proxy(factory(*this)) {}

		Cell(Cell &&) = delete;

		void move(int x, int y);

		void click();
		void setSelected(bool status);
		bool selected() const { return _selected;}
		int x() const { return _x;}
		int y() const { return _y;}

		const unsigned int symbol;
	private:

		int _x;
		int _y;
		bool _selected;
		Rules & _rules;
		std::unique_ptr<Proxy> _proxy;
};
}

#endif

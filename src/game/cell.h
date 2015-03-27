/*
 * Squarez puzzle game
 * Copyright (C) 2015  Patrick Niyas <patrickniyas@laposte.net>
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

class Cell
{
	public:
		class Proxy
		{
			public:
				virtual ~Proxy() = default;
				Proxy(Cell & owner): _owner(owner) {}
				Cell & _owner;

				virtual void moved(unsigned int /*x*/, unsigned int /*y*/) {}
				virtual void selectChanged(bool /*status*/) {}
		};
		typedef std::function<std::unique_ptr<Proxy>(Cell &)> proxy_factory;

		Cell(unsigned int symbol, unsigned int x, unsigned int y, Rules & rules, proxy_factory factory):
			_symbol(symbol), _x(x), _y(y), _selected(false), _rules(rules), _proxy(factory(*this)) {}

		Cell(Cell &&) = delete;

		void move(unsigned int x, unsigned int y);

		void click();
		void setSelected(bool status);
		bool selected() const { return _selected;}
		unsigned int x() const { return _x;}
		unsigned int y() const { return _y;}

		const unsigned int _symbol;
	private:

		unsigned int _x;
		unsigned int _y;
		bool _selected;
		Rules & _rules;
		std::unique_ptr<Proxy> _proxy;
};
}

#endif

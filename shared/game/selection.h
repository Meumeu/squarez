/*
 * Squarez puzzle game
 * Copyright (C) 2013-2015  Guillaume Meunier <guillaume.meunier@centraliens.net>
 * Copyright (C) 2013-2015  Patrick Nicolas <patricknicolas@laposte.net>
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

#ifndef SELECTION_H
#define SELECTION_H

#include <memory>
#include <set>
#include <utility>

#include <game/cell.h>

namespace squarez
{

class GameBoard;

class DeSerializer;
class Serializer;

class Selection
{
	friend Serializer & operator<<(Serializer & out, Selection const& selection);
public:

	enum class State
	{
		invalid,
		validated
	};
	typedef std::set<std::shared_ptr<Cell>, Cell::Compare> storage_t;
	typedef storage_t::iterator iterator;
	typedef storage_t::const_iterator const_iterator;

	Selection() {}
	Selection(DeSerializer& serialized, const GameBoard & board);

	virtual bool togglePoint(std::shared_ptr<Cell> cell);
	virtual void setState(State state);

	bool isValid() const { return _points.size() == 4; }
	State state() const { return _state; }

	iterator begin() { return _points.begin(); }
	iterator end() { return _points.end(); }
	const_iterator cbegin() const { return _points.cbegin(); }
	const_iterator cend() const { return _points.cend(); }
	const_iterator begin() const { return _points.cbegin(); }
	const_iterator end() const { return _points.cend(); }
	
private:
	storage_t _points;
	State _state;
};

class VisibleSelection : public Selection
{
public:
	class Proxy
	{
	public:
		virtual ~Proxy() {}
		Proxy(VisibleSelection & owner): _owner(owner) {}
		VisibleSelection & _owner;
		virtual void changed() {}
		virtual void stateChanged(State /*state*/) {}
	};
	typedef std::function<std::unique_ptr<Proxy>(VisibleSelection &)> proxy_factory;

	VisibleSelection(proxy_factory factory): _proxy(factory(*this)) {}
	VisibleSelection(proxy_factory factory, DeSerializer& serialized, const GameBoard & board): Selection(serialized, board), _proxy(factory(*this)) { _proxy->changed();}

	virtual bool togglePoint(std::shared_ptr<Cell>) override;
	virtual void setState(State state) override;

private:
	std::unique_ptr<Proxy> _proxy;
};

Serializer & operator<<(Serializer & out, Selection const& selection);

}

#endif // SELECTION_H

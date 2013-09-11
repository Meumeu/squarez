/*
 * Squarez puzzle game server binary
 * Copyright (C) 2013  Patrick Nicolas <patricknicolas@laposte.net>
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

#ifndef SQUAREZ_GAMESTATUS_H
#define SQUAREZ_GAMESTATUS_H

#include <boost/noncopyable.hpp>
#include <shared/gameboard.h>

#include <memory>
#include <stdexcept>

namespace squarez {

class GameStatus : public boost::noncopyable
{
public:
	GameStatus(GameBoard const& board) : _board(board) {_instance = this;}

	static GameStatus& access() {
		if (_instance)
			return *_instance;
		throw std::runtime_error("GameStatus has not been initialized");
	}

	GameBoard const& getBoard() const { return _board;}
	GameBoard & accessBoard() { return _board;}

private:
	static GameStatus* _instance;

	GameBoard _board;
};
}

#endif // SQUAREZ_GAMESTATUS_H

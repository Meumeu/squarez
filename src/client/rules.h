/*
 * Squarez puzzle game
 * Copyright (C) 2013  Guillaume Meunier <guillaume.meunier@centraliens.net>
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

#ifndef SQUAREZ_RULES_H
#define SQUAREZ_RULES_H

#include <chrono>
#include "shared/gameboard.h"

namespace squarez {

class UI;
class Timer;

class Rules
{
	friend class UI;
private:
	void setUI(UI * ui);
	
protected:
	GameBoard board;
	UI * ui;
	virtual void timeTick(std::chrono::duration<float> /* t */) { }
	
public:
	virtual void onSelect(Selection const& selection) = 0;
	virtual bool gameOver() const = 0;
	virtual Timer const& getTimer() const = 0;
	GameBoard const& getBoard() const { return board; }
	Rules(int board_size, int nb_symbols);
	Rules(GameBoard const& board);
	
	virtual ~Rules() {}
};
}

#endif // SQUAREZ_RULES_H

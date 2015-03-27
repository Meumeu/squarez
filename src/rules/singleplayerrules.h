/*
 * Squarez puzzle game
 * Copyright (C) 2013  Guillaume Meunier <guillaume.meunier@centraliens.net>
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

#ifndef SQUAREZ_SINGLEPLAYERRULES_H
#define SQUAREZ_SINGLEPLAYERRULES_H

#include "rules.h"
#include "game/selection.h"

namespace squarez {

class HighScores;

class SinglePlayerRules : public squarez::Rules
{
private:
	bool _scoreSaved;
	Selection _selection;

	static HighScores & accessHighScores();
	void saveScore();
	
public:
	SinglePlayerRules(Proxy & proxy, int board_size = 8, int nb_symbols = 3, int long_term = 10, int short_term = 60, int duration = 180);
	
	virtual void onClick(Cell & cell) override;
	virtual void setPlayerName(std::string const& name) override;
	bool gameOver();

	HighScores * getHighScores() {return &accessHighScores();}
	
	void setPause(bool state);
};
}

#endif // SQUAREZ_SINGLEPLAYERRULES_H

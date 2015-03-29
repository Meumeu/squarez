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

#ifndef SQUAREZ_SINGLEPLAYERRULES_H
#define SQUAREZ_SINGLEPLAYERRULES_H

#include "rules.h"
#include "game/selection.h"
#include "game/constants.h"

namespace squarez {

class SinglePlayerRules : public squarez::Rules
{
private:
	Selection _selection;
	
public:
	SinglePlayerRules(Proxy & proxy,
		Timer && timer,
		int board_size = constants::default_board_size,
		int nb_symbols = constants::default_symbols);
	
	virtual void onClick(Cell & cell) override;
	virtual void resetSelection() override;
	virtual void setPlayerName(std::string const& name) override;
	bool gameOver();
	
	void setPause(bool state);
};
}

#endif // SQUAREZ_SINGLEPLAYERRULES_H

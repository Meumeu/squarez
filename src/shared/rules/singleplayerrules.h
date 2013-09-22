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

#ifndef SQUAREZ_SINGLEPLAYERRULES_H
#define SQUAREZ_SINGLEPLAYERRULES_H

#include "rules.h"
#include "shared/timer.h"

namespace squarez {

class SinglePlayerRules : public squarez::Rules
{
private:
	Timer timer;
	unsigned int score;
	
public:
	SinglePlayerRules(int board_size, int nb_symbols, int long_term, int short_term, int duration);
	
	virtual void onSelect(Selection const& selection);
	virtual Timer const& getTimer() const;
	virtual bool gameOver() const;
	
	void pause();
	void unpause();
};
}

#endif // SQUAREZ_SINGLEPLAYERRULES_H

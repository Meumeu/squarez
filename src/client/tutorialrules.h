/*
 * Squarez puzzle game
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

#ifndef SQUAREZ_TUTORIAL_RULES_H
#define SQUAREZ_TUTORIAL_RULES_H

#include "rules.h"

#include "shared/selection.h"
#include "shared/timer.h"

namespace squarez
{

class TutorialRules : public Rules
{
private:
	Selection _selection;
	Timer _timer;
	unsigned int _score;
	unsigned int _step;
public:
	TutorialRules(int board_size, int nb_symbols);
	virtual void onSelect(Selection const& /*selection*/) {};
	virtual Timer const& getTimer() const {return _timer;};
	virtual bool gameOver() {return false;};
	virtual void setPlayerName(std::string const& /*name*/) {};

	//Move to the next hint
	void next();
};

}

#endif

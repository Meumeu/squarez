/*
 * Squarez puzzle game
 * Copyright (C) 2013  Guillaume Meunier <guillaume.meunier@centraliens.net>
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

#ifndef SQUAREZ_UI_H
#define SQUAREZ_UI_H

#include <memory>
#include <vector>

namespace squarez {

class Rules;
class Score;
class Selection;
class Transition;

class UI
{
protected:
	std::shared_ptr<Rules> rules;
	
public:
	virtual void onTransition(Transition const& transition) = 0; // Called before the board is modified
	virtual void onScoreChanged(int new_score) = 0;
	virtual void onScoreListChanged(std::vector<Score> const& scores) = 0;
	virtual void onSelectionAccepted(Selection const& selection) = 0;
	virtual void onSelectionRejected(Selection const& selection) = 0;

	// Called to notify the user interface that rules need the player name
	virtual void nameRequired(std::string const& lastName) = 0 ;

	UI() {} // Only for javascript, do not use
	UI(std::shared_ptr<Rules> rules);
	virtual ~UI();

	void setRules(std::shared_ptr<Rules> rules_);
};
}

#endif // SQUAREZ_UI_H

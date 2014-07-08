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

#ifndef SQUAREZ_PLAYER_H
#define SQUAREZ_PLAYER_H

#include <string>

namespace squarez {

class Player
{
public:
	Player(std::string name);

	std::string const& getName() const { return _name;}

	unsigned int getScore() const { return _score + _roundScore;}

	unsigned int getPreviousScore() const { return _previousScore;}

	void endGame();
	void endRound();
	bool isActive() const {return _active;}
	void setActive() {_active = true;}

	void setRoundScore(unsigned int roundScore);
	unsigned int getRoundScore() const { return _roundScore;}
private:
	std::string const _name;
	// Score during the current game
	unsigned int _score;

	// Total score of previous game
	unsigned int _previousScore;

	// Score of the best transition of the current round
	unsigned int _roundScore;

	bool _active;
};
}

#endif // SQUAREZ_PLAYER_H

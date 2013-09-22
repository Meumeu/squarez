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

#include "player.h"

namespace squarez
{
Player::Player(std::string name): _name(name), _score(0), _previousScore(0), _roundScore(0), _active(true)
{

}

void Player::endGame()
{
	_active = false;
	_previousScore = _score + _roundScore;
	_score = 0;
	_roundScore = 0;
}

void Player::endRound()
{
	_score += _roundScore;
	_roundScore = 0;
}

void Player::setRoundScore(unsigned int roundScore)
{
	if (roundScore > _roundScore)
		_roundScore = roundScore;
}

}
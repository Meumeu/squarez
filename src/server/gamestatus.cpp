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

#include "gamestatus.h"

namespace squarez
{
GameStatus* GameStatus::_instance = nullptr;

GameStatus::GameStatus(const GameBoard& board, std::chrono::seconds roundDuration): _board(board), _round(0), _roundDuration(roundDuration) {
	if (_instance)
		throw std::runtime_error("GameStatus already initialized");
	_instance = this;
}

uint16_t GameStatus::pushSelection(const Selection& selection)
{
	auto const& transition = _board.selectSquare(selection);

	// If The selection gives a better score than the stored one, keep it
	if (transition._score > _bestTransition._score)
	{
		_bestTransition = transition;
	}

	return transition._score;
}

GameStatus & GameStatus::instance()
{
	if (_instance)
		return *_instance;
	throw std::runtime_error("GameStatus has not been initialized");
}

ROGameStatus::ROGameStatus(): _gameStatus(GameStatus::instance()), _readLock(_gameStatus._readMutex) {}
RWGameStatus::RWGameStatus(): ROGameStatus(), _writeLock(_gameStatus._writeMutex) {}

}

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

#include <fastcgi++/message.hpp>

namespace squarez
{
GameStatus* GameStatus::_instance = nullptr;

GameStatus::GameStatus(const GameBoard& board, std::chrono::seconds roundDuration): _running(true), _board(board), _round(0), _roundDuration(roundDuration) {
	if (_instance)
		throw std::runtime_error("GameStatus already initialized");
	_instance = this;

	// Start the main loop in a separate thread
	_mainLoop = std::thread(&GameStatus::run, this);
}

GameStatus::~GameStatus()
{
	_running = false;
	_mainLoop.join();
}

uint16_t GameStatus::pushSelection(const Selection& selection)
{
	auto const& transition = _board.selectSquare(selection, true);

	// If the selection gives a better score than the stored one, keep it
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

void GameStatus::run()
{
	auto nextRound = std::chrono::steady_clock::now() + _roundDuration;
	while (_running)
	{
		{
			std::unique_lock<std::recursive_mutex> lock(_mutex);

			// If no transition has been found, select a random one
			if (_bestTransition._selection.getPoints().empty())
			{
				auto const & transitions = _board.findTransitions();
				_bestTransition = transitions[std::rand() % transitions.size()];
			}

			// Make sure the transition can not end the game
			_lastRoundTransition = _board.selectSquare(_bestTransition._selection, false);
			_bestTransition = Transition();

			_board.applyTransition(_lastRoundTransition);

			// Wake all waiting threads
			for (auto const& callback: _pending)
			{
				callback(Fastcgipp::Message(1));
			}

			_pending.clear();
			++_round;
			nextRound += _roundDuration;
		}
		std::this_thread::sleep_until(nextRound);
	}
}


ROGameStatus::ROGameStatus(): _gameStatus(GameStatus::instance()), _lock(_gameStatus._mutex) {}
RWGameStatus::RWGameStatus(): ROGameStatus() {}

}

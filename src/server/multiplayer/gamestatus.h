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

#ifndef SQUAREZ_GAMESTATUS_H
#define SQUAREZ_GAMESTATUS_H

#include <boost/noncopyable.hpp>
#include <boost/function.hpp>

#include "shared/gameboard.h"

#include "player.h"

#include <memory>
#include <stdexcept>
#include <chrono>
#include <mutex>
#include <thread>
#include <atomic>
#include <map>
#include <random>

namespace Fastcgipp {
struct Message;}

namespace squarez {

class GameStatus;

// Read-only thread-safe accessor for GameStatus
class ROGameStatus
{
public:
	ROGameStatus();
	GameStatus const& operator()() const { return _gameStatus;}
protected:
	GameStatus & _gameStatus;
private:
	std::unique_lock<std::recursive_mutex> const _lock;
};

// Read-write thread-safe accessor for GameStatus
class RWGameStatus : public ROGameStatus
{
public:
	RWGameStatus();
	GameStatus & operator()() { return _gameStatus;}
};

// Singleton-like class: can be initialized only once, and accessed anywhere using the RW/RO versions
class GameStatus : public boost::noncopyable
{
	friend class ROGameStatus;
	friend class RWGameStatus;
public:
	GameStatus(unsigned int size, unsigned int numSymbols, std::chrono::seconds roundDuration, unsigned int roundsPerGame);
	~GameStatus();

	GameBoard const& getBoard() const { return _board;}
	GameBoard & accessBoard() { return _board;}

	// Test a selection, returns the updated score for the player
	// And stores the transition if it is better
	uint16_t pushSelection(Selection const& selection, unsigned int token);

	// Wait for a transition to be available
	void registerWaitTransition(boost::function<void(Fastcgipp::Message)> const& callback, unsigned int token);
	// Wait for a change in score list
	void registerWaitScore(boost::function<void(Fastcgipp::Message)> const& callback);

	Transition const& getLastRoundTransition() const { return _lastRoundTransition;}

	// Current progress in the round (0: no timeleft, 1: _roundDuration time left)
	float getRoundTimeAdvancement() const;
	std::chrono::seconds getRoundDuration() const { return _roundDuration; }

	unsigned int getRound() const { return _round;}

	// Insert the provided player and return the associated token (Player is copied)
	unsigned int registerPlayer(Player const& player);

	Player const& getPlayer(unsigned int token) const;

	std::vector<std::reference_wrapper<const Player>> getPlayersByScore() const;

	const unsigned int _roundsPerGame;
private:
	std::mt19937 _random_generator;
	// Game main loop
	void run();
	std::thread _mainLoop;
	std::atomic_bool _running;

	static GameStatus& instance();
	static GameStatus* _instance;
	std::recursive_mutex _mutex;

	// Current game board
	GameBoard _board;

	// Winning transition from last round
	Transition _lastRoundTransition;

	// Best transition for the current round
	Transition _bestTransition;

	// Identifier of the round
	unsigned int _round;

	// Duration of a round
	std::chrono::seconds _roundDuration;

	// Request waiting for a new round
	std::vector<boost::function<void(Fastcgipp::Message)>> _pendingTransition;
	// Request waiting for a change on score list
	std::vector<boost::function<void(Fastcgipp::Message)>> _pendingScoreList;

	std::chrono::steady_clock::time_point _nextRound;

	std::map<unsigned int, Player> _players;
};

}
#endif // SQUAREZ_GAMESTATUS_H

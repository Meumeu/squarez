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

GameStatus::GameStatus(const GameBoard& board, std::chrono::seconds roundDuration, unsigned int roundsPerGame):
	_roundsPerGame(roundsPerGame), _running(true), _board(board), _round(0), _roundDuration(roundDuration)
{
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

uint16_t GameStatus::pushSelection(const Selection& selection, unsigned int token)
{
	auto const& transition = _board.selectSquare(selection, true);

	// If the selection gives a better score than the stored one, keep it
	if (transition._score > _bestTransition._score)
	{
		_bestTransition = transition;
	}

	// Add the score to the player
	auto & player = _players.at(token);
	player.setRoundScore(transition._score);

	return player.getScore();
}

GameStatus & GameStatus::instance()
{
	if (_instance)
		return *_instance;
	throw std::runtime_error("GameStatus has not been initialized");
}

void GameStatus::run()
{
	_nextRound = std::chrono::steady_clock::now() + _roundDuration;
	while (_running)
	{
		{
			std::unique_lock<std::recursive_mutex> lock(_mutex);

			// Adjust player scores
			for (auto & player: _players)
			{
				player.second.endRound();
			}

			// We have just finished the last round of the game
			if (_round == _roundsPerGame - 1)
			{
				// Player cleanup: remove those who have been inactive, reset scores
				for (auto it = _players.begin(); it != _players.end();)
				{
					if (not it->second.isActive())
						it = _players.erase(it);
					else
					{
						it->second.endGame();
						++it;
					}
				}

				// Shuffle the board
				_lastRoundTransition = Transition(_board.size());
			}
			else
			{
				// If no transition has been found, select a random one
				if (_bestTransition._selection.getPoints().empty())
				{
					auto const & transitions = _board.findTransitions();
					_bestTransition = transitions[std::rand() % transitions.size()];
				}

				// Make sure the transition can not end the game
				_lastRoundTransition = _board.selectSquare(_bestTransition._selection, false);
			}

			_bestTransition = Transition();

			_board.applyTransition(_lastRoundTransition);

			// Wake all waiting threads
			for (auto const& callback: _pending)
			{
				callback(Fastcgipp::Message(1));
			}

			_pending.clear();
			_round = (_round + 1)% _roundsPerGame;
			_nextRound += _roundDuration;
		}
		std::this_thread::sleep_until(_nextRound);
	}
}

float GameStatus::getRoundTimeAdvancement() const
{
	return std::chrono::duration<float>(_nextRound - std::chrono::steady_clock::now())/std::chrono::duration<float>(_roundDuration);
}

unsigned int GameStatus::registerPlayer(Player const& player)
{
	if (_players.empty())
	{
		_players.insert(std::make_pair(0, player));
		return 0;
	}

	unsigned int const pos = _players.rbegin()->first + 1;
	_players.insert(_players.end(), std::make_pair(pos, player));
	return pos;
}

Player const& GameStatus::getPlayer(unsigned int token) const
{
	return _players.at(token);
}

std::vector<std::reference_wrapper<const Player>> GameStatus::getPlayersByScore() const
{
	std::vector<std::reference_wrapper<const Player>> res;
	res.reserve(_players.size());
	for (auto const& p : _players)
	{
		res.push_back(p.second);
	}

	std::sort(res.begin(), res.end(), [](std::reference_wrapper<const Player> left, std::reference_wrapper<const Player> right)
	{
		return left.get().getScore() > right.get().getScore();
	});

	return res;
}

void GameStatus::registerWait(boost::function<void(Fastcgipp::Message)> const& callback, unsigned int token)
{
	_players.at(token).setActive();
	_pending.push_back(callback);
}

ROGameStatus::ROGameStatus(): _gameStatus(GameStatus::instance()), _lock(_gameStatus._mutex) {}
RWGameStatus::RWGameStatus(): ROGameStatus() {}

}

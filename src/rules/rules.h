/*
 * Squarez puzzle game
 * Copyright (C) 2013  Guillaume Meunier <guillaume.meunier@centraliens.net>
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

#ifndef SQUAREZ_RULES_H
#define SQUAREZ_RULES_H

#include <chrono>
#include <memory>
#include <random>
#include <string>
#include "timer.h"
#include "game/gameboard.h"

namespace squarez {
class Cell;
class Score;
class Selection;
class Transition;
}

namespace squarez {
class Rules
{
public:
	class Proxy
	{
	public:
		virtual ~Proxy() = default;
		virtual void scoreChanged(unsigned int score) = 0;
		virtual void gameOver(bool status) = 0;
		virtual void timerUpdated() = 0;
		virtual void nameRequired() = 0;
		virtual void animateSquare(std::array<Cell *, 4>) = 0;

		virtual std::unique_ptr<Cell::Proxy> cellProxyFactory(Cell & cell) = 0;
	};
private:
	unsigned int _score;
	bool _gameOver;
	Timer _timer;

protected:
	Proxy & _proxy;
	std::mt19937 _random_generator;
	std::unique_ptr<GameBoard> _board;
	std::string _playerName;
	void setScore(unsigned int score);
	void setGameOver(bool status);
	void applyTransition(const Transition & transition);

	void pauseTimer(bool pause);
	void refillTimer(unsigned int percentage);
	void setTimer(Timer && timer);

	Rules(Proxy & proxy, int board_size, int nb_symbols, std::uint_fast32_t random_seed, Timer timer, std::string name = "");
	Rules(Proxy & proxy, std::unique_ptr<GameBoard> && _board, std::uint_fast32_t random_seed, Timer timer, std::string name = "");

public:
	bool pause() const {return _timer.paused();}
	float getPercentageLeft(float offset = 0);
	int msLeft() { return _timer.msLeft();}
	virtual bool gameOver() {return _gameOver;}
	virtual void onClick(Cell & cell) = 0;
	unsigned int getScore() const {return _score;}
	virtual void setPlayerName(std::string const& name) = 0;
	Cell::proxy_factory getCellProxyFactory() { return [this](Cell & cell){return _proxy.cellProxyFactory(cell);};}
	
	virtual ~Rules() {}
};
}

#endif // SQUAREZ_RULES_H

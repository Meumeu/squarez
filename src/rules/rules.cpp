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

#include "rules.h"
#include "game/selection.h"

#include <algorithm>

squarez::Rules::Rules(Proxy & proxy, int board_size, int nb_symbols, std::uint_fast32_t random_seed, Timer timer, std::string name):
	_score(0), _timer(timer), _proxy(proxy), _random_generator(random_seed), _board(new squarez::GameBoard(board_size, nb_symbols, _random_generator, *this)), _playerName(name)
{
}

squarez::Rules::Rules(Proxy & proxy, std::unique_ptr<GameBoard> &&board, std::uint_fast32_t random_seed, Timer timer, std::string name):
	_score(0), _timer(timer),_proxy(proxy), _random_generator(random_seed), _board(std::move(board)), _playerName(name)
{
}

void squarez::Rules::setScore(unsigned int score)
{
	if (score == _score)
		return;
	_score = score;
	_proxy.scoreChanged(_score);
}

void squarez::Rules::setGameOver(bool status)
{
	if (status == _gameOver)
		return;
	_gameOver = status;
	_proxy.gameOver(_gameOver);
}

void squarez::Rules::applyTransition(const Transition &transition)
{
	if (transition._selection.isValid())
	{
		std::array<Cell *, 4> square;
		const auto & points = transition._selection.getPoints();
		std::transform(points.begin(), points.end(), square.begin(),
			[this](decltype(*points.begin()) point){return &_board->access(point);}
		);

		_proxy.animateSquare(square);
	}
	_board->applyTransition(transition);
}

float squarez::Rules::getPercentageLeft(float offset)
{
	return _timer.percentageLeft(offset);
}

void squarez::Rules::pauseTimer(bool pause)
{
	if (pause != _timer.paused())
	{
		_timer.setPause(pause);
		_proxy.timerUpdated();
	}
}

void squarez::Rules::refillTimer(unsigned int percentage)
{
	if (percentage == 0)
		return;
	_timer.refill(percentage);
	_proxy.timerUpdated();
}

void squarez::Rules::setTimer(squarez::Timer&& timer)
{
	_timer = timer;
	_proxy.timerUpdated();
}

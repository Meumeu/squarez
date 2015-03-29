/*
 * Squarez puzzle game
 * Copyright (C) 2015  Patrick Nicolas <patricknicolas@laposte.net>
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

#include "serverrules.h"

std::unique_ptr< squarez::Cell::Proxy > squarez::DummyProxy::cellProxyFactory(squarez::Cell& cell)
{
	return std::unique_ptr<squarez::Cell::Proxy>(new squarez::Cell::Proxy(cell));
}

squarez::ServerRules::ServerRules(std::string playerName, std::mt19937::result_type seed, int board_size, int nb_symbols, Timer && timer):
	Rules(*this, board_size, nb_symbols, seed, std::move(timer)),
	_epoch(std::chrono::steady_clock::now())
{
	_playerName = std::move(playerName);
}

bool squarez::ServerRules::playSelection(const squarez::Selection& selection, std::chrono::milliseconds msSinceEpoch)
{
	std::chrono::steady_clock::time_point when = _epoch + msSinceEpoch;
	if (_timer.msLeft(when) < 0)
	{
		setGameOver(true);
		return true;
	}
	auto const & transition = _board->selectSquare(selection, _random_generator);
	if (transition._score)
	{
		_timer.refill(transition._score * 2, when);
		setScore(score() + transition._score);

		this->applyTransition(transition);
	}

	return false;
}

void squarez::ServerRules::setPause(bool state, std::chrono::milliseconds msSinceEpoch)
{
	std::chrono::steady_clock::time_point when = _epoch + msSinceEpoch;
	_timer.setPause(state, when);
}

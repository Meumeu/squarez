/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2015  Guillaume Meunier <guillaume.meunier@centraliens.net>
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

#include "onlinesingleplayerrules.h"
#include "network/methods.h"
#include "utils/serializer.h"
#include "game/constants.h"

squarez::OnlineSinglePlayerRules::~OnlineSinglePlayerRules()
{
}


squarez::OnlineSinglePlayerRules::OnlineSinglePlayerRules(
	squarez::Rules::Proxy& proxy,
	int board_size,
	int nb_symbols,
	std::uint_fast32_t random_seed,
	std::string url,
	std::string name,
	unsigned int token) :
	
	Rules(proxy, board_size, nb_symbols, random_seed, constants::default_timer(), name),
	_url(url),
	_token(token),
	_epoch(std::chrono::steady_clock::now())
{
	
}

bool squarez::OnlineSinglePlayerRules::gameOver()
{
	if (squarez::Rules::gameOver())
		return true;

	if (_timer.msLeft() > 0)
		return false;

	setGameOver(true);

	return true;
}

void squarez::OnlineSinglePlayerRules::onClick(squarez::Cell& cell)
{
	if (gameOver())
		return;
	cell.setSelected(_selection.togglePoint(cell.x(), cell.y()));
	Transition const& tr = _board->selectSquare(_selection, _random_generator, false);
	if (tr._score)
	{
		_requestHandle = http::request(_url + onlineSinglePlayer::PushSelection::encodeRequest(_selection, _token, std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - _epoch)),
			[this](std::string response) // onload
			{
				DeSerializer s(response);
				onlineSinglePlayer::PushSelection reply(s);
				setGameOver(reply._gameOver);
			},
			[]() // onerror
			{
				// FIXME
			}
		);
		
		this->refillTimer(tr._score * 2);
		setScore(score() + tr._score);

		this->applyTransition(tr);
		resetSelection();
	}
}

void squarez::OnlineSinglePlayerRules::resetSelection()
{
	for(auto& i: _selection)
		_board->access(i).setSelected(false);

	_selection = Selection();
}

void squarez::OnlineSinglePlayerRules::setPause(bool state)
{
	if (state == pause())
		return;
	pauseTimer(state);
	
	_requestHandle = http::request(_url + onlineSinglePlayer::Pause::encodeRequest(_token, pause(), std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - _epoch)),
			[](std::string /*response*/) // onload
			{
			},
			[]() // onerror
			{
				// FIXME
			}
		);
}

void squarez::OnlineSinglePlayerRules::setPlayerName(const std::string& /*name*/)
{
}

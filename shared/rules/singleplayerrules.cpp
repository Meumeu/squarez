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

#include "singleplayerrules.h"
#include "network/methods.h"
#include "utils/serializer.h"
#include "game/constants.h"

squarez::SinglePlayerRules::~SinglePlayerRules()
{
}


squarez::SinglePlayerRules::SinglePlayerRules(
	squarez::Rules::Proxy& proxy,
	Timer && timer,
	int board_size,
	int nb_symbols,
	std::string name,
	std::uint_fast32_t random_seed,
	std::string url,
	unsigned int token) :

	Rules(proxy, board_size, nb_symbols, random_seed ? random_seed : std::random_device()(), std::move(timer), name),
	_url(url),
	_token(token),
	_selection(new VisibleSelection(selectionProxyFactory())),
	_epoch(std::chrono::steady_clock::now())
{
	// Silence warning about unused field with NO_HTTP_REQUEST
	(void)_token;
}

bool squarez::SinglePlayerRules::gameOver()
{
	if (squarez::Rules::gameOver())
		return true;

	if (_timer.msLeft() > 0)
		return false;

	setGameOver(true);

	return true;
}

void squarez::SinglePlayerRules::onClick(squarez::Cell& cell)
{
	if (gameOver())
		return;

	cell.setSelected(_selection->togglePoint(cell.x(), cell.y()));
	Transition const& tr = _board->selectSquare(*_selection, _random_generator, false);
	if (tr._score)
	{
#ifndef NO_HTTP_REQUEST
		if (not _url.empty())
			_requestHandle = http::request(_url + onlineSinglePlayer::PushSelection::encodeRequest(*_selection, _token, std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - _epoch)),
				[this](std::string response) // onload
				{
					DeSerializer s(response);
					onlineSinglePlayer::PushSelection reply(s);
					setGameOver(reply._gameOver);
				},
				[this]() // onerror
				{
					_proxy.networkError();
				}
			);
#endif

		this->refillTimer(tr._score * 2);
		setScore(score() + tr._score);

		this->applyTransition(tr);
		resetSelection();
	}
}

void squarez::SinglePlayerRules::resetSelection()
{
	for(auto& i: *_selection)
		_board->access(i).setSelected(false);

	_selection.reset(new VisibleSelection(selectionProxyFactory()));
}

void squarez::SinglePlayerRules::setPause(bool state)
{
	if (state == pause())
		return;
	pauseTimer(state);

#ifndef NO_HTTP_REQUEST
	if (not _url.empty())
		_requestHandle = http::request(_url + onlineSinglePlayer::Pause::encodeRequest(_token, pause(), std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - _epoch)),
			[](std::string /*response*/) // onload
			{
			},
			[this]() // onerror
			{
				_proxy.networkError();
			}
		);
#endif
}

void squarez::SinglePlayerRules::setPlayerName(const std::string& /*name*/)
{
}

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
	std::string name,
	int board_size,
	int nb_symbols,
	std::uint_fast32_t random_seed,
	std::string url,
	std::uint32_t token) :

	Rules(proxy, board_size, nb_symbols, random_seed ? random_seed : std::random_device()(), std::move(timer), name),
	_url(url),
	_token(token),
	_selection(new VisibleSelection(selectionProxyFactory())),
	_epoch(std::chrono::CLOCK_TYPE::now())
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

	cell.setSelected(_selection->togglePoint(cell.shared_from_this()));
	Transition const& tr = _board->selectSquare(*_selection, _random_generator, false);
	if (tr._score)
	{
#ifndef NO_HTTP_REQUEST
		if (not _url.empty())
			_requestHandle = http::request(_url + onlineSinglePlayer::PushSelection::encodeRequest(*_selection, _token, std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::CLOCK_TYPE::now() - _epoch)),
				[this](std::string response) // onload
				{
					DeSerializer s(response);
					onlineSinglePlayer::PushSelection reply(s);
					setGameOver(reply._gameOver);
				},
				[this]() // onerror
				{
					_proxy.networkError();
					_url = "";
				}
			);
#endif

		_selection->setState(Selection::State::validated);
		this->refillTimer(tr._score * 2);
		setScore(score() + tr._score);

		this->applyTransition(tr);
		resetSelection();
	}
}

void squarez::SinglePlayerRules::resetSelection()
{
	for(auto& i: *_selection)
		i->setSelected(false);

	_selection.reset(new VisibleSelection(selectionProxyFactory()));
}

void squarez::SinglePlayerRules::setPause(bool state)
{
	if (state == pause() or gameOver())
		return;
	pauseTimer(state);

#ifndef NO_HTTP_REQUEST
	if (not _url.empty())
		_requestHandle = http::request(_url + onlineSinglePlayer::Pause::encodeRequest(_token, pause(), std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::CLOCK_TYPE::now() - _epoch)),
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

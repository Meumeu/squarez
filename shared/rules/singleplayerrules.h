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

#ifndef SQUAREZ_SINGLEPLAYERRULES_H
#define SQUAREZ_SINGLEPLAYERRULES_H

#include "game/constants.h"
#include "rules/rules.h"
#include "rules/timer.h"
#include "httprequest.h"

namespace squarez {

class SinglePlayerRules : public squarez::Rules
{
	std::string _url;
	std::uint32_t _token;
	std::unique_ptr<VisibleSelection> _selection;
	std::unique_ptr<squarez::http::Handle> _requestHandle;
	std::chrono::steady_clock::time_point _epoch;

public:
	virtual ~SinglePlayerRules();
	SinglePlayerRules(Proxy & proxy,
		Timer && timer,
		std::string name,
		int board_size =  constants::default_board_size,
		int nb_symbols = constants::default_symbols,
		std::uint_fast32_t random_seed = 0,
		std::string url = "",
		std::uint32_t token = 0);

	void onClick(Cell & cell) override;
	void resetSelection() override;
	bool gameOver() override;
	
	void setPause(bool state);
};
}

#endif

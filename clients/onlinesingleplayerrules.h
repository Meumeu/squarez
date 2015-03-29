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

#ifndef SQUAREZ_ONLINESINGLEPLAYERRULES_H
#define SQUAREZ_ONLINESINGLEPLAYERRULES_H

#include "rules/rules.h"
#include "httprequest.h"
#include "rules/timer.h"

namespace squarez {

class OnlineSinglePlayerRules : public squarez::Rules
{
	std::string _url;
	unsigned int _token;
	Selection _selection;
	std::unique_ptr<squarez::http::Handle> _requestHandle;
	std::chrono::steady_clock::time_point _epoch;
	
public:
    virtual ~OnlineSinglePlayerRules();
	OnlineSinglePlayerRules(Proxy & proxy, int board_size, int nb_symbols, std::uint_fast32_t random_seed, std::string url, std::string name, unsigned int token);
	void setPlayerName(std::string const& name) override;
	void onClick(Cell & cell) override;
	void resetSelection() override;
	bool gameOver() override;
};
}

#endif // SQUAREZ_ONLINESINGLEPLAYERRULES_H

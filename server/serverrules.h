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

#ifndef SQUAREZ_SERVERRULES_H
#define SQUAREZ_SERVERRULES_H

#include "rules/rules.h"
#include "highscores.h"

#include <chrono>
#include <random>

namespace squarez {

class DummyProxy : public squarez::Rules::Proxy
{
public:
	virtual ~DummyProxy() {}
	virtual void scoreChanged(unsigned int /*score*/) override {};
	virtual void gameOverChanged(bool /*status*/) override {}
	virtual void timerUpdated() override {}
	virtual void nameRequired() override {}
	virtual void networkError() override {}

	virtual std::unique_ptr<Cell::Proxy> cellProxyFactory(Cell & cell) override;
	virtual std::unique_ptr< VisibleSelection::Proxy > selectionProxyFactory(VisibleSelection& selection) override;
};

class ServerRules : public DummyProxy, public squarez::Rules
{
	std::shared_ptr<HighScores> _highScores;

public:
	ServerRules(std::string playerName,
		std::mt19937::result_type seed,
		int board_size,
		int nb_symbols,
		Timer && timer,
		std::shared_ptr<HighScores> highScores);
	~ServerRules();

	bool playSelection(const Selection & selection, std::chrono::milliseconds msSinceEpoch);

	void setPause(bool state, std::chrono::milliseconds msSinceEpoch);

	// not applicable on server
	virtual void setPlayerName(std::string const&) {};
	virtual void onClick(Cell &) {};
private:
	std::chrono::steady_clock::time_point _epoch;
};
}

#endif // SQUAREZ_SERVERRULES_H

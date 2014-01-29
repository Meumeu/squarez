/*
 * Squarez puzzle game
 * Copyright (C) 2013  Guillaume Meunier <guillaume.meunier@centraliens.net>
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

#ifndef SQUAREZ_MULTIPLAYERRULES_H
#define SQUAREZ_MULTIPLAYERRULES_H

#include "rules.h"
#include "httprequest.h"
#include "shared/timer.h"

#ifndef EMSCRIPTEN
	#include <mutex>
#endif

namespace squarez {

class MultiPlayerRules : public squarez::Rules
{
private:
	Timer _timer;
#ifndef EMSCRIPTEN
	std::mutex _mutex;
#endif
	HttpRequest _xhr;
	
	std::string _url;
	unsigned int _token;

	unsigned int _numberOfRounds;

	//TODO: player scores

public:
	virtual bool gameOver();
	virtual const squarez::Timer& getTimer() { return _timer; }
	virtual void onSelect(const squarez::Selection& selection);
	
	MultiPlayerRules(const std::string& url, const std::string& username);

	// Action not authorized (throws)
	virtual void setPlayerName(std::string const& name);

	unsigned int getNumberOfRounds() const { return _numberOfRounds; }

	// Callback for transition polling mechanism
	void onTransitionPoll(std::string const& serializedTransition);
	// Callback for accepted selection
	void onSelectionPushed(Selection const& selection, std::string const& res);
	// Callback when new scores have been returned
	void onScoreListPoll(std::string const& scoreList);

};

}

#endif // SQUAREZ_MULTIPLAYERRULES_H

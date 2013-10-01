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

#include <client/rules.h>
#include <client/httprequest.h>
#include <shared/timer.h>

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
	std::string _username;
	unsigned int _token;

	//TODO: player scores
	
protected:
	virtual void timeTick(std::chrono::duration<float>);
	
public:
	virtual bool gameOver() const;
	virtual const squarez::Timer& getTimer() const { return _timer; }
	virtual void onSelect(const squarez::Selection& selection);
	
	MultiPlayerRules(const std::string& url, const std::string& username);

	// Callback for transition polling mechanism
	void onTransitionPoll(std::string const& serializedTransition);
	// Callback for accepted selection
	void onSelectionPushed(std::string const& res);

};

}

#endif // SQUAREZ_MULTIPLAYERRULES_H

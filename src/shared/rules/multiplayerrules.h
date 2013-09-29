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

#include <shared/rules/rules.h>
#include <shared/httprequest.h>
#include <shared/timer.h>
#include <mutex>

namespace squarez {

class MultiPlayerRules : public squarez::Rules
{
private:
	Timer _timer;
	std::mutex _mutex;
	HttpRequest _xhr;
	
	std::string _url;
	std::string _username;
	std::string _token;
	
protected:
	virtual void timeTick(std::chrono::duration<float>);
	
public:
	virtual bool gameOver();
	virtual const squarez::Timer& getTimer() { return _timer; }
	virtual void onSelect(const squarez::Selection& selection);
	
	MultiPlayerRules(const std::string& url, const std::string& username);
	
	struct Settings
	{
		Settings(squarez::Serializer & serialized);
		
		GameBoard _board;
		std::string _token;
		
		uint16_t _roundDuration; // in seconds
		float _roundProgress;
		
		unsigned int _numberOfRounds;
		unsigned int _currentRound;
	};
};
}

#endif // SQUAREZ_MULTIPLAYERRULES_H

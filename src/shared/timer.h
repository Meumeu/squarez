/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2013  <copyright holder> <email>
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

#include <chrono>

#ifndef TIMER_H
#define TIMER_H

namespace squarez
{

class Timer
{
public:
	// Build a timer that will last longTerm seconds after an infinite amount of time
	// Initially duration will be shortTerm secons, every halfLife seconds the difference
	// between the current duration and longTerm is divided by half
	Timer(uint16_t longTerm, uint16_t shortTerm, uint16_t halfLife);
	
	void refill(uint8_t percentage);
	
	uint8_t percentageLeft() const;
	uint16_t secondsLeft() const;
	
	void pause();
	void unPause();
private:
	const std::chrono::seconds _longTerm;
	const std::chrono::seconds _halfLife;
	std::chrono::milliseconds _bonusDuration;
	
	std::chrono::steady_clock::time_point _begin;
	std::chrono::steady_clock::time_point _end;
	
	bool _paused;
	std::chrono::steady_clock::time_point _pauseTime;
};
}
#endif // TIMER_H

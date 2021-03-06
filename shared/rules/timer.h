/*
 * Squarez puzzle game
 * Copyright (C) 2013  Patrick Nicolas <patricknicolas@laposte.net>
 * Copyright (C) 2013  Guillaume Meunier <guillaume.meunier@centraliens.net>
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

#ifndef TIMER_H
#define TIMER_H

#include <chrono>

#if __GNUC__ == 4 && __GNUC_MINOR__ < 7 && __clang__ != 1
// workaround for missing c++11 type in gcc 4.6
#	define CLOCK_TYPE monotonic_clock
#elif defined(EMSCRIPTEN)
#	define CLOCK_TYPE system_clock
#else
#	define CLOCK_TYPE steady_clock
#endif

namespace squarez
{

class Timer
{
public:
	// Build a timer that will last longTerm seconds after an infinite amount of time
	// Initially duration will be shortTerm secons, every halfLife seconds the difference
	// between the current duration and longTerm is divided by half
	// You should have (shortTerm - longTerm) * e * ln(2) < halfLife
	// ie halfLife > 1.9 (shortTerm - longTerm)
	Timer(std::chrono::seconds longTerm, std::chrono::seconds shortTerm, std::chrono::seconds halfLife);

	// Simple timer, with constant duration
	Timer(std::chrono::seconds duration, float percentLeft = 1);
	
	void refill(unsigned int percentage);
	void refill(unsigned int percentage, std::chrono::CLOCK_TYPE::time_point when);
	
	float percentageLeft() const;
	float percentageLeft(std::chrono::CLOCK_TYPE::time_point when) const;
	int msLeft() const;
	int msLeft(std::chrono::CLOCK_TYPE::time_point when) const;
	
	void setPause(bool state);
	void setPause(bool state, std::chrono::CLOCK_TYPE::time_point when);
	bool paused() const { return _paused; }
private:
	std::chrono::seconds _longTerm;
	std::chrono::seconds _halfLife;
	std::chrono::milliseconds _bonusDuration;
	
	std::chrono::CLOCK_TYPE::time_point _begin;
	std::chrono::CLOCK_TYPE::time_point _end;
	
	bool _paused;
	std::chrono::CLOCK_TYPE::time_point _pauseTime;
};
}
#endif // TIMER_H

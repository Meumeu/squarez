/*
 * Squarez puzzle game
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

#include "timer.h"
#include <cmath>

namespace squarez
{

Timer::Timer(std::chrono::seconds longTerm, std::chrono::seconds shortTerm, std::chrono::seconds halfLife):
_longTerm(longTerm), _halfLife(halfLife), _bonusDuration(shortTerm - longTerm), _begin(std::chrono::CLOCK_TYPE::now()), _end(_begin + _longTerm + _bonusDuration), _paused(false)
{}

Timer::Timer(std::chrono::seconds duration, float percentLeft):
_longTerm(duration), _halfLife(0), _bonusDuration(0), _begin(std::chrono::CLOCK_TYPE::now()),
_end(_begin + std::chrono::duration_cast<std::chrono::CLOCK_TYPE::duration>(std::chrono::duration<float>(duration) * percentLeft)), _paused(false)
{}

float Timer::percentageLeft() const { return percentageLeft(std::chrono::CLOCK_TYPE::now()); }
float Timer::percentageLeft(std::chrono::CLOCK_TYPE::time_point when) const
{
	float res =  std::chrono::duration<float>(_end - (_paused ? _pauseTime : when)) / std::chrono::duration<float>(_longTerm + _bonusDuration);
	if (res > 0)
		return res;
	return 0;
}

int Timer::msLeft() const { return msLeft(std::chrono::CLOCK_TYPE::now()); }
int Timer::msLeft(std::chrono::CLOCK_TYPE::time_point when) const
{
	return (std::chrono::duration_cast<std::chrono::milliseconds>(_end - (_paused ? _pauseTime : when))).count();
}

void Timer::refill(unsigned int percentage) { refill(percentage, std::chrono::CLOCK_TYPE::now()); }
void Timer::refill(unsigned int percentage, std::chrono::CLOCK_TYPE::time_point newBegin)
{
	//Calculate decay of remaining time
	if (_bonusDuration.count())
	{
		_bonusDuration = std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::duration<double>(_bonusDuration) * std::exp2( std::chrono::duration<double>(_begin - newBegin) / std::chrono::duration<double>(_halfLife)));
	}

	// The timer can not have negative values
	if (_end < newBegin)
		_end = newBegin;
	
	_end += percentage * (_longTerm + _bonusDuration) / 100;
	_begin = newBegin;
	if (_end - _begin > _longTerm + _bonusDuration)
		_end = _begin + _longTerm + _bonusDuration;
}

void Timer::setPause(bool state) { setPause(state, std::chrono::CLOCK_TYPE::now()); }
void Timer::setPause(bool state, std::chrono::CLOCK_TYPE::time_point when)
{
    if (state == _paused)
        return;

    _paused = state;

    if (_paused)
    {
        _pauseTime = when;
    }
    else
    {
        auto elapsed = when - _pauseTime;
        _begin += elapsed;
        _end += elapsed;
    }
}

}

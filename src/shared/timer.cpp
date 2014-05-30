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
_longTerm(longTerm), _halfLife(halfLife), _bonusDuration(shortTerm - longTerm), _begin(std::chrono::steady_clock::now()), _end(_begin + _longTerm + _bonusDuration), _paused(false)
{}

Timer::Timer(std::chrono::seconds duration, float percentLeft):
_longTerm(duration), _halfLife(0), _bonusDuration(0), _begin(std::chrono::steady_clock::now()),
_end(_begin + std::chrono::duration_cast<std::chrono::steady_clock::duration>(std::chrono::duration<float>(duration) * percentLeft)), _paused(false)
{}

float Timer::percentageLeft(float offset) const
{
	std::chrono::duration<float> offset_chrono(offset);
	float res =  (std::chrono::duration<float>(_end - std::chrono::steady_clock::now()) - offset_chrono) / std::chrono::duration<float>(_longTerm + _bonusDuration);
	if (_paused)
		res = std::chrono::duration<float>(_end - _pauseTime) / std::chrono::duration<float>(_longTerm + _bonusDuration);
	if (res > 0)
		return res;
	return 0;
}

uint16_t Timer::secondsLeft() const
{
	auto res = (std::chrono::duration_cast<std::chrono::seconds>(_end - std::chrono::steady_clock::now())).count();
	if (_paused)
		res = (std::chrono::duration_cast<std::chrono::seconds>(_end - _pauseTime)).count();
	if (res > 0)
		return res;
	return 0;
}

unsigned int Timer::msLeft() const
{
	auto res = (std::chrono::duration_cast<std::chrono::milliseconds>(_end - std::chrono::steady_clock::now())).count();
	if (_paused)
		res = (std::chrono::duration_cast<std::chrono::milliseconds>(_end - _pauseTime)).count();
	if (res > 0)
		return res;
	return 0;
}


void Timer::refill(unsigned int percentage)
{
	auto newBegin(std::chrono::steady_clock::now());
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

void Timer::setPause(bool state)
{
    if (state == _paused)
        return;

    _paused = state;

    if (_paused)
    {
        _pauseTime = std::chrono::steady_clock::now();
    }
    else
    {
        auto elapsed = std::chrono::steady_clock::now() - _pauseTime;
        _begin += elapsed;
        _end += elapsed;
    }
}

}

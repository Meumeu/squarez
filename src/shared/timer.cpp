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

Timer::Timer(uint16_t longTerm, uint16_t shortTerm, uint16_t halfLife):
_longTerm(longTerm), _halfLife(halfLife), _bonusDuration(1000*(uint32_t)(shortTerm - longTerm)), _begin(std::chrono::steady_clock::now()), _end(_begin + _longTerm + _bonusDuration), _paused(false)
{}

float Timer::percentageLeft() const
{
	float res =  std::chrono::duration<float>(_end - std::chrono::steady_clock::now()) / std::chrono::duration<float>(_longTerm + _bonusDuration);
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

void Timer::refill(unsigned int percentage)
{
	auto newBegin(std::chrono::steady_clock::now());
	//Calculate decay of remaining time
	_bonusDuration = std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::duration<double>(_bonusDuration) * std::exp2( std::chrono::duration<double>(_begin - newBegin) / std::chrono::duration<double>(_halfLife)));
	
	_end += percentage * (_longTerm + _bonusDuration) / 100;
	_begin = newBegin;
	if (_end - _begin > _longTerm + _bonusDuration)
		_end = _begin + _longTerm + _bonusDuration;
}

void Timer::pause()
{
	if (not _paused)
	{
		_pauseTime = std::chrono::steady_clock::now();
		_paused = true;
	}
}

void Timer::unPause()
{
	if (_paused)
	{
		_paused = false;
		auto elapsed = std::chrono::steady_clock::now() - _pauseTime;
		_begin += elapsed;
		_end += elapsed;
	}
}

}
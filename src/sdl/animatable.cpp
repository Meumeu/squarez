/*
 * Squarez puzzle game
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

#include "animatable.h"

std::chrono::time_point<std::chrono::steady_clock> squarez::Animatable::currentTime;

squarez::Animatable::Animatable(float value) : x0(value), x1(value), period(0), startTime(std::chrono::steady_clock::now()), isPaused(false)
{
	
}

const squarez::Animatable& squarez::Animatable::operator=(float value)
{
	x0 = *this;
	x1 = value;
	startTime = (isPaused ? pauseTime : currentTime);
	
	return *this;
}

void squarez::Animatable::setInstantly(float value)
{
	x0 = x1 = value;
}


squarez::Animatable::operator float() const
{
	if (period.count() > 0.001)
	{
		std::chrono::duration<float> dt = (isPaused ? pauseTime : currentTime) - startTime;
		
		if (dt.count() < 0)
			return x0;
		else if (dt.count() > period.count())
			return x1;
		else
		{
			float alpha = dt.count() / period.count();
			float dx = x1 - x0;
			
			return x0 + dx * alpha * alpha;
		}
	}
	else
	{
		return x1;
	}
}

void squarez::Animatable::setPeriod(std::chrono::duration<float> _period)
{
	period = _period;
}

void squarez::Animatable::setTime(std::chrono::time_point<std::chrono::steady_clock> t)
{
	currentTime = t;
}

void squarez::Animatable::pause()
{
	if (isPaused) return;
	
	pauseTime = currentTime;
	isPaused = true;
}

void squarez::Animatable::unpause()
{
	if (!isPaused) return;
	
	startTime = startTime + (currentTime - pauseTime);
	isPaused = false;
}

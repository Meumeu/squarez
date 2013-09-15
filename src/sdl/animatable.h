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

#ifndef ANIMATABLE_H
#define ANIMATABLE_H

#include <chrono>

namespace squarez
{
class Animatable
{
private:
	float x0; // initial value;
	float x1; // final value
	std::chrono::duration<float> period;
	std::chrono::time_point<std::chrono::steady_clock> startTime;
	std::chrono::time_point<std::chrono::steady_clock> pauseTime;
	static std::chrono::time_point<std::chrono::steady_clock> currentTime;
	bool isPaused;
	
public:
	operator float() const;
	Animatable(float value = 0);
	const Animatable& operator=(float value);
	
	void setPeriod(std::chrono::duration<float> period);
	void setInstantly(float value);
	
	static void setTime(std::chrono::time_point<std::chrono::steady_clock> t);
	
	void pause();
	void unpause();
};
}

#endif

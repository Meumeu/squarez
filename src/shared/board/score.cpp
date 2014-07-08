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

#include "score.h"

#include "shared/serializer.h"

namespace squarez{

Score::Score(unsigned int score, const std::string& name):
_score(score), _name(name), _date(std::chrono::system_clock::now()) {}

Score::Score(unsigned int score, const std::string& name, std::time_t date):
_score(score), _name(name), _date(std::chrono::system_clock::from_time_t(date)) {}

std::time_t Score::getDate() const
{
	return std::chrono::system_clock::to_time_t(_date);
}

DeSerializer& operator>>(DeSerializer& in, Score& score)
{
	std::time_t date;
	in >> date >> score._name >> score._score;
	score._date = std::chrono::system_clock::from_time_t(date);
	return in;
}
Serializer& operator<<(Serializer& out, const Score& score)
{
	out << score.getDate() << score._name << score._score;
	return out;
}

}

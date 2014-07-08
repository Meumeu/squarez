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

#ifndef SQUAREZ_SCORE_H
#define SQUAREZ_SCORE_H

#include <chrono>
#include <string>

namespace squarez {

class DeSerializer;
class Serializer;

class Score
{
public:
	Score(unsigned int score, std::string const& name);
	Score(unsigned int score, std::string const& name, std::time_t date);
	Score() = default;

	unsigned int _score;
	std::string _name;
	std::chrono::system_clock::time_point _date;

	std::time_t getDate() const;

	bool operator<(Score const& other) const { return _score < other._score; }
};

Serializer & operator<<(Serializer & out, Score const& score);
DeSerializer & operator>>(DeSerializer & in, Score & score);

}

#endif // SQUAREZ_SCORE_H

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

#ifndef SQUAREZ_HIGHSCORES_H
#define SQUAREZ_HIGHSCORES_H

#include <set>
#include <string>
#include <chrono>

namespace squarez {

class HighScores
{
public:
	struct Score
	{
		Score(unsigned int score, std::string const& name);

		unsigned int _score;
		std::string _name;
		std::chrono::system_clock::time_point _date;

		bool operator<(Score const& other) { return _score < other._score; }
	};

	HighScores(unsigned int maxScores);

	bool save(unsigned int score, std::string const& name);

	std::multiset<Score> const& getScores() const { return _scores; }

private:
	unsigned int _maxScores;
	std::multiset<Score> _scores;
};
}

#endif // SQUAREZ_HIGHSCORES_H

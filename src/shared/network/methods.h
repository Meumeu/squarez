/*
 * Squarez puzzle game
 * Copyright (C) 2013  Guillaume Meunier <guillaume.meunier@centraliens.net>
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

#ifndef SQUAREZ_METHODS_H
#define SQUAREZ_METHODS_H

#include <chrono>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "shared/board/gameboard.h"
#include "shared/board/score.h"

namespace squarez
{
	class DeSerializer;
	class Serializer;
	class Selection;

	// Get the parameters of a multiplayer name
	struct GameInit
	{
		static std::string const& method();
		static std::string encodeRequest(std::string const& playerName);

		GameInit(DeSerializer & serialized);

		// Serialize the setting elements into a valid object
		static void serialize(Serializer & serialized,
			GameBoard const&,
			unsigned int token,
			std::chrono::seconds roundDuration,
			float roundProgress,
			unsigned int numberOfRounds,
			unsigned int currentRound);


		std::unique_ptr<GameBoard> _board;
		unsigned int _token;

		std::chrono::seconds _roundDuration;
		float _roundProgress;

		unsigned int _numberOfRounds;
		unsigned int _currentRound;
	};

	struct TransitionPoll
	{
		static std::string const& method();
		static std::string encodeRequest(unsigned int token);

		TransitionPoll(DeSerializer & serialized);

		static void serialize(Serializer & serialized, unsigned int round, Transition const& transition);

		unsigned int _round;
		Transition _transition;
	};

	namespace PushSelection
	{
		std::string const& method();
		std::string encodeRequest(Selection const& selection, unsigned int token);
	};

	struct ScoreList
	{
		static std::string const& method();
		static std::string encodeRequest();

		ScoreList(DeSerializer & serialized);
		ScoreList() = default;

		std::vector<Score> _scores;
	};
	Serializer & operator<<(Serializer & out, ScoreList const& scoreList);
}

#endif // SQUAREZ_METHODS_H

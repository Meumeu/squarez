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

#include "client/multiplayerrules.h"
#include "client/singleplayerrules.h"
#include "client/tutorialrules.h"
#include "client/highscores.h"
#include "client/ui.h"
#include "shared/gameboard.h"
#include "shared/selection.h"
#include "shared/score.h"

#ifndef EMSCRIPTEN
#error this file should be compiled with emscripten
#endif

#include <emscripten/bind.h>

using namespace squarez;

EMSCRIPTEN_BINDINGS(Selection) {
	emscripten::class_<Selection>("Selection")
		.constructor<>()
		.function("addPoint", &Selection::addPoint)
		.function("getX", &Selection::getX)
		.function("getY", &Selection::getY)
		.property("valid", &Selection::isValid)
		;
}

EMSCRIPTEN_BINDINGS(GameBoard) {
	emscripten::class_<GameBoard>("GameBoard")
		.function("size", &GameBoard::size)
		.function("get", emscripten::select_overload<unsigned int(unsigned int, unsigned int)const>(&GameBoard::get))
		;
}

EMSCRIPTEN_BINDINGS(Transition) {
	emscripten::class_<Transition>("Transition")
	.function("getCellTransition", &Transition::getCellTransition)
	.function("size", &Transition::size)
	.function("get", &Transition::get)
	.property("score", &Transition::_score)
	;
}

EMSCRIPTEN_BINDINGS(CellTransition) {
	emscripten::class_<Transition::CellTransition>("CellTransition")
	.property("fromx", &Transition::CellTransition::_fromx)
	.property("fromy", &Transition::CellTransition::_fromy)
	.property("tox", &Transition::CellTransition::_tox)
	.property("toy", &Transition::CellTransition::_toy)
	.property("removed", &Transition::CellTransition::_removed)
	.property("symbol", &Transition::CellTransition::_symbol)
	.function("isNew", &Transition::CellTransition::isNew)
	;
}

EMSCRIPTEN_BINDINGS(Score) {
	emscripten::class_<Score>("Score")
	.property("score", &Score::_score)
	.property("name", &Score::_name)
	.property("date", &Score::getDate)
	;

	emscripten::register_vector<Score>("VectorScore");

	emscripten::class_<HighScores>("HighScores")
	.property("scores", &HighScores::getScores)
	;
}

struct UIWrapper: public emscripten::wrapper<UI>
{
	EMSCRIPTEN_WRAPPER(UIWrapper);
	void onGameOverChanged(bool value)
	{
		return call<void>("onGameOverChanged", value);
	}
	void onTransition(Transition const& transition)
	{
		return call<void>("onTransition", transition);
	}
	void onScoreChanged(int new_score)
	{
		return call<void>("onScoreChanged", new_score);
	}
	void onScoreListChanged(std::vector<Score> const& scores)
	{
		return call<void>("onScoreListChanged", scores);
	}
	void onSelectionAccepted(Selection const& selection)
	{
		return call<void>("onSelectionAccepted", selection);
	}
	void onSelectionApplied(Selection const& selection)
	{
		return call<void>("onSelectionApplied", selection);
	}
	void onSelectionRejected(Selection const& selection)
	{
		return call<void>("onSelectionRejected", selection);
	}
	void onMessage(std::string const& message)
	{
		return call<void>("onMessage", message);
	}
	void onTimerUpdated(float currentPercentage)
	{
		return call<void>("onTimerUpdated", currentPercentage);
	}
	void nameRequired(std::string const& lastName)
	{
		return call<void>("onNameRequired", lastName);
	}
};

EMSCRIPTEN_BINDINGS(UI) {
	emscripten::class_<UI>("UI")
	.function("setRules", &UI::setRules)
	.allow_subclass<UIWrapper>("UIWrapper")
	;
}

EMSCRIPTEN_BINDINGS(Rules) {
	emscripten::class_<Rules>("Rules")
	.smart_ptr<std::shared_ptr<Rules>>("shared_ptr<Rules>")
	.function("getBoard", emscripten::select_overload<GameBoard const*()const>(&Rules::getBoard), emscripten::allow_raw_pointers())
	.function("getGameOver", &Rules::gameOver)
	.function("getPercentageLeft", &Rules::getPercentageLeft)
	.function("msLeft", &Rules::msLeft)
	;

	emscripten::class_<SinglePlayerRules, emscripten::base<Rules>>("SinglePlayerRules")
	.smart_ptr_constructor("shared_ptr<Rules>", &std::make_shared<SinglePlayerRules,int,int,int,int,int>)
	.function("onSelect", &SinglePlayerRules::onSelect)
	.function("setPlayerName", &SinglePlayerRules::setPlayerName)
	.function("getHighScores", &SinglePlayerRules::getHighScores, emscripten::allow_raw_pointers())
	.property("pause", &SinglePlayerRules::pause, &SinglePlayerRules::setPause)
	.property("score", &SinglePlayerRules::getScore)
	;

	emscripten::class_<MultiPlayerRules, emscripten::base<Rules>>("MultiPlayerRules")
	.smart_ptr_constructor("shared_ptr<Rules>", &std::make_shared<MultiPlayerRules,std::string,std::string>)
	.function("onSelect", &MultiPlayerRules::onSelect)
	.function("setPlayerName", &MultiPlayerRules::setPlayerName)
	.property("currentRound", &MultiPlayerRules::currentRound)
	.property("numberOfRounds", &MultiPlayerRules::getNumberOfRounds)
	;

	emscripten::class_<TutorialRules, emscripten::base<Rules>>("TutorialRules")
	.smart_ptr_constructor("shared_ptr<Rules>", &std::make_shared<TutorialRules,int,int>)
	.function("next", &TutorialRules::next)
	.function("onSelect", &TutorialRules::onSelect)
	.function("setPlayerName", &TutorialRules::setPlayerName)
	;
}

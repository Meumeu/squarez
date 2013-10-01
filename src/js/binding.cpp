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

#include "shared/gameboard.h"
#include "shared/timer.h"
#include "shared/selection.h"
#include "client/singleplayerrules.h"
#include "client/ui.h"

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
	.property("selection", &Transition::_selection)
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

EMSCRIPTEN_BINDINGS(Timer) {
	emscripten::class_<Timer>("Timer")
	.function("percentageLeft", &Timer::percentageLeft)
	.function("secondsLeft", &Timer::secondsLeft)
	.function("paused", &Timer::paused)
	;
}

struct UIWrapper: public emscripten::wrapper<UI>
{
	EMSCRIPTEN_WRAPPER(UIWrapper);
	void onTransition(Transition const& transition)
	{
		return call<void>("onTransition", transition);
	}
	void onScoreChanged(int new_score)
	{
		return call<void>("onScoreChanged", new_score);
	}
	void onSelectionAccepted(Selection const& selection)
	{
		return call<void>("onSelectionAccepted", selection);
	}
};

EMSCRIPTEN_BINDINGS(UI) {
	emscripten::class_<UI>("UI")
	.function("onTransition", &UI::onTransition)
	.function("onScoreChanged", &UI::onScoreChanged)
	.function("onSelectionAccepted", &UI::onSelectionAccepted)
	.function("setRules", &UI::setRules)
	.allow_subclass<UIWrapper>()
	;
}

EMSCRIPTEN_BINDINGS(SinglePlayerRules) {
	emscripten::class_<Rules>("Rules")
	.smart_ptr<std::shared_ptr<Rules>>()
	;
	emscripten::class_<SinglePlayerRules, emscripten::base<Rules>>("SinglePlayerRules")
	.smart_ptr_constructor(&std::make_shared<SinglePlayerRules,int,int,int,int,int>)
	.function("onSelect", &SinglePlayerRules::onSelect)
	.function("gameOver", &SinglePlayerRules::gameOver)
	.function("pause", &SinglePlayerRules::pause)
	.function("unPause", &SinglePlayerRules::unpause)
	.property("timer", &SinglePlayerRules::getTimer)
	.property("board", &SinglePlayerRules::getBoard)
	.property("score", &SinglePlayerRules::getScore)
	;
}


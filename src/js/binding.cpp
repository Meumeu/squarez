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

#include <emscripten/bind.h>

using namespace squarez;

EMSCRIPTEN_BINDINGS(Selection) {
	emscripten::class_<Selection>("Selection")
		.constructor<>()
		.function("addPoint", &Selection::addPoint)
		.function("getX", &Selection::getX)
		.function("getY", &Selection::getY)
		.function("serialize", emscripten::select_overload<std::string(void)const>(&Selection::serialize))
		;
}

EMSCRIPTEN_BINDINGS(GameBoard) {
	emscripten::class_<GameBoard>("GameBoard")
		.constructor<int,int>()
		.constructor<std::string>()
		.function("selectSquare", &GameBoard::selectSquare)
		.function("applyTransition", &GameBoard::applyTransition)
		.function("size", &GameBoard::size)
		.function("get", emscripten::select_overload<unsigned int(unsigned int, unsigned int)const>(&GameBoard::get))
		.function("print", &GameBoard::print)
		;
}

EMSCRIPTEN_BINDINGS(Transition) {
	emscripten::class_<Transition>("Transition")
	.constructor<std::string>()
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
	.constructor<int,int,int>()
	.constructor<int>()
	.function("refill", &Timer::refill)
	.function("percentageLeft", &Timer::percentageLeft)
	.function("secondsLeft", &Timer::secondsLeft)
	.function("pause", &Timer::pause)
	.function("unPause", &Timer::unPause)
	.function("paused", &Timer::paused)
	;
}
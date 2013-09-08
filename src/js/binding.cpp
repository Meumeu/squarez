#include "shared/gameboard.h"
#include "shared/timer.h"

#include <emscripten/bind.h>

using namespace squarez;

EMSCRIPTEN_BINDINGS(Selection) {
	emscripten::class_<Selection>("Selection")
		.constructor<>()
		.function("addPoint", &Selection::addPoint)
		.function("getPoint", &Selection::getPoint)
		;
}

EMSCRIPTEN_BINDINGS(GameBoard) {
	emscripten::class_<GameBoard>("GameBoard")
		.constructor<int,int>()
		.function("selectSquare", &GameBoard::selectSquare)
		.function("applyTransition", &GameBoard::applyTransition)
		.function("size", &GameBoard::size)
		.function("get", emscripten::select_overload<uint8_t(uint8_t, uint8_t)const>(&GameBoard::get))
		.function("print", &GameBoard::print)
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

EMSCRIPTEN_BINDINGS(Timer) {
	emscripten::class_<Timer>("Timer")
	.constructor<int,int,int>()
	.function("refill", &Timer::refill)
	.function("percentageLeft", &Timer::percentageLeft)
	.function("secondsLeft", &Timer::secondsLeft)
	.function("pause", &Timer::pause)
	.function("unPause", &Timer::unPause)
	;
}
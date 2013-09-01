#include "shared/gameboard.h"

#include <emscripten/bind.h>

using namespace squarez;

EMSCRIPTEN_BINDINGS(Selection) {
	emscripten::class_<Selection>("Selection")
		.constructor<>()
		.function("addPoint", &Selection::addPoint)
		;
}

EMSCRIPTEN_BINDINGS(GameBoard) {
	emscripten::class_<GameBoard>("GameBoard")
		.constructor<int,int>()
		.function("selectSquare", &GameBoard::selectSquare)
		.function("applyTransition", &GameBoard::applyTransition)
		.function("size", &GameBoard::size)
		.function("get", emscripten::select_overload<uint8_t(uint8_t, uint8_t)const>(&GameBoard::get))
		;
}

EMSCRIPTEN_BINDINGS(Transition) {
	emscripten::class_<Transition>("Transition")
	.function("getMove", &Transition::getMove)
	.function("getRemoved", &Transition::getRemoved)
	.property("score", &Transition::_score)
	;
}
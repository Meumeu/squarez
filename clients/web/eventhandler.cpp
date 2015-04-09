/*
 * Squarez puzzle game
 * Copyright (C) 2015  Patrick Nicolas <patricknicolas@laposte.net>
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

#include "eventhandler.h"
#include <sstream>

namespace squarez {
namespace internal {
struct EventHandler
{
	std::function<void(emscripten::val)> _callback;

	EventHandler(std::function<void(emscripten::val)> callback): _callback(callback) {}
	void handleEvent(emscripten::val v) { _callback(v); }
};

void callHandler(EventHandler & h, emscripten::val v)
{
	h.handleEvent(v);
}

}
}

EMSCRIPTEN_BINDINGS(eventHandler)
{
	emscripten::class_<squarez::internal::EventHandler>("EventHandler")
	.function<void>("handleEvent", &squarez::internal::EventHandler::handleEvent);

	emscripten::function<void>("callHandler", &squarez::internal::callHandler);
}

squarez::web::EventHandler::EventHandler(emscripten::val target, const std::string & event, std::function<void(emscripten::val)> callback, bool useCapture):
	_target(target),
	_event(event),
	_handler(internal::EventHandler(callback))
{
	_target.call<void>("addEventListener", _event, _handler, useCapture);
}

squarez::web::EventHandler::~EventHandler()
{
	_target.call<void>("removeEventListener", _event, _handler);
	_handler.call<void>("delete");
}

void squarez::web::EventHandler::setCallback(std::function<void(emscripten::val)> callback)
{
	_handler.as<internal::EventHandler &>()._callback = callback;
}

void squarez::web::EventHandler::setTimeout(int ms, emscripten::val value)
{
	auto window = emscripten::val::global("window");
	window.call<void>("setTimeout", window["Squarez"]["callHandler"], ms, _handler, value);
}
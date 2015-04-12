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

#include "jscallback.h"
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

squarez::web::JSCallback::JSCallback(std::function<void(emscripten::val)> callback):
	_hasTimeout(false),
	_handler(internal::EventHandler(callback)) {}

squarez::web::JSCallback::~JSCallback()
{
	clearTimeout();
	for (auto & target: _targets)
		target.first.call<void>("removeEventListener", target.second, _handler);
	_handler.call<void>("delete");
}

void squarez::web::JSCallback::addEventListener(emscripten::val target, const std::string& event, bool useCapture)
{
	target.call<void>("addEventListener", event, _handler, useCapture);
	_targets.emplace_back(target, event);
}

void squarez::web::JSCallback::setTimeout(int ms, emscripten::val value)
{
	clearTimeout();
	auto window = emscripten::val::global("window");
	window.call<int>("setTimeout", window["Squarez"]["callHandler"], ms, _handler, value);
}

void squarez::web::JSCallback::clearTimeout()
{
	auto window = emscripten::val::global("window");
	if (_hasTimeout)
		window.call<void>("clearTimeout", _timeoutId);
	_hasTimeout = false;
}

void squarez::web::JSCallback::setCallback(std::function<void(emscripten::val)> callback)
{
	_handler.as<internal::EventHandler &>()._callback = callback;
}
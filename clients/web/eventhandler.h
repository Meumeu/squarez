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

#ifndef SQUAREZ_WEB_EVENTHANDLER_H
#define SQUAREZ_WEB_EVENTHANDLER_H

#include <functional>
#include <string>

#include <emscripten/bind.h>

namespace squarez {
namespace web {

class EventHandler
{
	std::function<void(emscripten::val)> _callback;
	EventHandler(std::function<void(emscripten::val)> callback): _callback(callback) {}
public:
	void handleEvent(emscripten::val v) { _callback(v); }

	static void addEventHandler(emscripten::val element, const std::string & event, std::function<void(emscripten::val)> handler, bool useCapture);
};

}}

#endif // SQUAREZ_WEB_EVENTHANDLER_H

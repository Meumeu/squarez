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


EMSCRIPTEN_BINDINGS(eventHandler)
{
	emscripten::class_<squarez::web::EventHandler>("EventHandler")
	.function<void>("handleEvent", &squarez::web::EventHandler::handleEvent);
}

void squarez::web::EventHandler::addEventHandler(emscripten::val element, const std::string& event, std::function<void(emscripten::val)> handler, bool useCapture)
{
	std::string name = event + "Handler";
	// Store the handler as a member to avoid garbage collection
	element.set(name, EventHandler(handler));
	element.call<void>("addEventListener", event, element[name], useCapture);
}

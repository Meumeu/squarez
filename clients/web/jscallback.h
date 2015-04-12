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

#ifndef SQUAREZ_WEB_JSCALLBACK_H
#define SQUAREZ_WEB_JSCALLBACK_H

#include <functional>
#include <string>

#include <emscripten/bind.h>

namespace squarez {
namespace web {

class JSCallback
{
	std::vector<std::pair<emscripten::val, std::string>> _targets;
	bool _hasTimeout;
	int _timeoutId;
	emscripten::val _handler;
public:
	JSCallback(std::function<void(emscripten::val)> callback);
	~JSCallback();

	void setCallback(std::function<void(emscripten::val)> callback);

	void addEventListener(emscripten::val target, const std::string & event, bool useCapture = false);
	void setTimeout(int ms, emscripten::val value = emscripten::val::undefined());

	void clearTimeout();
};

}}

#endif // SQUAREZ_WEB_EVENTHANDLER_H

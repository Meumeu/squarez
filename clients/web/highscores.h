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

#ifndef SQUAREZ_WEB_HIGHSCORES_H
#define SQUAREZ_WEB_HIGHSCORES_H

#include "httprequest.h"
#include "jscallback.h"

#include <emscripten/val.h>

#include <memory>

namespace squarez {
namespace web {

class HighScores
{
public:
	enum class Page
	{
		lastDay,
		lastWeek,
		lastMonth,
		forever
	};
private:

	emscripten::val _rootElement;
	emscripten::val _buttonBefore;
	emscripten::val _content;
	emscripten::val _buttonAfter;
	JSCallback _before;
	JSCallback _after;
	const std::string _url;
	unsigned int _numScores;
	Page _page;
	std::unique_ptr<squarez::http::Handle> _requestHandle;
	
	void switchPage(int count);
public:
	HighScores(emscripten::val rootElement, std::string url);
};
}}

#endif // SQUAREZ_WEB_HIGHSCORES_H

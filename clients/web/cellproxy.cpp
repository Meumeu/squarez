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

#include "cellproxy.h"
#include "jscallback.h"
#include "rulesproxy.h"

#include <sstream>

squarez::web::CellProxy::CellProxy(squarez::Cell & owner, squarez::web::RulesProxy & rules) : Proxy(owner),
_rules(rules),
_element(emscripten::val::global("document").call<emscripten::val>("createElement", emscripten::val("div"))),
_clickHandler([this](emscripten::val event){event.call<void>("preventDefault"); _rules.message(""); _owner.click();}),
_moveHandler([this](emscripten::val){setXY();})
{
	std::stringstream elClass;
	elClass << "cell symbol" << owner.symbol;
	_element.set("className", elClass.str());
	setXY();
	rules._rootElement.call<void>("appendChild", _element);
	_clickHandler.addEventListener(_element, "click");
	_clickHandler.addEventListener(_element, "touchstart");
}

squarez::web::CellProxy::~CellProxy()
{
	_rules._rootElement.call<void>("removeChild", _element);
}

void squarez::web::CellProxy::moved(int /*x*/, int /*y*/)
{
	_rules._rootElement["classList"].call<void>("remove", emscripten::val("resizing"));
	_moveHandler.setTimeout(100);
}

void squarez::web::CellProxy::selectChanged(bool status)
{
	if (status)
		_element["classList"].call<void>("add", emscripten::val("selected"));
	else
		_element["classList"].call<void>("remove", emscripten::val("selected"));
}

void squarez::web::CellProxy::setXY()
{
	std::stringstream transform;
	transform << "translate("
		<< _owner.x() << "em,"
		<< _owner.y() << "em)";
	_element["style"].set("transform", transform.str());
}

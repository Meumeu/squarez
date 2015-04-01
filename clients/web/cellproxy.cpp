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
#include "rulesproxy.h"

#include <emscripten/emscripten.h>
#include <emscripten/val.h>

#include <sstream>

void setXY(squarez::web::CellProxy & proxy, int x, int y)
{
	proxy.setXY(x,y);
}

EMSCRIPTEN_BINDINGS(cellproxy)
{
	emscripten::class_<squarez::web::CellProxy>("CellProxy")
	.function("click", &squarez::web::CellProxy::click)
	.function("setXY", &squarez::web::CellProxy::setXY);

	emscripten::function("setXY", &setXY);
}

squarez::web::CellProxy::CellProxy(squarez::Cell & owner, squarez::web::RulesProxy & rules) : Proxy(owner),
_rules(rules),
_element(emscripten::val::global("document").call<emscripten::val>("createElement", emscripten::val("div")))
{
	std::stringstream elClass;
	elClass << "cell symbol" << owner.symbol;
	_element.set("className", elClass.str());
	_element.set("proxy", *this);
	emscripten::val::global("window").set("e", _element);
	EM_ASM(window.e.onclick = function () {this.proxy.click()});
	setXY(owner.x(), owner.y());
	rules._rootElement.call<void>("appendChild", _element);
}

squarez::web::CellProxy::~CellProxy()
{
	_rules._rootElement.call<void>("removeChild", _element);
}

void squarez::web::CellProxy::moved(int x, int y)
{
	emscripten::val::global("window").call<void>("setTimeout", emscripten::val::global("Module")["setXY"], 100, _element["proxy"], x, y);
}

void squarez::web::CellProxy::selectChanged(bool status)
{
	if (status)
		_element["classList"].call<void>("add", emscripten::val("selected"));
	else
		_element["classList"].call<void>("remove", emscripten::val("selected"));
}

void squarez::web::CellProxy::click()
{
	_rules.click(_owner);
}


void squarez::web::CellProxy::setXY (int x, int y)
{
	std::stringstream strx;
	strx << x << "em";
	_element["style"].set("left", strx.str());
	std::stringstream stry;
	stry << y << "em";
	_element["style"].set("top", stry.str());
}

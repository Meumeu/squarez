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

#include "selectionproxy.h"
#include "eventhandler.h"
#include "rulesproxy.h"

#include <cmath>
#include <sstream>
#include <iostream>

#include <emscripten/val.h>

namespace
{

emscripten::val make_val_internal(std::stringstream && str)
{
	return emscripten::val(str.str());
}

template<typename T, typename ...Args>
emscripten::val make_val_internal(std::stringstream && str, T val, Args ... args)
{
	str << val;
	return make_val_internal(std::move(str), args...);
}

template <typename ...Args>
emscripten::val make_val(Args... args)
{
	return make_val_internal(std::stringstream(), args...);
}
}

squarez::web::SelectionProxy::SelectionProxy(squarez::VisibleSelection& owner, RulesProxy & rules): squarez::VisibleSelection::Proxy(owner), _rules(rules)
{

}

squarez::web::SelectionProxy::~SelectionProxy()
{
	try
	{
		switch (_owner.state())
		{
		case VisibleSelection::State::incomplete:
			return;
		case VisibleSelection::State::validated:
			// This should not happen, but check anyway
			if (not _owner.isValid())
				return;
			{
				// Get the center of the selection
				std::pair<double, double> center{0,0};
				for (auto cell : _owner)
				{
					center.first += cell->x();
					center.second += cell->y();
				}
				center.first /= 4;
				center.second /= 4;
				auto symbol = (*_owner.begin())->symbol;

				auto p1 = _owner.begin();
				auto p0 = p1++;

				auto size = std::hypot((*p0)->x() - (*p1)->x(), (*p0)->y() - (*p1)->y());
				auto rotation = std::atan2((*p1)->y() - (*p0)->y(), (*p1)->x() - (*p0)->x());

				auto n = emscripten::val::global("document").call<emscripten::val>("createElement", emscripten::val("div"));
				auto n1 = emscripten::val::global("document").call<emscripten::val>("createElement", emscripten::val("div"));
				n.call<void>("appendChild", n1);

				n["style"].set("left", make_val(0.5 + center.first, "em"));
				n["style"].set("top", make_val(0.5 + center.second, "em"));
				n["style"].set("position", emscripten::val("absolute"));
				n["style"].set("transform", make_val("rotate(", rotation, "rad)"));
				n["style"].set("transformOrigin", emscripten::val("0 0"));
				n["classList"].call<void>("add", emscripten::val("transition-square-container"));

				n1["style"].set("marginLeft", emscripten::val("-50%"));
				n1["style"].set("marginTop", emscripten::val("-50%"));
				n1["style"].set("width", make_val(size, "em"));
				n1["style"].set("height", make_val(size, "em"));
				n1["classList"].call<void>("add", emscripten::val("transition-square"));
				n1["classList"].call<void>("add", make_val("symbol", symbol));

				auto & rootElement = _rules._rootElement;
				EventHandler::addEventHandler(n, "animationend",
					[n, rootElement](emscripten::val)
					{
						rootElement.call<void>("removeChild", n);
					},
					false
				);
				EventHandler::addEventHandler(n, "webkitAnimationEnd",
					[n, rootElement](emscripten::val)
					{
						rootElement.call<void>("removeChild", n);
					},
					false
				);

				rootElement.call<void>("appendChild", n);
			}
		}
	}
	catch (...) {}
}

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

#include "highscores.h"

#include "network/methods.h"
#include "utils/serializer.h"
#include "utils/unicode.h"

#include <emscripten/bind.h>

#include <sstream>

EMSCRIPTEN_BINDINGS(highscores)
{
	emscripten::class_<squarez::web::HighScores>("HighScores")
	.constructor<emscripten::val, std::string>();
}

namespace
{
unsigned int age(squarez::web::HighScores::Page page)
{
	switch(page)
	{
		case squarez::web::HighScores::Page::lastDay:
			return 86400;
		case squarez::web::HighScores::Page::lastWeek:
			return 86400 * 7;
		case squarez::web::HighScores::Page::lastMonth:
			return 86400 * 30;
		case squarez::web::HighScores::Page::forever:
			return 0;
	}
	throw std::runtime_error("unreachable");
}
std::string name(squarez::web::HighScores::Page page)
{
	switch(page)
	{
		case squarez::web::HighScores::Page::lastDay:
			return "Last day";
		case squarez::web::HighScores::Page::lastWeek:
			return "Last week";
		case squarez::web::HighScores::Page::lastMonth:
			return "Last month";
		case squarez::web::HighScores::Page::forever:
			return "Forever";
	}
	throw std::runtime_error("unreachable");
}
}

void squarez::web::HighScores::switchPage(int count)
{
	int target = static_cast<int>(_page) + count;
	target = std::min(std::max(target, 0), static_cast<int>(Page::forever));
	_page = static_cast<Page>(target);

	_buttonBefore.set("disabled", true);
	_buttonAfter.set("disabled", true);

	_requestHandle = http::request(_url + squarez::onlineSinglePlayer::GetScores::encodeRequest(age(_page), _numScores),
		[this, count](std::string const & response)
		{
			_buttonBefore.set("disabled", _page == Page::lastDay);
			_buttonAfter.set("disabled", _page == Page::forever);
			if (count == 0)
			{
				_rootElement.call<void>("removeChild", _content);
			}
			else
			{
				emscripten::val oldContent = _content;
				JSCallback * deleteLater = new JSCallback([](emscripten::val){});
				deleteLater->setCallback(
					[this, deleteLater, oldContent](emscripten::val)
					{
						_rootElement.call<void>("removeChild", oldContent);
						delete deleteLater;
					}
				);
				deleteLater->addEventListener(oldContent, "animationend");
				deleteLater->addEventListener(oldContent, "webkitAnimationEnd");
				oldContent["classList"].call<void>("add", emscripten::val(count > 0 ? "discardLeft" : "discardRight"));
			}

			_content = emscripten::val::global("document").call<emscripten::val>("createElement", emscripten::val("table"));
			DeSerializer ser(response);
			std::stringstream innerHTML;
			squarez::onlineSinglePlayer::GetScores res(ser);
			innerHTML << "<thead><tr><th colspan=\"3\">" << name(_page) << "</th></tr></thead>";
			_content.set("innerHTML", innerHTML.str());

			auto tbody = emscripten::val::global("document").call<emscripten::val>("createElement", emscripten::val("tbody"));
			_content.call<void>("appendChild", tbody);
			const char * format = (_page == Page::lastDay) ? "toLocaleTimeString" : "toLocaleDateString";
			emscripten::val formatOptions = emscripten::val::object();
			if (_page == Page::lastDay)
			{
				formatOptions.set("hour", emscripten::val("numeric"));
				formatOptions.set("minute", emscripten::val("numeric"));
			}
			for (auto const & score : res._scores)
			{
				auto tr = emscripten::val::global("document").call<emscripten::val>("createElement", emscripten::val("tr"));
				tbody.call<void>("appendChild", tr);

				auto td = emscripten::val::global("document").call<emscripten::val>("createElement", emscripten::val("td"));
				td["classList"].call<void>("add", emscripten::val("playerName"));
				tr.call<void>("appendChild", td);
				td.set("textContent", utf8_to_utf16(score._playerName));

				td = emscripten::val::global("document").call<emscripten::val>("createElement", emscripten::val("td"));
				emscripten::val date = emscripten::val::global("Date").new_(score._date);
				tr.call<void>("appendChild", td);
				td["classList"].call<void>("add", emscripten::val("date"));
				td.set("textContent", date.call<std::string>(format, emscripten::val::undefined(), formatOptions));

				td = emscripten::val::global("document").call<emscripten::val>("createElement", emscripten::val("td"));
				td["classList"].call<void>("add", emscripten::val("score"));
				tr.call<void>("appendChild", td);
				td.set("textContent", score._score);
			}
			if (count != 0)
				_content["classList"].call<void>("add", emscripten::val(count > 0 ? "appearLeft": "appearRight"));
			_rootElement.call<void>("insertBefore", _content, _buttonAfter);
		},
		[this](){
			_rootElement["style"].set("display", emscripten::val("none"));
		}
	);
}

squarez::web::HighScores::HighScores(emscripten::val rootElement, std::string url):
	_rootElement(rootElement),
	_buttonBefore(emscripten::val::global("document").call<emscripten::val>("createElement", emscripten::val("button"))),
	_content(emscripten::val::global("document").call<emscripten::val>("createElement", emscripten::val("table"))),
	_buttonAfter(emscripten::val::global("document").call<emscripten::val>("createElement", emscripten::val("button"))),
	_before([this](emscripten::val){ switchPage(-1);}),
	_after([this](emscripten::val){ switchPage(1);}),
	_url(url), _numScores(10), _page(Page::lastDay)
{
	_buttonBefore["classList"].call<void>("add", emscripten::val("before"));
	_buttonAfter["classList"].call<void>("add", emscripten::val("after"));
	_rootElement.call<void>("appendChild", _buttonBefore);
	_rootElement.call<void>("appendChild", _content);
	_rootElement.call<void>("appendChild", _buttonAfter);
	switchPage(0);
	_before.addEventListener(_buttonBefore, "click");
	_after.addEventListener(_buttonAfter, "click");
}

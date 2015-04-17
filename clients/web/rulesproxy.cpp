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

#include "rulesproxy.h"

#include "cellproxy.h"
#include "jscallback.h"
#include "selectionproxy.h"

#include "game/constants.h"
#include "rules/singleplayerrules.h"
#include "tutorialrules.h"
#include "network/methods.h"
#include "utils/serializer.h"

#include <sstream>
#include <regex>

#include <emscripten/emscripten.h>

EMSCRIPTEN_BINDINGS(rulesproxy)
{
	emscripten::class_<squarez::web::RulesProxy>("Squarez")
	.constructor<emscripten::val, emscripten::val, emscripten::val, emscripten::val, std::string>()
	.constructor<emscripten::val, emscripten::val, emscripten::val, emscripten::val, std::string, std::string>()
	.constructor<emscripten::val, emscripten::val, emscripten::val, emscripten::val>()
	.function<void>("togglePause", &squarez::web::RulesProxy::togglePause)
	.function<void>("resetSelection", &squarez::web::RulesProxy::resetSelection);
}

squarez::web::RulesProxy::RulesProxy(emscripten::val rootElement, emscripten::val scoreElement, emscripten::val timerElement, emscripten::val messageElement, std::string playerName):
	_scoreElement(scoreElement), _timerElement(timerElement),
	_rootElement(rootElement), _messageElement(messageElement),
	_timerHandler([this](emscripten::val){setTimer(0, _rules->msLeft()+1, "linear");})
{
	_rules.reset(new SinglePlayerRules(*this, constants::default_timer(), playerName));
	gameReady();
}

squarez::web::RulesProxy::RulesProxy(emscripten::val rootElement, emscripten::val scoreElement, emscripten::val timerElement, emscripten::val messageElement, std::string playerName, std::string url):
	_scoreElement(scoreElement), _timerElement(timerElement),
	_rootElement(rootElement), _messageElement(messageElement),
	_timerHandler([this](emscripten::val){setTimer(0, _rules->msLeft()+1, "linear");})
{
	// SinglePlayerRules will think we are in local mode if url is empty, but in fact we want to build a relative url
	if (url.empty())
		url = "./";
	_initHandle = squarez::http::request(url + onlineSinglePlayer::GameInit::encodeRequest(playerName, constants::default_board_size, constants::default_symbols),
		[this, url, playerName](std::string response)
		{
			DeSerializer s(response);
			onlineSinglePlayer::GameInit game(s);
			_rules.reset(new SinglePlayerRules(
				*this,
				constants::default_timer(),
				playerName,
				constants::default_board_size,
				constants::default_symbols,
				game._seed,
				url,
				game._token));
			gameReady();
		},
		[this, playerName]()
		{
			message("Server unreachable\nscore will not be saved");
			_rules.reset(new SinglePlayerRules(*this, constants::default_timer(), playerName));
			gameReady();
		}
	);
}

squarez::web::RulesProxy::RulesProxy(emscripten::val rootElement, emscripten::val scoreElement, emscripten::val timerElement, emscripten::val messageElement):
	_scoreElement(scoreElement), _timerElement(timerElement),
	_rootElement(rootElement), _messageElement(messageElement),
	_timerHandler([this](emscripten::val){setTimer(0, _rules->msLeft()+1, "linear");})
{
	_rules.reset(new TutorialRules(*this, constants::default_board_size, constants::default_symbols));
	gameReady();
}


void squarez::web::RulesProxy::gameReady()
{
	_rootElement["classList"].call<void>("remove", emscripten::val("loading"));
	_timerHandler.addEventListener(_timerElement, "transitionend");
	// Force the transitionend event to be called at least once
	_timerHandler.setTimeout(0, emscripten::val::undefined());
}


squarez::web::RulesProxy::~RulesProxy()
{

}

std::unique_ptr<squarez::Cell::Proxy> squarez::web::RulesProxy::cellProxyFactory(squarez::Cell& cell)
{
	return std::unique_ptr<squarez::Cell::Proxy>(new CellProxy(cell, *this));
}

std::unique_ptr<squarez::VisibleSelection::Proxy> squarez::web::RulesProxy::selectionProxyFactory(squarez::VisibleSelection& selection)
{
	return std::unique_ptr<VisibleSelection::Proxy>(new SelectionProxy(selection, *this));
}

void squarez::web::RulesProxy::timerUpdated()
{
	if (_rules->pause())
	{
		_rootElement["classList"].call<void>("add", emscripten::val("paused"));
		setTimer(_rules->percentageLeft(), 0, "linear");
	}
	else
	{
		_rootElement["classList"].call<void>("remove", emscripten::val("paused"));
		setTimer(_rules->percentageLeft(), 200, "ease-out");
	}

}

void squarez::web::RulesProxy::gameOverChanged(bool status)
{
	_rootElement["classList"].call<void>(status ? "add" : "remove", emscripten::val("gameOver"));
}

void squarez::web::RulesProxy::scoreChanged(unsigned int score)
{
	_scoreElement.set("innerHTML", score);
}

void squarez::web::RulesProxy::networkError()
{
	//FIXME: implement
}

void squarez::web::RulesProxy::message(const std::string& message)
{
	if (message.empty())
		_messageElement["style"].set("display", emscripten::val("none"));
	else
	{
		_messageElement["style"].set("display", emscripten::val(""));
		_messageElement.set("innerHTML",
			std::regex_replace(message, std::regex("\n"), "<br/>"));
	}
}

void squarez::web::RulesProxy::setTimer(float percentage, int duration, std::string animation)
{
	std::stringstream st;
	st << 100 * (1-percentage) << "%";
	_timerElement["style"].set("right", st.str());
	_timerElement["style"].set("top", st.str());

	std::stringstream tr;
	tr << "right " << duration << "ms " << animation << ", top " << duration << "ms " << animation;
	_timerElement["style"].set("transition", tr.str());
	_timerElement["style"].set("webkitTransition", tr.str());
}

void squarez::web::RulesProxy::togglePause()
{
	auto rules = dynamic_cast<SinglePlayerRules *>(_rules.get());
	if (rules)
		rules->setPause(not _rules->pause());
}

void squarez::web::RulesProxy::resetSelection()
{
	_rules->resetSelection();
}
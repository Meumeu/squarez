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

#ifndef SQUAREZ_WEB_RULESPROXY_H
#define SQUAREZ_WEB_RULESPROXY_H

#include "rules/rules.h"
#include "jscallback.h"
#include "httprequest.h"

#include <functional>
#include <memory>

#include <emscripten/bind.h>

namespace squarez {
namespace web {

class RulesProxy : public squarez::Rules::Proxy
{
	std::unique_ptr<squarez::Rules> _rules;
	std::unique_ptr<squarez::http::Handle> _initHandle;
	emscripten::val _scoreElement;
	emscripten::val _timerElement;
public:
	emscripten::val _rootElement;
private:
	emscripten::val _messageElement;
	JSCallback _timerHandler;
public:
	// Constructor for single player game
	RulesProxy(emscripten::val rootElement, emscripten::val scoreElement, emscripten::val timerElement, emscripten::val messageElement, std::string playerName);

	// Constructor for online single player game
	RulesProxy(emscripten::val rootElement, emscripten::val scoreElement, emscripten::val timerElement, emscripten::val messageElement, std::string playerName, std::string url);

	// Constructor for tutorial
	RulesProxy(emscripten::val rootElement, emscripten::val scoreElement, emscripten::val timerElement, emscripten::val messageElement);

	~RulesProxy();
	virtual std::unique_ptr<Cell::Proxy> cellProxyFactory(squarez::Cell& cell) override;
	virtual std::unique_ptr<VisibleSelection::Proxy> selectionProxyFactory(VisibleSelection& selection) override;
	virtual void timerUpdated() override;
	virtual void gameOverChanged(bool status) override;
	virtual void scoreChanged(unsigned int score) override;
	virtual void networkError() override;
	virtual void message(const std::string& message) override;

	void setTimer(float percentage, int duration, std::string animation);

	void togglePause();
	void resetSelection();
private:
	void gameReady();
};
}}

#endif // SQUAREZ_WEB_RULESPROXY_H

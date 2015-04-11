/*
 * Squarez puzzle game
 * Copyright (C) 2015  Guillaume Meunier <guillaume.meunier@centraliens.net>
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
#include "cellproxy.h"
#include "rulesproxy.h"

namespace squarez {
namespace qt {

SelectionProxy::SelectionProxy(VisibleSelection& selection, RulesProxy& rules): Proxy(selection), _rules(rules)
{
}

void SelectionProxy::stateChanged(Selection::State state)
{
	switch(state)
	{
		case Selection::State::invalid:
		case Selection::State::validated:
			break;
	}
}

SelectionProxy::~SelectionProxy()
{
	
}


}
}
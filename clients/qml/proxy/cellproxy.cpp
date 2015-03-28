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

#include "cellproxy.h"
#include "rulesproxy.h"


namespace squarez {
namespace qt {

CellProxy::CellProxy(Cell& owner, RulesProxy& rules): QObject(), Proxy(owner), _rules(rules)
{
}

CellProxy::~CellProxy()
{
	_rules.removeCell(this);
}

void CellProxy::moved(int x, int y)
{
	emit onXChanged(x);
	emit onYChanged(y);
}

void CellProxy::selectChanged(bool status)
{
	emit onSelectedChanged(status);
}

void CellProxy::clicked()
{
	owner.click();
}

}
}

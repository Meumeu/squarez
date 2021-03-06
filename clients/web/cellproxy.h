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

#ifndef SQUAREZ_WEB_CELLPROXY_H
#define SQUAREZ_WEB_CELLPROXY_H

#include "game/cell.h"
#include "jscallback.h"

#include <emscripten/bind.h>

namespace squarez {

namespace web {

class RulesProxy;

class CellProxy : public squarez::Cell::Proxy
{
private:
	RulesProxy & _rules;
	emscripten::val _element;
	JSCallback _clickHandler;
	JSCallback _moveHandler;

	void setXY();
public:
	CellProxy(squarez::Cell & owner, RulesProxy & rules);
	virtual ~CellProxy();

	virtual void moved(int x, int y) override;
	virtual void selectChanged(bool status) override;
};

}
}

#endif // SQUAREZ_WEB_CELLPROXY_H

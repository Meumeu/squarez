/*
 * Squarez puzzle game
 * Copyright (C) 2015  Guillaume Meunier <guillaume.meunier@centraliens.net>
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

#include "cell.h"
#include <rules/rules.h>

bool squarez::Cell::Compare::operator() (const squarez::Cell& left, const squarez::Cell& right) const
{
	if (left._x < right._x) return true;
	if (left._x > right._x) return false;
	return left._y < right._y;
}

void squarez::Cell::move(int x, int y)
{
	_x = x;
	_y = y;
	_proxy->moved(x, y);
}

void squarez::Cell::click()
{
	_rules.onClick(*this);
}

void squarez::Cell::setSelected(bool status)
{
	if (_selected == status)
		return;
	_selected = status;
	_proxy->selectChanged(_selected);
}

squarez::Cell::Proxy::~Proxy()
{
}

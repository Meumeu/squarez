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

#ifndef SQUAREZ_QT_CELLPROXY_H
#define SQUAREZ_QT_CELLPROXY_H

#include <QObject>

#include <memory>

#include "game/cell.h"

namespace squarez {
namespace qt {

class RulesProxy;

class CellProxy : public QObject, public Cell::Proxy
{
	Q_OBJECT

	RulesProxy& _rules;

public:
	Q_PROPERTY(int x READ x NOTIFY onXChanged)
	Q_PROPERTY(int y READ y NOTIFY onYChanged)
	Q_PROPERTY(unsigned int symbol READ symbol CONSTANT)
	Q_PROPERTY(bool selected READ isSelected NOTIFY onSelectedChanged)
	
	CellProxy(Cell& owner, RulesProxy& rules);
	~CellProxy();
	virtual void moved(int x, int y) override;
	virtual void selectChanged(bool status) override;

	int x() const { return _owner.x(); }
	int y() const { return _owner.y(); }
	unsigned int symbol() const { return _owner.symbol; }
	bool isSelected() const { return _owner.selected(); }

signals:
	void onXChanged(int x);
	void onYChanged(int y);
	void onSelectedChanged(bool isSelected);
	
public slots:
	void clicked();
};
}
}

#endif // SQUAREZ_QT_CELLPROXY_H

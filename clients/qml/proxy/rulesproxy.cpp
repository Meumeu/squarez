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

#include "rulesproxy.h"
#include "cellproxy.h"
#include "game/constants.h"
#include "rules/singleplayerrules.h"
#include <assert.h>

namespace squarez {
namespace qt {

RulesProxy::RulesProxy(QObject *parent) :
	QAbstractListModel(parent)
{
}

RulesProxy::~RulesProxy()
{
}

void RulesProxy::scoreChanged(unsigned int score)
{
	emit onScoreChanged(score);
}

void RulesProxy::gameOverChanged(bool status)
{
	emit onGameOverChanged(status);
}

void RulesProxy::timerUpdated()
{
	emit onTimerUpdated();
}

void RulesProxy::nameRequired()
{
	emit onNameRequired();
}

void RulesProxy::animateSquare(std::array<Cell *, 4>)
{
// FIXME: not implemented
}

void RulesProxy::resetSelection()
{
	if (_rules)
		_rules->resetSelection();
}


std::unique_ptr<Cell::Proxy> RulesProxy::cellProxyFactory(Cell & cell)
{
	auto cellproxy = std::unique_ptr<CellProxy>(new CellProxy(cell, *this));
	beginInsertRows(QModelIndex(), _cells.count(), _cells.count());
	_cells.push_back(cellproxy.get());
	endInsertRows();
	return std::move(cellproxy);
}

void RulesProxy::removeCell(CellProxy* cell)
{
	int idx = _cells.indexOf(cell);
	assert(idx != -1);
	
	beginRemoveRows(QModelIndex(), idx, idx);
	_cells.removeAt(idx);
	endRemoveRows();
}

void RulesProxy::setType(QString type)
{
	if (type == "singlePlayer")
	{
		_rules = std::unique_ptr<Rules>(new SinglePlayerRules(*this, constants::default_timer()));
		if (not _playerName.isEmpty())
			_rules->setPlayerName(_playerName.toStdString());
		_type = type;
		emit onTypeChanged(_type);
	}
}

void RulesProxy::setPlayerName(QString playerName)
{
	_playerName = playerName;
	if (_rules)
		_rules->setPlayerName(playerName.toStdString());
	emit onPlayerNameChanged(playerName);
}

QVariant RulesProxy::data(const QModelIndex& index, int /*role*/) const
{
	return QVariant::fromValue(_cells.at(index.row()));
}

QHash<int, QByteArray> RulesProxy::roleNames() const
{
	QHash<int, QByteArray> roles;
	roles[0] = "cell";
	return roles;
}

int RulesProxy::rowCount(const QModelIndex&) const
{
	return _cells.count();
}


} // namespace qt
} // namespace squarez

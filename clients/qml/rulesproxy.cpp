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
#include "../tutorialrules.h"
#include "network/methods.h"
#include "utils/serializer.h"
#include "selectionproxy.h"
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
	emit onPausedChanged(paused());
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

std::unique_ptr<VisibleSelection::Proxy> RulesProxy::selectionProxyFactory (VisibleSelection& selection)
{
	return std::unique_ptr<VisibleSelection::Proxy>(new SelectionProxy(selection, *this));
}

void RulesProxy::removeCell(CellProxy* cell)
{
	int idx = _cells.indexOf(cell);
	assert(idx != -1);
	
	beginRemoveRows(QModelIndex(), idx, idx);
	_cells.removeAt(idx);
	endRemoveRows();
}

void RulesProxy::tryStartGame()
{
	if (_rules)
		return;

	if (_type == "singlePlayer" and !_playerName.isEmpty())
	{
		_rules = std::unique_ptr<Rules>(new SinglePlayerRules(
			*this,
			constants::default_timer(),
			_playerName.toStdString(),
			constants::default_board_size,
			constants::default_symbols));
		emit onReadyChanged(true);
	}
	else if (_type == "onlineSinglePlayer" and !_url.isEmpty() and !_playerName.isEmpty())
	{
		_gameInitHandle = squarez::http::request(_url.toStdString() + onlineSinglePlayer::GameInit::encodeRequest(_playerName.toStdString(), constants::default_board_size, constants::default_symbols),
			[this](std::string response) // onload
			{
				DeSerializer s(response);
				onlineSinglePlayer::GameInit game(s);
				_rules = std::unique_ptr<Rules>(new SinglePlayerRules(
					*this,
					constants::default_timer(),
					_playerName.toStdString(),
					constants::default_board_size,
					constants::default_symbols,
					game._seed,
					_url.toStdString(),
					game._token));
				emit onReadyChanged(true);
			},
			[this]() // onerror
			{
				_type = "singlePlayer";
				emit onTypeChanged(_type);

				networkError();

				_rules = std::unique_ptr<Rules>(new SinglePlayerRules(
					*this,
					constants::default_timer(),
					_playerName.toStdString(),
					constants::default_board_size,
					constants::default_symbols));
				emit onReadyChanged(true);
			}
		);
	}
	else if (_type == "tutorial")
	{
		_rules = std::unique_ptr<Rules>(new TutorialRules(*this,
			constants::default_board_size,
			constants::default_symbols));
		emit onReadyChanged(true);
	}
}

void RulesProxy::setType(QString type)
{
	if (_rules)
		return;

	if (type == "singlePlayer" or type == "onlineSinglePlayer" or type == "tutorial")
	{
		_type = type;
		emit onTypeChanged(_type);
	}

	tryStartGame();
}

void RulesProxy::setPlayerName(QString playerName)
{
	_playerName = playerName;
	emit onPlayerNameChanged(playerName);

	tryStartGame();
}

void RulesProxy::setUrl(QString url)
{
	if (_rules)
		return;

	if (not url.endsWith('/'))
		url += '/';

	_url = url;
	emit onUrlChanged(url);

	tryStartGame();
}

void RulesProxy::networkError()
{
	emit onNetworkError();
}

void RulesProxy::message (const std::string& message)
{
	_message = QString::fromStdString(message);
	emit onMessageChanged(_message);
}

void RulesProxy::setPaused(bool paused)
{
	if (!_rules)
		return;
	
	auto rules = dynamic_cast<SinglePlayerRules*>(_rules.get());
	if (rules) rules->setPause(paused);
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

void RulesProxy::next()
{
	if (!_cells.empty())
		_cells.front()->clicked();
}


}
}

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

#include "highscores.h"
#include "utils/serializer.h"

#include <QDateTime>

namespace squarez {
namespace qt {

HighScores::~HighScores()
{
}

HighScores::HighScores(QObject *parent) : QAbstractListModel(parent), _loading(true), _updateAllowed(true), _scoresToBeUpdated(false)
{
	connect(&_reloadTimer, &QTimer::timeout, this, &HighScores::refresh);
	_reloadTimer.setSingleShot(true);
	_reloadTimer.setInterval(5000);
}

QVariant HighScores::data(const QModelIndex& index, int role) const
{
	const squarez::onlineSinglePlayer::GetScores::Score& score = _scores.at(index.row());
	QDateTime date;
	
	switch(role)
	{
	case 0:
		return QString::fromStdString(score._playerName);
	case 1:
		return score._score;
	case 2:
		date = QDateTime:: fromString(QString::fromStdString(score._date), Qt::ISODate);
		date.setTimeSpec(Qt::UTC);
		return date;
	default:
		return QVariant();
	}
}

int HighScores::rowCount(const QModelIndex& /*parent*/) const
{
	return _scores.size();
}

QHash<int, QByteArray> HighScores::roleNames() const
{
	QHash<int, QByteArray> roles;
	roles[0] = "playerName";
	roles[1] = "score";
	roles[2] = "date";
	return roles;
}

void HighScores::refresh()
{
	if (!_url.isEmpty())
	{
		_loading = true;
		emit onLoadingChanged(_loading);

		_loadHandle = squarez::http::request(_url.toStdString() + onlineSinglePlayer::GetScores::encodeRequest(_minDate.toTime_t(), _maxDate.toTime_t(), 10),
			[this](std::string response) // onload
			{
				DeSerializer s(response);
				onlineSinglePlayer::GetScores scores(s);

				_newScores = scores._scores;
				_scoresToBeUpdated = true;

				updateIfAllowed();
			},
			[this]() // onerror
			{
				emit onNetworkError();

				_newScores.clear();
				_scoresToBeUpdated = true;

				updateIfAllowed();
			}
		);
	}
}

void HighScores::setUrl(QString url)
{
	if (not url.endsWith('/'))
		url += '/';
	_url = url;

	emit onUrlChanged(url);

	_reloadTimer.start();
}

void HighScores::setMinDate(QDateTime minDate)
{
	_minDate = minDate;
	emit onMinDateChanged(_minDate);

	_reloadTimer.start();
}

void HighScores::setMaxDate(QDateTime maxDate)
{
	_maxDate = maxDate;
	emit onMaxDateChanged(_maxDate);

	_reloadTimer.start();
}

void HighScores::updateIfAllowed()
{
	if (_updateAllowed && _scoresToBeUpdated)
	{
		beginResetModel();
		_scores = _newScores;
		endResetModel();
		_scoresToBeUpdated = false;

		_loading = false;
		emit onLoadingChanged(_loading);
	}
}

void HighScores::setUpdateAllowed(bool updateAllowed)
{
	if (updateAllowed == _updateAllowed)
		return;

	_updateAllowed = updateAllowed;
	emit onUpdateAllowedChanged(updateAllowed);

	updateIfAllowed();
}

void HighScores::classBegin() {}


void HighScores::componentComplete()
{
	beginResetModel();
	_scores.clear();
	endResetModel();

	refresh();
}

}
}

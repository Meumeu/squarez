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
		date = QDateTime::fromString(QString::fromStdString(score._date), "yyyy-MM-dd HH:mm:ss");
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


void HighScores::setUrl(QString url)
{
	if (not url.endsWith('/'))
		url += '/';
	_url = url;

	beginResetModel();
	_scores.clear();
	endResetModel();
	
	if (!_url.isEmpty())
	{
		_scoresLoadHandle = squarez::http::request(_url.toStdString() + onlineSinglePlayer::GetScores::encodeRequest(),
			[this](std::string response) // onload
			{
				DeSerializer s(response);
				onlineSinglePlayer::GetScores scores(s);
				beginResetModel();
				_scores = scores._scores;
				std::sort(_scores.begin(), _scores.end(), [](Score& a, Score& b){ return a._score > b._score; });
				endResetModel();
			},
			[this]() // onerror
			{
				emit onNetworkError();
			}
		);
	}

	emit onUrlChanged(url);
}

}
}

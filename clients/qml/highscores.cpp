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

HighScores::section::section(int maxAge, std::string name) : maxAge(maxAge), name(name)
{}

HighScores::section::section(const section& rhs) : maxAge(rhs.maxAge), name(rhs.name)
{}

HighScores::HighScores(QObject *parent) : QAbstractListModel(parent), _scores{{7 * 86400, "Last week"}, {30 * 86400, "Last month"}, {0, "All time"} }
{
	std::cerr << "HighScores::HighScores" << std::endl;
	for(auto& i: _scores)
	{
		std::cerr << "    maxAge: " << i.maxAge << ", name: " << i.name << std::endl;
	}
}

QVariant HighScores::data(const QModelIndex& index, int role) const
{
	size_t idx1 = 0;
	size_t idx2 = index.row();
	while(idx1 < _scores.size() && idx2 >= _scores[idx1].scores.size())
	{
		idx2 -= _scores[idx1].scores.size();
		idx1++;
	}

	const squarez::onlineSinglePlayer::GetScores::Score& score = _scores.at(idx1).scores[idx2];
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
	case 3:
		return QString::fromStdString(_scores.at(idx1).name);
	default:
		return QVariant();
	}
}

int HighScores::rowCount(const QModelIndex& /*parent*/) const
{
	int size = 0;
	for(const auto& i: _scores)
		size += i.scores.size();

	return size;
}

QHash<int, QByteArray> HighScores::roleNames() const
{
	QHash<int, QByteArray> roles;
	roles[0] = "playerName";
	roles[1] = "score";
	roles[2] = "date";
	roles[3] = "section";
	return roles;
}

void HighScores::refresh()
{
	if (!_url.isEmpty())
	{
		for(auto& i: _scores)
		{
			i.loadHandle = squarez::http::request(_url.toStdString() + onlineSinglePlayer::GetScores::encodeRequest(i.maxAge, 5),
				[this, &i](std::string response) // onload
				{
					DeSerializer s(response);
					onlineSinglePlayer::GetScores scores(s);
					beginResetModel();
					i.scores = scores._scores;
					std::sort(i.scores.begin(), i.scores.end(), [](const Score& a, const Score& b){ return a._score > b._score; });
					endResetModel();
				},
				[this]() // onerror
				{
					emit onNetworkError();
				}
			);
		}
	}
}

void HighScores::setUrl(QString url)
{
	if (not url.endsWith('/'))
		url += '/';
	_url = url;

	beginResetModel();
	for(auto& i: _scores)
		i.scores.clear();
	endResetModel();
	
	refresh();

	emit onUrlChanged(url);
}

}
}

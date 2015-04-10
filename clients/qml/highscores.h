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

#ifndef SQUAREZ_HIGHSCORES_H
#define SQUAREZ_HIGHSCORES_H

#include <QAbstractListModel>
#include "httprequest.h"
#include "network/methods.h"

namespace squarez {
namespace qt {

class HighScores : public QAbstractListModel
{
	Q_OBJECT

	typedef squarez::onlineSinglePlayer::GetScores::Score Score;
	QString _url;
	std::unique_ptr<squarez::http::Handle> _scoresLoadHandle;
	std::vector<Score> _scores;

public:
	Q_PROPERTY(QString url READ url WRITE setUrl NOTIFY onUrlChanged)

	~HighScores();

	QVariant data(const QModelIndex& index, int role) const override;
	int rowCount(const QModelIndex& parent) const override;
	QHash<int, QByteArray> roleNames() const override;

	const QString& url() const { return _url; }
	void setUrl(QString url);

signals:
	void onUrlChanged(QString url);
	void onNetworkError();
};
}
}

#endif // SQUAREZ_HIGHSCORES_H

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
#include <QQmlParserStatus>
#include <QDateTime>
#include <QTimer>
#include "httprequest.h"
#include "network/methods.h"

namespace squarez {
namespace qt {

class HighScores : public QAbstractListModel, public QQmlParserStatus
{
	Q_OBJECT
	Q_INTERFACES(QQmlParserStatus)

	typedef squarez::onlineSinglePlayer::GetScores::Score Score;
	QString _url;
	bool _loading;
	QDateTime _minDate;
	QDateTime _maxDate;
	bool _updateAllowed;

	bool _scoresToBeUpdated;
	std::vector<Score> _scores;
	std::vector<Score> _newScores;
	std::unique_ptr<squarez::http::Handle> _loadHandle;

	QTimer _reloadTimer;

	void updateIfAllowed();

public:
	Q_PROPERTY(QString url READ url WRITE setUrl NOTIFY onUrlChanged)
	Q_PROPERTY(QDateTime minDate READ minDate WRITE setMinDate NOTIFY onMinDateChanged)
	Q_PROPERTY(QDateTime maxDate READ maxDate WRITE setMaxDate NOTIFY onMaxDateChanged)
	Q_PROPERTY(bool loading READ loading NOTIFY onLoadingChanged)
	Q_PROPERTY(bool updateAllowed READ updateAllowed WRITE setUpdateAllowed NOTIFY onUpdateAllowedChanged)
	Q_PROPERTY(int count READ count NOTIFY onCountChanged)

	explicit HighScores(QObject* parent = nullptr);
	~HighScores();

	QVariant data(const QModelIndex& index, int role) const override;
	int rowCount(const QModelIndex& parent) const override;
	QHash<int, QByteArray> roleNames() const override;

	const QString& url() const { return _url; }
	bool loading() const { return _loading; }
	QDateTime minDate() const { return _minDate; }
	QDateTime maxDate() const { return _maxDate; }
	bool updateAllowed() const { return _updateAllowed; }
	int count() const { return _scores.size(); }

	void setUrl(QString url);
	void setMinDate(QDateTime minDate);
	void setMaxDate(QDateTime maxDate);
	void setUpdateAllowed(bool updateAllowed);

	Q_INVOKABLE void refresh();

	// Implementation of QQmlParserStatus
	void classBegin();
	void componentComplete();

signals:
	void onUrlChanged(QString url);
	void onNetworkError();
	void onLoadingChanged(bool loading);
	void onMinDateChanged(QDateTime minDate);
	void onMaxDateChanged(QDateTime maxDate);
	void onUpdateAllowedChanged(bool updateAllowed);
	void onCountChanged(int count);
};
}
}

#endif // SQUAREZ_HIGHSCORES_H

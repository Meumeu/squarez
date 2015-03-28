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

#ifndef SQUAREZ_QT_RULESPROXY_H
#define SQUAREZ_QT_RULESPROXY_H

#include <QAbstractListModel>

#include <memory>

#include "rules/rules.h"

namespace squarez {
namespace qt {

class CellProxy;

class RulesProxy : public QAbstractListModel, public Rules::Proxy
{
	Q_OBJECT

	// _cells has to be before _rules because the destructor of _rules calls removeCell() so _cells be destroyed last
	QList<CellProxy *> _cells;
	std::unique_ptr<Rules> _rules;
	
	QString _type;
	QString _playerName;
	

public:
	explicit RulesProxy(QObject *parent = 0);

	Q_PROPERTY(unsigned int score READ score NOTIFY onScoreChanged)
	Q_PROPERTY(bool gameOver READ gameOver NOTIFY onGameOverChanged)
	Q_PROPERTY(QString type READ type WRITE setType NOTIFY onTypeChanged)
	Q_PROPERTY(QString playerName READ playerName WRITE setPlayerName NOTIFY onPlayerNameChanged)
	Q_PROPERTY(int boardSize READ boardSize NOTIFY onBoardSizeChanged)
	
	virtual ~RulesProxy();
	virtual void scoreChanged(unsigned int score);
	virtual void gameOverChanged(bool status);
	virtual void timerUpdated();
	virtual void nameRequired();
	virtual void animateSquare(std::array<Cell *, 4>);

	virtual std::unique_ptr<Cell::Proxy> cellProxyFactory(Cell & cell);

	unsigned int score() const { return _rules ? _rules->score() : 0; }
	bool gameOver() const { return _rules ? _rules->gameOver() : false; }
	QString type() const { return _type; }
	QString playerName() const { return _playerName; }
	int boardSize() const { return 8; } // FIXME

	void setType(QString type);
	void setPlayerName(QString playerName);

	// QAbstractListModel implementation
	int rowCount(const QModelIndex &) const override;
	QVariant data(const QModelIndex &index, int role) const override;
	QHash<int, QByteArray> roleNames() const override;

	void removeCell(CellProxy * cell);

signals:
	void onScoreChanged(unsigned int score);
	void onGameOverChanged(bool gameOver);
	void onTypeChanged(QString type);
	void onPlayerNameChanged(QString playerName);
	void onTimerUpdated();
	void onNameRequired();
	void onBoardSizeChanged(int boardSize);

public slots:
	void resetSelection();
};

} // namespace qt
} // namespace squarez

#endif // SQUAREZ_QT_RULESPROXY_H

/*
 * Squarez puzzle game
 * Copyright (C) 2013  Guillaume Meunier <guillaume.meunier@centraliens.net>
 * Copyright (C) 2013  Patrick Nicolas <patricknicolas@laposte.net>
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

#ifndef SQUAREZ_RULES_H
#define SQUAREZ_RULES_H

#include <chrono>
#include <string>
#include "shared/gameboard.h"

#ifdef SQUAREZ_QT
#include <QObject>
#include <QList>
#include <QPoint>
#include "qt/cell.h"

namespace squarez {
class Score;
class Selection;
class Transition;
}
#else
namespace squarez {
class UI;
}
#endif

namespace squarez {

class Timer;

class Rules
#ifdef SQUAREZ_QT
		:public QObject
{
	Q_OBJECT
	Q_PROPERTY(QQmlListProperty<squarez::qt::Cell> board READ getBoardModel)
signals:
	void transition(const Transition & transition);
	void scoreChanged(int new_score);
	void scoreListChanged(std::vector<Score> const& scores);
	void selectionAccepted(Selection const& selection);
	void selectionRejected(Selection const& selection);
	void message(std::string const& message);
	void nameRequired(std::string const& previousName);

public slots:
	void select(const QList<QPoint>&);

public:
	QQmlListProperty<qt::Cell> getBoardModel();

#else
{
	friend class UI;
protected:
	UI * _ui;
	virtual void setUI(UI * ui) { _ui = ui;}
#endif

protected:
	GameBoard board;
	std::string _playerName;

public:
	virtual void onSelect(Selection const& selection) = 0;
	virtual bool gameOver() = 0;
	virtual Timer const& getTimer() const = 0;
	GameBoard const& getBoard() const { return board; }
	virtual void setPlayerName(std::string const& name) = 0;

	Rules(int board_size, int nb_symbols, std::string const& name = "");
	Rules(GameBoard const& board, std::string const& name = "");
	
	virtual ~Rules() {}
};
}

#endif // SQUAREZ_RULES_H

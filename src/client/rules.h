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
#include <memory>
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
	Q_PROPERTY(squarez::GameBoard* board READ getBoard CONSTANT)
	Q_PROPERTY(float percentageLeft READ getPercentageLeft)
	Q_PROPERTY(unsigned int score READ getScore NOTIFY scoreChanged)
	Q_PROPERTY(bool gameOver READ gameOver NOTIFY gameOverChanged)
	Q_PROPERTY(QString playerName READ playerName WRITE setPlayerName)
signals:
	void gameOverChanged(bool);
	void scoreChanged(unsigned int score);
	void selectionAccepted(const QVariantList& selection);
	void selectionRejected(const QVariantList& selection);
	void message(std::string const& message);
	void nameRequired(QString previousName);

public slots:
	void select(const QVariantList &);

public:
	float getPercentageLeft();
	bool gameOver() const {return _gameOver;}

	QString playerName() const {return QString::fromStdString(_playerName);}
	void setPlayerName(QString name);

#else
{
	friend class UI;
protected:
	UI * _ui;
	virtual void setUI(UI * ui) { _ui = ui;}
#endif
private:
	unsigned int _score;
	bool _gameOver;

protected:
	std::unique_ptr<GameBoard> _board;
	std::string _playerName;
	void setScore(unsigned int score);
	void setGameOver(bool status);

public:
	virtual void onSelect(Selection const& selection) = 0;
	virtual Timer const& getTimer() = 0;
	unsigned int getScore() const {return _score;}
	GameBoard const* getBoard() const { return _board.get(); }
	GameBoard * getBoard() { return _board.get(); }
	virtual void setPlayerName(std::string const& name) = 0;

	Rules(int board_size, int nb_symbols, std::string name = "");
	Rules(std::unique_ptr<GameBoard> && _board, std::string name = "");
	
	virtual ~Rules() {}
};
}

#endif // SQUAREZ_RULES_H

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
#include <random>
#include <string>
#include "shared/gameboard.h"
#include "shared/timer.h"

#ifdef SQUAREZ_QT
#include <QObject>
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
	Q_PROPERTY(squarez::GameBoard* board READ getBoard NOTIFY boardChanged)
	Q_PROPERTY(float percentageLeft READ getPercentageLeft)
	Q_PROPERTY(unsigned int score READ getScore NOTIFY scoreChanged)
	Q_PROPERTY(bool gameOver READ gameOver NOTIFY gameOverChanged)
	Q_PROPERTY(QString playerName READ playerName WRITE setPlayerName NOTIFY playerNameChanged)
	Q_PROPERTY(bool pause READ pause WRITE setPause NOTIFY pauseChanged)
signals:
	void boardChanged(GameBoard * board);
	void gameOverChanged(bool);
	void scoreChanged(unsigned int score);
	// Selection is valid and should be displayed on screen
	void selectionAccepted(const QVariantList& selection);
	// Previously valid selection should be removed
	void selectionRejected(const QVariantList& selection);
	// Previously valid or new selection has been accepted
	void selectionApplied(const QVariantList& selection);
	void message(QString message);
	void nameRequired(QString previousName);
	void pauseChanged(bool);
	void playerNameChanged(QString);

public slots:
	void select(const QVariantList &);

public:
	QString playerName() const {return QString::fromStdString(_playerName);}
	void setPlayerName(QString name);

	virtual void setPause(bool) {}

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
	Timer _timer;

	void notifyTimer();

protected:
	std::mt19937 _random_generator;
	std::unique_ptr<GameBoard> _board;
	std::string _playerName;
	void setScore(unsigned int score);
	void setGameOver(bool status);
	bool getGameOver() const { return _gameOver;}

	void acceptSelection(const Selection & selection);
	void rejectSelection(const Selection & selection);
	void applySelection(const Selection & selection);
	void applyTransition(const Transition & transition);

	void pauseTimer(bool pause);
	void refillTimer(unsigned int percentage);
	void setTimer(Timer && timer);

	Rules(int board_size, int nb_symbols, std::uint_fast32_t random_seed, Timer timer, std::string name = "");
	Rules(std::unique_ptr<GameBoard> && _board, std::uint_fast32_t random_seed, Timer timer, std::string name = "");

public:
	bool pause() const {return _timer.paused();}
	float getPercentageLeft(float offset = 0);
	int msLeft() { return _timer.msLeft();}
	virtual bool gameOver() {return _gameOver;}
	virtual void onSelect(Selection const& selection) = 0;
	unsigned int getScore() const {return _score;}
	GameBoard const* getBoard() const { return _board.get(); }
	GameBoard * getBoard() { return _board.get(); }
	virtual void setPlayerName(std::string const& name) = 0;
	
	virtual ~Rules() {}
};
}

#endif // SQUAREZ_RULES_H

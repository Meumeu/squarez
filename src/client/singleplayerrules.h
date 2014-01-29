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

#ifndef SQUAREZ_SINGLEPLAYERRULES_H
#define SQUAREZ_SINGLEPLAYERRULES_H

#include "rules.h"
#include "shared/timer.h"

namespace squarez {

class HighScores;

class SinglePlayerRules : public squarez::Rules
{
#ifdef SQUAREZ_QT
	Q_OBJECT
	Q_PROPERTY(bool pause READ pause WRITE setPause NOTIFY pauseChanged)
	Q_PROPERTY(squarez::HighScores * highScores READ getHighScores CONSTANT)
signals:
	void pauseChanged(bool);
#endif
private:
	Timer timer;
	bool _scoreSaved;

	static HighScores & accessHighScores();
	bool checkGameOver();
	void saveScore();

#ifndef SQUAREZ_QT
protected:
	virtual void setUI(UI * ui);
#endif
	
public:
	SinglePlayerRules(int board_size = 8, int nb_symbols = 3, int long_term = 10, int short_term = 60, int duration = 180);
	
	virtual void onSelect(Selection const& selection);
	virtual Timer const& getTimer();
	virtual void setPlayerName(std::string const& name);

	HighScores * getHighScores() {return &accessHighScores();}
	
	bool pause() const;
	void setPause(bool state);
};
}

#endif // SQUAREZ_SINGLEPLAYERRULES_H

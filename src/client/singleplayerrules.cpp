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

#include "singleplayerrules.h"
#include "client/ui.h"
#include "shared/timer.h"

squarez::SinglePlayerRules::SinglePlayerRules(int board_size, int nb_symbols, int long_term, int short_term, int duration) :
	Rules(board_size, nb_symbols),
	timer(std::chrono::seconds(long_term), std::chrono::seconds(short_term), std::chrono::seconds(duration)),
	score(0)
{
}

bool squarez::SinglePlayerRules::gameOver() const
{
	return timer.percentageLeft() == 0;
}

void squarez::SinglePlayerRules::onSelect(const squarez::Selection& selection)
{
	Transition tr = board.selectSquare(selection, false);
	if (tr._score)
	{
		score += tr._score;
		timer.refill(tr._score * 2);
		
		ui->onScoreChanged(score);
		ui->onTransition(tr);
		
		board.applyTransition(tr);
	}
}

void squarez::SinglePlayerRules::pause()
{
	timer.pause();
}

void squarez::SinglePlayerRules::unpause()
{
	timer.unPause();
}

const squarez::Timer& squarez::SinglePlayerRules::getTimer() const
{
	return timer;
}


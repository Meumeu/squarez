/*
 * Squarez puzzle game
 * Copyright (C) 2013  Guillaume Meunier <guillaume.meunier@centraliens.net>
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

#include "rules.h"
#include "shared/selection.h"

squarez::Rules::Rules(int board_size, int nb_symbols, std::string const& name) : board(board_size, nb_symbols), _playerName(name)
{

}

squarez::Rules::Rules(const squarez::GameBoard& board, std::string const& name) : board(board), _playerName(name)
{

}

#ifdef SQUAREZ_QT
QQmlListProperty<squarez::qt::Cell> squarez::Rules::getBoardModel()
{
	return board.getModel(this);
}


void squarez::Rules::select(const QList<QPoint> & qSelection)
{
	squarez::Selection selection;
	for (const auto & point : qSelection)
		selection.addPoint(point.x(), point.y());

	this->onSelect(selection);
}

#endif

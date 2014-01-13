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

squarez::Rules::Rules(int board_size, int nb_symbols, std::string name) :
	_board(new squarez::GameBoard(board_size, nb_symbols)), _playerName(name)
{

}

squarez::Rules::Rules(std::unique_ptr<GameBoard> &&board, std::string name) :
	_board(std::move(board)), _playerName(name)
{

}

#ifdef SQUAREZ_QT

void squarez::Rules::select(const QVariantList & qSelection)
{
	squarez::Selection selection;
	for (const QVariant & pointVariant : qSelection)
	{
		QPoint point = pointVariant.toPoint();
		selection.addPoint(point.x(), point.y());
	}

	this->onSelect(selection);
}

#endif

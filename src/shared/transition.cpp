#include "transition.h"

#include "gameboard.h"

#include <cstdlib>

namespace squarez
{

Transition::Transition(const GameBoard& board): _score(0), _direction(board.direction()), _size(board.size())
{
	_cells.resize(_size * _size);
}

Transition::Transition(const GameBoard& board, const Selection& selection, uint32_t score): _score(score), _direction(board.direction()), _size(board.size())
{
	_cells.resize(_size * _size);
	
	// Calculate the cell movement for the transition
	int xinc = 0, yinc = 0;
	switch (_direction)
	{
		case up:
			yinc = 1;
			break;
		case down:
			yinc = -1;
			break;
		case left:
			xinc = -1;
			break;
		case right:
			xinc = 1;
			break;
	}
	
	for (auto point: selection.getPoints())
	{
		uint8_t x = point.first;
		uint8_t y = point.second;
		this->remove(x, y);
		x -= xinc;
		y -= yinc;
		while (x > 0 and x < _size - 1 and y > 0 and y < _size - 1)
		{
			this->move(x, y);
			x -= xinc;
			y -= yinc;
		}
		
		bool doubleMove = false;
		for (auto cell: _newCells)
		{
			doubleMove |= cell._x == x or cell._y == y;
		}
		
		uint8_t symbol = std::rand() % board.symbol();
		if (doubleMove)
			_newCells.push_back(NewCell(x + xinc, y + yinc, true, symbol));
		else
			_newCells.push_back(NewCell(x, y, false, symbol));
	}
}

void Transition::move(uint8_t x, uint8_t y)
{
	++_cells.at(x * _size + y)._move;
}
void Transition::remove(uint8_t x, uint8_t y)
{
	_cells.at(x * _size + y)._removed = true;
}

}
#include "transition.h"

#include "gameboard.h"

#include <cstdlib>
#include <set>

namespace squarez
{

Transition::Transition(const GameBoard& board): _doubleMove(false), _score(0), _size(board.size())
{
	_cells.resize(_size * _size);
}

Transition::Transition(const GameBoard& board, const Selection& selection, uint32_t score): _score(score), _size(board.size())
{
	_cells.resize(_size * _size);
	
	// Check if square is aligned with grid, in which case new cells will need double move set to true
	_doubleMove = false;
	std::set<uint8_t> targets;
	
	for (auto point: selection.getPoints())
	{
		uint8_t x = point.first;
		uint8_t y = point.second;
		this->remove(x, y);
		while (y > 0)
		{
			this->move(x, --y);
		}
		
		uint8_t symbol = std::rand() % board.symbol();
		if (targets.find(x) != targets.end())
		{
			y = 1;
			_doubleMove = true;
		}
		_newCells.push_back(NewCell(x, y, symbol));
		targets.insert(x);
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
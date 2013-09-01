#ifndef TRANSITION_H
#define TRANSITION_H

#include <stdint.h>
#include <vector>

namespace squarez
{

typedef enum
{
	up, right, down, left
} direction_t;
	
class GameBoard;
class Selection;

class Transition
{
public:
	struct CellTransition
	{
		CellTransition(): _move(0), _removed(false) {}
		uint8_t _move;
		bool _removed;
	};
	struct NewCell
	{
		uint8_t _x; // target position
		uint8_t _y;
		bool _doubleMove;
		uint8_t _symbol;
		NewCell(uint8_t x, uint8_t y,bool doubleMove, uint8_t symbol):
		_x(x), _y(y), _doubleMove(doubleMove), _symbol(symbol) {}
	};
	Transition(GameBoard const& board);
	Transition(GameBoard const& board, Selection const& selection, uint32_t score);
	
	CellTransition operator() (uint8_t x, uint8_t y) const { return _cells.at(x * _size + y);}
	
	uint32_t _score;
	direction_t _direction;
	uint8_t _size;
private:
	void move(uint8_t x, uint8_t y);
	void remove(uint8_t x, uint8_t y);
	
	std::vector<CellTransition> _cells;
	std::vector<NewCell> _newCells;
};

}
#endif
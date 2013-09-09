#ifndef TRANSITION_H
#define TRANSITION_H

#include "selection.h"

#include <stdint.h>
#include <vector>

namespace squarez
{
	
class GameBoard;

class Transition
{
public:
	struct CellTransition
	{
		CellTransition(int16_t fromx, int16_t fromy, uint8_t tox, uint8_t toy, uint8_t symbol = 0xFF):
			_fromx(fromx), _fromy(fromy), _tox(tox), _toy(toy), _symbol(symbol), _removed(false) {}
		CellTransition(int16_t deletex, int16_t deletey):
			_fromx(deletex), _fromy(deletey), _tox(deletex), _toy(deletey), _symbol(0xFF), _removed(true) {}
		
		int16_t _fromx;
		int16_t _fromy;
		
		uint8_t _tox;
		uint8_t _toy;
		
		uint8_t _symbol;
		bool _removed;
		
		bool isNew() const { return _symbol != 0xFF;}
	};
	Transition() : _score(0) {}
	Transition(GameBoard const& board, Selection const& selection, uint32_t score);
	
	std::vector<CellTransition> const& getCellTransition() const { return _cells; }
	std::size_t size() const { return _cells.size();}
	CellTransition const& get(std::size_t pos) const { return _cells.at(pos);}
	
	uint32_t _score;
private:
	std::vector<CellTransition> _cells;
	Selection _selection;
};

}
#endif

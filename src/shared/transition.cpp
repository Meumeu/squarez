#include "transition.h"

#include "gameboard.h"

#include <cstdlib>
#include <map>

namespace squarez
{

Transition::Transition(const GameBoard& board, const Selection& selection, uint32_t score): _score(score)
{
	std::map<std::pair<int16_t, int16_t>, CellTransition> cellTransition;
	
	for (std::pair<int16_t, int16_t> point: selection.getPoints())
	{
		cellTransition.insert(std::make_pair(point, CellTransition(point.first, point.second)));
		while (point.second > 0)
		{
			--point.second;
			auto it = cellTransition.find(point);
			if  (it != cellTransition.end())
			{
				if (not it->second._removed)
				{
					it->second._toy++;
				}
			}
			else
			{
				cellTransition.insert(std::make_pair(point, CellTransition(point.first, point.second, point.first, point.second+1)));
			}
		}
		--point.second;
		
		uint8_t symbol = std::rand() % board.symbol();
		if (cellTransition.find(point) != cellTransition.end())
		{
			cellTransition.find(point)->second._toy++;
			--point.second;
			cellTransition.insert(std::make_pair(point, CellTransition(point.first, point.second, point.first, point.second+2, symbol)));
		}
		else
		{
			cellTransition.insert(std::make_pair(point, CellTransition(point.first, point.second, point.first, point.second+1, symbol)));
		}
	}
	
	for (auto const& cell : cellTransition)
	{
		_cells.push_back(cell.second);
	}
}

}

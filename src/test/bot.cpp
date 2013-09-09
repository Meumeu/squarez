#include "shared/gameboard.h"
#include <iostream>

int main()
{
	squarez::GameBoard board(8, 3);
	int score = 0;
	int nbIter = 0;
	
	while (true)
	{
		squarez::Transition best_t;
		int nbCarres = 0;

		for (auto const& transition: board.findTransitions())
		{
			if (transition._score > best_t._score)
			best_t = transition;
			
			if (transition._score)
				nbCarres++;
		}

		board.applyTransition(best_t);

		if (best_t._score)
		{
			nbIter++;
			score += best_t._score;
			std::cout << "Score: " << score << std::endl;
			std::cout << "Number of carrés: " << nbCarres << std::endl;
			std::cout << "Number of itérations: " << nbIter << std::endl;
			std::cout << board << std::endl;
		}
		else
		{
			return 1;
		}
	}
}
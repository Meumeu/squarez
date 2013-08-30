#include <iostream>
#include "gameboard.h"

int main(int argc, char **argv) {
	squarez::GameBoard b(8,3);
	std::cout << b<< std::endl;
	
	squarez::Selection s;
	s.addPoint(0,0);
	s.addPoint(0,1);
	s.addPoint(1,1);
	s.addPoint(1,0);
	std::cout << b.selectSquare(s);
	return 0;
}

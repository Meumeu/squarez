#include <iostream>
#include "shared/gameboard.h"

int main(int argc, char **argv) {
	squarez::GameBoard b(8,3);
	std::cout << b<< std::endl;
	
	b.set(0,5,0);
	b.set(0,6,0);
	b.set(1,5,0);
	b.set(1,6,0);
	
	std::cout << std::endl << b<< std::endl;
	
	squarez::Selection s;
	s.addPoint(0,5);
	s.addPoint(0,6);
	s.addPoint(1,6);
	s.addPoint(1,5);
	squarez::Transition t = b.selectSquare(s);
	
	b.applyTransition(t);
	std::cout << std::endl << b<< std::endl;
	
	return 0;
}

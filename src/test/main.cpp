#include <iostream>
#include "shared/gameboard.h"

int main() {
	squarez::GameBoard b(8,3);
	
	b.set(0,4,0);
	b.set(0,6,0);
	b.set(2,4,0);
	b.set(2,6,0);
	b.set(3,4,1);
	
	std::cout << std::endl << b<< std::endl;
	
	squarez::Selection s;
	s.addPoint(0,4);
	s.addPoint(3,4);
	s.addPoint(0,6);
	s.addPoint(2,6);
	s.addPoint(3,4);
	s.addPoint(2,4);
	squarez::Transition t = b.selectSquare(s);
	
	b.applyTransition(t);
	std::cout << std::endl << b<< std::endl;
	
	return 0;
}

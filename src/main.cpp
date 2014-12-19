
#include "BoardState.h"
#include "PuzzleSolver.h"

int main(int argc, char** argv) {
	const char* str = "0,4,2,3,13,8,7,6,5,10,11,1,9,12,15,14";
	BoardState p(str);
	PuzzleSolver ps(p);
	return 0;
}



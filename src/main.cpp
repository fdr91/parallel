
#include "BoardState.h"
#include "PuzzleSolver.h"
#include <string>

int main(int argc, char** argv) {
	const char* str = "0,4,2,3,13,8,7,6,5,10,11,1,9,12,15,14";
	BoardState p(str);
	PuzzleSolver ps(p);
	ps.solve(1);
	std::string initial("0,4,2,3,13,8,7,6,5,10,11,1,9,12,15,14");
	Path pp = ps.getPath();
	std::vector<std::string> vector = pp.getDirections(initial);
	for(std::string s:vector){
		printf("%s\n", s.c_str());
	}
	return 0;
}



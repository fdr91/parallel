#include "BoardState.h"
#include "PuzzleSolver.h"
#include <string>

int main(int argc, char** argv) {
	const char* str = "0,4,2,3,13,8,7,6,5,10,11,1,9,12,15,14";
	PuzzleSolver ps(str);
	ps.solve(1);
	std::string initial("0,4,2,3,13,8,7,6,5,10,11,1,9,12,15,14");
	Path pp = ps.getPath();

	std::vector<std::string> vector = pp.getDirections(initial);
	for (std::string s : vector) {
		printf("%s\n", s.c_str());
	}
	printf("Done\n");
	PuzzleSolver ps1(str);
		ps1.solve(1);
		std::string initial1("0,4,2,3,13,8,7,6,5,10,11,1,9,12,15,14");
		Path pp1 = ps1.getPath();

		std::vector<std::string> vector1 = pp.getDirections(initial);
		for (std::string s : vector) {
			printf("%s\n", s.c_str());
		}
	printf("Done\n");
	return 0;
}


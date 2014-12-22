#include "BoardState.h"
#include "PuzzleSolver.h"
#include <string>
#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>



int main(int argc, char** argv) {
		const char* str = "0,4,2,3,13,8,7,6,5,10,11,1,9,12,15,14";
		PuzzleSolver ps(str);
		ps.solve(8);
		std::string initial(str);
		Path pp = ps.getPath();
		std::vector<std::string> vector = pp.getDirections(initial);
		for (size_t i=0; i<vector.size(); i++) {
			printf("%s\n", vector.at(i).c_str());
		}
		printf("Done\n");
	return 0;
}


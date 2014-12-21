#include "BoardState.h"
#include "PuzzleSolver.h"
#include <string>
#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>

int main(int argc, char** argv) {
//	try{
		const char* str = "0,4,2,3,13,8,7,6,5,10,11,1,9,12,15,14";
		PuzzleSolver ps(str);
		ps.solve(7);
		std::string initial("0,4,2,3,13,8,7,6,5,10,11,1,9,12,15,14");
		Path pp = ps.getPath();

		std::vector<std::string> vector = pp.getDirections(initial);

		for (int i=0; i<vector.size(); i++) {
			printf("%s\n", vector.at(i).c_str());
		}
		printf("Done\n");
/*	} catch (int e){
		printf("Catched int exception: %d", e);
	}*/

	return 0;
}


#include "BoardState.h"
#include "PuzzleSolver.h"
#include <string>
#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>



int main(int argc, char** argv) {
	try{
		std::string initial(argv[2]);
		PuzzleSolver ps(initial);
		ps.solve(/*atoi(argv[1])*/7);
		Path pp = ps.getPath();
		/*std::vector<std::string> vector = pp.getDirections(initial);
		for (size_t i=0; i<vector.size(); i++) {
			printf("%s\n", vector.at(i).c_str());
		}*/
		//std::string pstr = pp.getPath();
		if(Path::checkPath(initial, pp.getPath())){
			printf("Path:\n %s\nIs solution for %s with length %lu\n", pp.getPath().c_str(), initial.c_str(), pp.getPath().length());
		} else {
			printf("Path:\n %s\nIs not solution for %s\n", pp.getPath().c_str(), initial.c_str());
		}
		printf("Done\n");
	} catch(int e){
		printf("Exception int=%d\n", e);
	}
	return 0;
}


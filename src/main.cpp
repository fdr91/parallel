#include "BoardState.h"
#include "PuzzleSolver.h"
#include <string>
#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>
#include "StdCSolver.h"


int main(int argc, char** argv) {
	try{
		std::string initial(argv[2]);
		StdCSolver ps(initial);
		clock_t cl = clock();
		ps.solve(atoi(argv[1]));
		Path pp = ps.getPath();
		cl =  clock() - cl;
		printf("Time elapsed: %f ms\n", (double)cl*1000/CLOCKS_PER_SEC);
		std::string pstr = pp.getPath();
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


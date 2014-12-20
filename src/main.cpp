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
	/*
	str = "9,5,15,4,2,10,12,3,14,6,7,8,0,13,11,1";
	ps.reset(str);
	ps.solve(1);
	initial = std::string(str);
	pp = ps.getPath();
	vector = pp.getDirections(initial);
	for (std::string s : vector) {
		printf("%s\n", s.c_str());
	}
	str = "14,12,15,10,8,9,5,1,6,11,7,2,4,3,0,13";
	ps.reset(str);
	ps.solve(1);
	initial = std::string(str);
	pp = ps.getPath();
	vector = pp.getDirections(initial);
	for (std::string s : vector) {
		printf("%s\n", s.c_str());
	}
	str = "3,5,11,8,1,13,7,0,10,6,12,2,9,4,14,15";
	ps.reset(str);
	ps.solve(1);
	initial = std::string(str);
	pp = ps.getPath();
	vector = pp.getDirections(initial);
	for (std::string s : vector) {
		printf("%s\n", s.c_str());
	}*/
	return 0;
}


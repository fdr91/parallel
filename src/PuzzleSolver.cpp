/*
 * PuzzleSolver.cpp
 *
 *  Created on: Nov 14, 2014
 *      Author: fedor
 */

#include "PuzzleSolver.h"
#include "Utility.h"
#include <stdio.h>
#include <cstring>
#include "IDAStar.h"




PuzzleSolver::PuzzleSolver() {
	// TODO Auto-generated constructor stub

}

PuzzleSolver::PuzzleSolver(std::vector<char> state, int threadCount){
	IDAStar algorithm;
	//algorithm.clear();
	algorithm.solve(arrayToLong(state), threadCount);
	displayStats(state);
	directions = getDirections(state);
}

PuzzleSolver::~PuzzleSolver() {

}

std::vector<std::string> PuzzleSolver::getSolution(void){
	return directions;
}

void PuzzleSolver::getCharSSolution(char *solution, int size){
	std::string tmp;
	for(std::vector<std::string>::iterator it = directions.begin(); it!=directions.end(); it++){
		tmp+='\n';
		tmp+=*it;
	}
	tmp.copy(solution, size, 0);;
//memccpy((void*)tmp.c_str(), (void*)solution, 0, tmp.length()*sizeof(char));
	return;
}



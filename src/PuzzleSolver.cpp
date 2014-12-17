/*
 * PuzzleSolver.cpp
 *
 *  Created on: Nov 14, 2014
 *      Author: fedor
 */

#include "PuzzleSolver.h"
#include "PuzzleConfiguration.h"
#include "Utility.h"


PuzzleSolver::PuzzleSolver() {
	// TODO Auto-generated constructor stub

}

PuzzleSolver::PuzzleSolver(std::vector<char> state, int threadCount){
	PuzzleConfiguration::setVerbose(true);
	PuzzleConfiguration::initialize(16);
	IDAStar algorithm = PuzzleConfiguration::getAlgorithm();
	algorithm.solve(arrayToLong(state), threadCount);
	displayStats(state);
	directions = getDirections(state);
}

PuzzleSolver::~PuzzleSolver() {
	// TODO Auto-generated destructor stub
}

std::vector<std::string> PuzzleSolver::getSolution(void){
	return directions;
}

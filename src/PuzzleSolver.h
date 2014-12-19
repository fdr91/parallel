/*
 * PuzzleSolver.h
 *
 *  Created on: Nov 14, 2014
 *      Author: fedor
 */

#ifndef PUZZLESOLVER_H_
#define PUZZLESOLVER_H_

#include <vector>
#include <string>

class PuzzleSolver {
	std::vector<std::string> directions;
public:
	std::vector<std::string> getSolution(void);
	char* getSolutionChar(void);
	PuzzleSolver();
	PuzzleSolver(std::vector<char> state, int threadCount);
	void getCharSSolution(char *solution, int size);
	virtual ~PuzzleSolver();
};

#endif /* PUZZLESOLVER_H_ */

/*
 * PuzzleSolver.h
 *
 *  Created on: Dec 19, 2014
 *      Author: fedor
 */

#ifndef PUZZLESOLVER_H_
#define PUZZLESOLVER_H_
#include "BoardState.h"
#include "Path.h"
#include <string>
#include <pthread.h>

class PuzzleSolver {
	BoardState puzzle;
	BoardState state;
	int threadCount;
	bool solved;
	char* costTable_15_puzzle_0;
	char* costTable_15_puzzle_1;
	char* costTable_15_puzzle_2;
	pthread_mutex_t running_mutex;
	const int size=16;

	Path path;

	void loadStreamCostTable(const std::string filename, char* costTable, int size);

	int* tilePositions;
	int* tileSubsets;

public:
	int h(BoardState);
	PuzzleSolver();
	PuzzleSolver(BoardState& puzzle);
	Path getPath();
	void setPath(Path p);
	bool getSolved();
	bool setSolved();
	void solve(int t);
	void solveSingleThread();
	void setInitial();
	virtual ~PuzzleSolver();
};

#endif /* PUZZLESOLVER_H_ */

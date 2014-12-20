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
#include <list>
#include <pthread.h>
#include <map>

class PuzzleSolver {
	BoardState puzzle;
	BoardState state;
	int threadCount;
	volatile bool solved;
	char* costTable_15_puzzle_0;
	char* costTable_15_puzzle_1;
	char* costTable_15_puzzle_2;

	const int size=16;

	int initialMovesEstimate, movesRequired;

	Path path;


	void loadStreamCostTable(const std::string filename, char* costTable, int size);

	int tilePositions [16] = {-1, 0, 0, 1, 2, 1, 2, 0, 1, 3, 4, 2, 3, 5, 4, 5};
	int tileSubsets [16] = {-1, 1, 0, 0, 0, 1, 1, 2, 2, 1, 1, 2, 2, 1, 2, 2};
	void completeBFS(Path p);
	void putToQueue(Path* path, std::map<int64_t, Path>* m,  std::list<Path>& list);
public:
	int h(BoardState);
	PuzzleSolver();
//	PuzzleSolver(const PuzzleSolver& i);
	void reset(const char* puzzle);
	PuzzleSolver(const char* puzzle);
	PuzzleSolver(BoardState& puzzle);
	Path getPath();
	void setPath(Path p);
	bool getSolved();
	bool setSolved();
	void solve(int t);
	void solveSingleThread();
	void solveMultyThread(int threadCount);
	void setInitial();
	void findStartingPositions(BoardState state, int tc, std::list<Path>& list);
	virtual ~PuzzleSolver();
};

#endif /* PUZZLESOLVER_H_ */

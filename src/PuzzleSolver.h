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
	static char costTable_15_puzzle_0 [];
	static char costTable_15_puzzle_1 [];
	static char costTable_15_puzzle_2 [];

	const int size=16;

	int initialMovesEstimate, movesRequired;

	Path path;

	void loadStreamCostTable(const std::string& filename, char* costTable, int size);

	static int tilePositions [16];
	static int tileSubsets [16];
	void completeBFS(Path& p);
	void putToQueue(Path* path, std::map<int64_t, Path>* m,  std::list<Path>& list);
public:
	static int h(BoardState&);
	PuzzleSolver();
	void reset(const char* puzzle);
	PuzzleSolver(const std::string& puzzle);
	PuzzleSolver(BoardState& puzzle);
	Path getPath();
	void setPath(Path& p);
	bool getSolved();
	void solve(int t);
	void solveSingleThread();
	void solveMultyThread(int threadCount);
	void setInitial();
	void findStartingPositions(BoardState& state, size_t tc, std::list<Path>& list);
	virtual ~PuzzleSolver();
	static void * runWorker1(void*);
};

typedef struct ThreadArg {
	PuzzleSolver *parrent;
	Path node;
	int movesRequired;
	std::string ret;
}THREAD_ARG, *PTHREAD_ARG;

#endif /* PUZZLESOLVER_H_ */

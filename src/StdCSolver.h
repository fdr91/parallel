/*
 * PuzzleSolver.h
 *
 *  Created on: Dec 19, 2014
 *      Author: fedor
 */

#ifndef STDCSOLVER_H_
#define STDCSOLVER_H_
#include "BoardState.h"
#include "Path.h"
#include <string>
#include <list>
#include <map>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>


class StdCSolver {
	BoardState puzzle;
	BoardState state;
	static int threadCount;
	static int threadStarted;

	static bool solved;
	static char costTable_15_puzzle_0 [];
	static char costTable_15_puzzle_1 [];
	static char costTable_15_puzzle_2 [];

	const int size=16;

	static int movesRequired;
	int initialMovesEstimate;

	static Path path;
	std::vector<std::thread> threadPool;


	static std::mutex poolThreadMutex;
	static std::mutex mainThreadMutex;

	static std::condition_variable poolThreadCV;
	static std::condition_variable mainThreadCV;

	static std::unique_lock<std::mutex> mainThreadLock;
	static std::unique_lock<std::mutex> poolThreadLock;

	static std::queue<Path> q;
	static std::mutex qMutex;
	static std::mutex pMutex;


	void loadStreamCostTable(const std::string& filename, char* costTable, int size);

	static int tilePositions [16];
	static int tileSubsets [16];
	void completeBFS(Path& p);
	void putToQueue(Path* path, std::map<int64_t, Path>* m,  std::list<Path>& list);
	static void poolFunction();
	static std::list<Path> list;
	static std::list<Path>::iterator it;
public:
	static int runWorker1(Path node, int movesRequired, std::string* retStr);
	static int h(BoardState&);
	StdCSolver();
	void reset(const char* puzzle);
	StdCSolver(const std::string& puzzle);
	StdCSolver(BoardState& puzzle);
	Path getPath();
	void setPath(Path& p);
	bool getSolved();
	void solve(int t);
	void solveSingleThread();
	void solveMultyThread(int threadCount);
	void setInitial();
	void findStartingPositions(BoardState& state, size_t tc, std::list<Path>& list);
	virtual ~StdCSolver();
	static void * runWorker1(void*);
};

typedef struct ThreadArg1 {
	StdCSolver *parrent;
	Path node;
	int movesRequired;
	std::string ret;
}THREAD_ARG1, *PTHREAD_ARG1;

#endif /* PUZZLESOLVER_H_ */

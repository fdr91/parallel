/*
 * IDAStar.h
 *
 *  Created on: Nov 14, 2014
 *      Author: fedor
 */

#ifndef IDASTAR_H_
#define IDASTAR_H_

#include <string>
#include "BFSNode.h"
#include "DFSWorker.h"
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <mqueue.h>
#include <map>
#include <queue>



class IDAStar {
	static void initialize();

	static void markEndTime();

	void completeBFS(const BFSNode node);

	/*private Queue<BFSNode> queue;*/
	DFSWorker* workers;

	static clock_t startTime, endTime;

	void findStartingPositions(const int64_t currentState, int numOfThreads);

	std::queue<BFSNode> queue;
	void static openQueue(mqd_t&  q);
	void putToQueue(BFSNode* node, std::map<int64_t, BFSNode>* m);


public:
	//static volatile bool running, solved;
	volatile static int64_t numberVisited, numberExpanded;

	IDAStar();
	virtual ~IDAStar();

	static const int NOT_APPLICABLE = -1;

	static int initialMovesEstimate;
	static int movesRequired;
	//static std::string shortestPath;
	static pthread_mutex_t running_mutex;
	//static char shortestPath[256];
	static std::vector<char> shortestPath;

	static float getRunningTimeInSeconds();

	static float getElapsedTimeInSeconds();

	void solveSingleThreaded(const int64_t currentState);

	void solveMultyThreaded(const int64_t currentState, int threadCount);

	void solve(const int64_t currentState, int threadCount);

};
#endif /* IDASTAR_H_ */

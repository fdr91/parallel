/*
 * IDAStar.h
 *
 *  Created on: Nov 14, 2014
 *      Author: fedor
 */

#ifndef IDASTAR_H_
#define IDASTAR_H_

#include <string>
//#include "BFSNode.h"
#include "DFSWorker.h"

class IDAStar {
	static void initialize();

	static void markEndTime();

	//void completeBFS(const BFSNode node);

	/*private Queue<BFSNode> queue;*/
	//DFSWorker* workers;

	static clock_t startTime, endTime;

public:
	volatile static uint64_t numberVisited, numberExpanded;

	IDAStar();
	virtual ~IDAStar();

	static const int NOT_APPLICABLE = -1;

	static int initialMovesEstimate;
	static int movesRequired;
	static bool running, solved;
	static std::string shortestPath;

	static float getRunningTimeInSeconds();

	static float getElapsedTimeInSeconds();

	void solveSingleThreaded(const uint64_t currentState);

	void solve(const uint64_t currentState);

	void start();

	void stop();


};
#endif /* IDASTAR_H_ */

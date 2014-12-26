/*
 * Worker.h
 *
 *  Created on: Dec 19, 2014
 *      Author: fedor
 */

#ifndef WORKER_H_
#define WORKER_H_
#include "BoardState.h"
#include "Path.h"
#include "StdCSolver.h"
#include <unordered_map>
#include <mutex>

class Worker {

	BoardState currentState;
	Path path;
	int depth;
	int pos;
	bool solved;
	char fromDirection;
	int startIndex;
	std::string finalize(std::string p);



public:
	static int terminationFlag;
	static std::mutex term;
	static void cleanMap();
	Worker();
	int isSolved();
	std::string getSolution();
	void depthFirstSearch(BoardState currentState,
			const char fromDirection, const int depth, const int pos);
	Worker(StdCSolver* parrent);
	void setConfig(Path& path, int depth);
	bool run();
	virtual ~Worker();
};

#endif /* WORKER_H_ */

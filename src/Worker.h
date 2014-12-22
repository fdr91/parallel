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
#include "PuzzleSolver.h"

class Worker {
	BoardState currentState;
	PuzzleSolver *parrent;
	Path path;
	int depth;
	int pos;
	bool solved;
	char fromDirection;
	bool terminationFlag=false;
	//std::string startPath;
	int startIndex;
	std::string finalize(std::string p);

public:
	Worker();
	//bool run(std::string* retval);
	int isSolved();
	std::string getSolution();
	void setConfig(BoardState currentState, Path path, char from, int depth, int pos);
	void depthFirstSearch(BoardState currentState,
			const char fromDirection, const int depth, const int pos);
	Worker(PuzzleSolver* parrent);
	bool run();
	virtual ~Worker();
};

#endif /* WORKER_H_ */

/*
 * DFSWorker.h
 *
 *  Created on: Nov 14, 2014
 *      Author: fedor
 */

#ifndef DFSWORKER_H_
#define DFSWORKER_H_

#include <string>

class DFSWorker {

	uint64_t currentState, numberVisited, numberExpanded;
	int movesRequired;
    int depth, pos, yieldCount;
    bool solved;
    char fromDirection;
    char* path = new char[81];

    void depthFirstSearch(const uint64_t currentState,
                                  const char fromDirection,
                                  const int depth,
                                  const int pos);

public:


    void setConfig(const uint64_t currentState,
                             const std::string pathStr,
                             const int depth,
                             const int pos);

    void run();
    std::string getShortestPath();

	DFSWorker();
	virtual ~DFSWorker();
};

#endif /* DFSWORKER_H_ */

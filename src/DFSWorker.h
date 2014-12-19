/*
 * DFSWorker.h
 *
 *  Created on: Nov 14, 2014
 *      Author: fedor
 */

#ifndef DFSWORKER_H_
#define DFSWORKER_H_

#include <string>
#include <stdint.h>

class DFSWorker {

	int64_t currentState, numberVisited, numberExpanded;
	int movesRequired;
    int depth, pos, yieldCount;
    bool solved;
    char fromDirection;
    std::string path;

    static int id;
    int myId;



    void depthFirstSearch(const int64_t currentState,
                                  const char fromDirection,
                                  const int depth,
                                  const int pos);

public:


    void setConfig(const int64_t currentState,
                             const std::string pathStr,
                             const int depth,
                             const int pos);

    void run();
    std::string getShortestPath();

	DFSWorker();
	~DFSWorker();
};

#endif /* DFSWORKER_H_ */

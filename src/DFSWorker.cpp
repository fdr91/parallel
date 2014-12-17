/*
 * DFSWorker.cpp
 *
 *  Created on: Nov 14, 2014
 *      Author: fedor
 */

#include "DFSWorker.h"
#include "Node.h"
#include "IDAStar.h"
#include  "IDAStarNode.h"
#include <iostream>
#include <cstring>
#include "PuzzleConfiguration.h"

using namespace std;


DFSWorker::DFSWorker() {
}

/*char* DFSWorker::getShortestPath() {
		//TODO
        return string(path).substr(1);
    }*/

void DFSWorker::run() {
		printf("DFSWorker start run\n");
        depthFirstSearch(currentState, fromDirection, depth, pos);
        IDAStar::numberVisited += numberVisited;
        IDAStar::numberExpanded += numberExpanded;
        if (solved) {
        	 printf("---Solved--- %d\n", strlen(path));
        	for(int i=strlen(path); i>0; i--)
        		IDAStar::shortestPath[i-1]=path[i];
        }
        printf("DFSWorker finish run\n");
    }


DFSWorker::~DFSWorker() {
	// TODO Auto-generated destructor stub
}

void DFSWorker::setConfig(const uint64_t currentState,
                          string pathStr,
                          const int depth,
                          const int pos) {
        this->currentState = currentState;
        this->depth = depth;
        this->pos = pos;
        this->solved=false;
        // It's not necessary to initialize path elements each time this method
        // is called, since subsequent paths will always be uint64_ter by 2 moves.
        memset(path,0,sizeof(path));
        path[0]=pathStr.at(0);
        fromDirection = path[0];
        numberVisited = numberExpanded = yieldCount = 0;
    }

 void DFSWorker::depthFirstSearch(const uint64_t currentState,
		 const char fromDirection,
		 const int depth,
		 const int pos) {

        if (!IDAStar::running) return;

        if (currentState == Node::goalState) {
            IDAStar::solved = true;
            IDAStar::running = false;
            path[pos] = fromDirection;
            solved = true;
            if (PuzzleConfiguration::getVerbose()) {
                std::cout<<"done."<<std::endl;
            }
            return;
        }

        const int posOfSpace = Node::posOfSpace(currentState),
                  posPlusOne = pos + 1;

        if (fromDirection != 'R') {
            const uint64_t successor = IDAStarNode::moveLeft(currentState, posOfSpace);
            if (successor != 0) {
                ++numberExpanded;
                if (posPlusOne + Node::h(successor) <= depth) {
                    depthFirstSearch(successor, 'L', depth, posPlusOne);
                } else {
                    ++numberVisited;
                }
                if (solved) {
                    path[pos] = fromDirection;
                    return;
                }
            }
        }

        if (fromDirection != 'L') {
            const uint64_t successor = IDAStarNode::moveRight(currentState, posOfSpace);
            if (successor != 0) {
                ++numberExpanded;
                if (posPlusOne + Node::h(successor) <= depth) {
                    depthFirstSearch(successor, 'R', depth, posPlusOne);
                } else {
                    ++numberVisited;
                }
                if (solved) {
                    path[pos] = fromDirection;
                    return;
                }
            }
        }

        if (fromDirection != 'D') {
            uint64_t successor = IDAStarNode::moveUp(currentState, posOfSpace);
            if (successor != 0) {
                ++numberExpanded;
                if (posPlusOne + Node::h(successor) <= depth) {
                    depthFirstSearch(successor, 'U', depth, posPlusOne);
                } else {
                    ++numberVisited;
                }
                if (solved) {
                    path[pos] = fromDirection;
                    return;
                }
            }
        }

        if (fromDirection != 'U') {
            uint64_t successor = IDAStarNode::moveDown(currentState, posOfSpace);
            if (successor != 0) {
                ++numberExpanded;
                if (posPlusOne + Node::h(successor) <= depth) {
                    depthFirstSearch(successor, 'D', depth, posPlusOne);
                } else {
                    ++numberVisited;
                }
                if (solved) {
                    path[pos] = fromDirection;
                    return;
                }
            }
        }
 }

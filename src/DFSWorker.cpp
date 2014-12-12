/*
 * DFSWorker.cpp
 *
 *  Created on: Nov 14, 2014
 *      Author: fedor
 */

#include "DFSWorker.h"
#include "Node.h"
#include "IDAStar.h"

#include <iostream>
#include "PuzzleSolver.h"

using namespace std;


DFSWorker::DFSWorker() {
	solved=false;
	numberVisited=0;
	currentState =0;
	depth=0;
	fromDirection=0;
	movesRequired=0;
	numberExpanded=0;
	pos=0;
	yieldCount=0;
}

string DFSWorker::getShortestPath() {
		//TODO
        return string(path).substr(1);
    }

 void DFSWorker::run() {
        depthFirstSearch(currentState, fromDirection, depth, pos);
        IDAStar::numberVisited += numberVisited;
        IDAStar::numberExpanded += numberExpanded;
        if (solved) {
            IDAStar::shortestPath = getShortestPath();
        }
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
        int i = 0;
        // It's not necessary to initialize path elements each time this method
        // is called, since subsequent paths will always be uint64_ter by 2 moves.
        for (const int len = pathStr.length(); i < len; ++i) {
            path[i] = pathStr.at(i);
        }
        fromDirection = path[i - 1];
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
            if (PuzzleSolver::getVerbose()) {
                std::cout<<"done."<<std::endl;
            }
            return;
        }

        const int posOfSpace = Node::posOfSpace(currentState),
                  posPlusOne = pos + 1;

        if (fromDirection != 'R') {
            const uint64_t successor = Node::moveLeft(currentState, posOfSpace);
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
            const uint64_t successor = Node::moveRight(currentState, posOfSpace);
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
            uint64_t successor = Node::moveUp(currentState, posOfSpace);
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
            uint64_t successor = Node::moveDown(currentState, posOfSpace);
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

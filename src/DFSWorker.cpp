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
#include <stdio.h>
#include "globals.h"

using namespace std;

int DFSWorker::id;

DFSWorker::DFSWorker() {
	/*printf("Worker created\n");*/
	myId=id++;
	solved = false;
	pos = 0;
	this->yieldCount=0;
}

/*char* DFSWorker::getShortestPath() {
 //TODO
 return string(path).substr(1);
 }*/

//char*

void vToStr(){

}

void DFSWorker::run() {
	depthFirstSearch(currentState, fromDirection, depth, pos);
	IDAStar::numberVisited += numberVisited;
	IDAStar::numberExpanded += numberExpanded;
	pthread_mutex_lock(&IDAStar::running_mutex);
	if (solved) {
		IDAStar::shortestPath.clear();
		for (int i = 1; i <= path.length(); i++)
			IDAStar::shortestPath.push_back(path[i]);
		printf("Solved. The path size is %d %d\n", IDAStar::shortestPath.size(), path.length());
	}
	pthread_mutex_unlock(&IDAStar::running_mutex);
}

DFSWorker::~DFSWorker() {
}

void DFSWorker::setConfig(const int64_t currentState, string pathStr,
		const int depth, const int pos) {
	this->currentState = currentState;
	this->depth = depth;
	this->pos = pos;
	this->solved = false;
	//memset(path, 0, 256*sizeof(char));
	//strcpy(path, pathStr.c_str());
	for(unsigned int  i=0; i<pathStr.length(); i++){
		this->path[i]=pathStr.c_str()[i];
	}
	this->path[0] = pathStr.at(0);
	fromDirection = path[0];
	numberVisited = numberExpanded = yieldCount = 0;
}

void DFSWorker::depthFirstSearch(const int64_t currentState,
		const char fromDirection, const int depth, const int pos) {

	if (!gruning)
		return;

	if (currentState == GOAL_STATE) {
		pthread_mutex_lock(&IDAStar::running_mutex);
		if (!gsolved) {
			gsolved = true;
			gruning = false;
			path.resize(pos+1);
			path[pos] = fromDirection;
			solved = true;
			if (IS_VERBOSE) {
				printf("Solved\n");
			}
		}
		pthread_mutex_unlock(&IDAStar::running_mutex);
		return;
	}

	const int posOfSpace = Node::posOfSpace(currentState), posPlusOne = pos + 1;

	if (fromDirection != 'R') {
		const int64_t successor = IDAStarNode::moveLeft(currentState,
				posOfSpace);
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
		const int64_t successor = IDAStarNode::moveRight(currentState,
				posOfSpace);
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
		int64_t successor = IDAStarNode::moveUp(currentState, posOfSpace);
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
		int64_t successor = IDAStarNode::moveDown(currentState, posOfSpace);
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

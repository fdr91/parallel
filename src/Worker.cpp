/*
 * Worker.cpp
 *
 *  Created on: Dec 19, 2014
 *      Author: fedor
 */

#include "Worker.h"
#include <stdio.h>
#include <algorithm>

extern pthread_mutex_t running_mutex;
extern pthread_mutex_t solved_mutex;


Worker::Worker() {
	this->parrent = NULL;
	this->depth = -1;
	this->pos = -1;
	this->solved = false;
	this->fromDirection = 'X';

}

Worker::Worker(PuzzleSolver *parrent) {

	this->depth = -1;
	this->pos = -1;
	this->solved = false;
	this->fromDirection = 'X';
	this->parrent = parrent;

}

void Worker::setConfig(BoardState currentState, Path path, char from, int depth,
		int pos) {
	this->currentState = BoardState(currentState);
	this->depth = depth;
	this->pos = pos;
	this->solved = currentState.isGoal();
	this->path = Path(path);
	this->fromDirection = from;
}

int Worker::isSolved() {
	return this->solved && !terminationFlag ? 1 : 0;
}

bool Worker::run() {
	depthFirstSearch(currentState, fromDirection, depth, pos);
	bool ret=false;
	pthread_mutex_lock(&running_mutex);
	if (solved && !terminationFlag) {
		/*if (parrent->setSolved()) {
		 parrent->setPath(this->path);
		 printf("Solved. The path size is %d\n", this->path.size());
		 }*/
		std::string finalPath=finalize(path.getPath());
		this->path.setPath(finalPath);
		parrent->setPath(this->path);
		ret = solved;

	}
	pthread_mutex_unlock(&running_mutex);
	return ret;

}

bool Worker::run(std::string* retval) {
	depthFirstSearch(currentState, fromDirection, depth, pos);
	bool ret=false;
	pthread_mutex_lock(&running_mutex);
	if (solved && !terminationFlag) {
		/*if (parrent->setSolved()) {
		 parrent->setPath(this->path);
		 printf("Solved. The path size is %d\n", this->path.size());
		 }*/
		std::string finalPath=finalize(path.getPath());
		this->path.setPath(finalPath);
		//parrent->setPath(this->path);
		ret = solved;
		*retval = std::string(path.getPath());
	}
	pthread_mutex_unlock(&running_mutex);
	return ret;

}


std::string Worker::finalize(std::string _p){
	std::string p(_p);
	int endIndex;
	if(*(p.end()-1)=='X')
		endIndex=-2;
	std::string ret = p.substr(startIndex, p.length()-endIndex);
	if(*(p.begin())=='X')
		p.erase(p.begin());
	std::reverse(ret.begin(), ret.end());
	ret=p.substr(0, startIndex-2)+ret;
	printf("%s\n", ret.c_str());
	return ret;
}


void Worker::depthFirstSearch(BoardState currentState, const char fromDirection,
		const int depth, const int pos) {
	if (parrent!=NULL && parrent->getSolved())
		return;
	this->startIndex = path.getPath().length();
	if (currentState.isGoal()) {
		pthread_mutex_lock(&running_mutex);
		//if (parrent->setSolved()) {
			solved = true;
			path.append(fromDirection);
			printf("Solved\n");
		//}
		pthread_mutex_unlock(&running_mutex);
		if (!solved)
			this->terminationFlag = true;
		return;

	}

	const int posPlusOne = pos + 1;

	if (fromDirection != 'R') {
		BoardState successor = currentState.moveLeft();
		if (successor.getLong() != 0) {
			if (posPlusOne + PuzzleSolver::h(successor) <= depth) {
				depthFirstSearch(successor, 'L', depth, posPlusOne);
			}
			if (terminationFlag) {
				return;
			}
			if (solved) {
				//path.set(pos,fromDirection);
				path.append(fromDirection);
				return;
			}

		}
	}

	if (fromDirection != 'L') {
		BoardState successor = currentState.moveRight();
		if (successor.getLong() != 0) {

			if (posPlusOne + PuzzleSolver::h(successor) <= depth) {
				depthFirstSearch(successor, 'R', depth, posPlusOne);
			}

			if (terminationFlag) {
				return;
			}
			if (solved) {
				//path.set(pos,fromDirection);
				path.append(fromDirection);
				return;
			}
		}
	}

	if (fromDirection != 'D') {
		BoardState successor = currentState.moveUp();
		if (successor.getLong() != 0) {
			if (posPlusOne + PuzzleSolver::h(successor) <= depth) {
				depthFirstSearch(successor, 'U', depth, posPlusOne);
			}

			if (terminationFlag) {
				return;
			}
			if (solved) {
				//path.set(pos,fromDirection);
				path.append(fromDirection);
				return;
			}
		}
	}

	if (fromDirection != 'U') {
		BoardState successor = currentState.moveDown();
		if (successor.getLong() != 0) {
			if (posPlusOne + PuzzleSolver::h(successor) <= depth) {
				depthFirstSearch(successor, 'D', depth, posPlusOne);
			}

			if (terminationFlag) {
				return;
			}
			if (solved) {
				//path.set(pos,fromDirection);
				path.append(fromDirection);
				return;
			}
		}
	}
	return;
}

Worker::~Worker() {
	// TODO Auto-generated destructor stub
}


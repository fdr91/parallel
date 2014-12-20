/*
 * Worker.cpp
 *
 *  Created on: Dec 19, 2014
 *      Author: fedor
 */

#include "Worker.h"

Worker::Worker() {
	this->parrent=NULL;
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

void Worker::run() {
	depthFirstSearch(currentState, fromDirection, depth, pos);
	if (solved) {
		if (parrent->setSolved()) {
			parrent->setPath(this->path);
			printf("Solved. The path size is %d\n", this->path.size());
		}
	}
}

void Worker::depthFirstSearch(BoardState currentState, const char fromDirection,
		const int depth, const int pos) {
	if (parrent->getSolved())
		return;

	if (currentState.isGoal()) {
		solved = true;
		path.set(pos, fromDirection);
		printf("Solved\n");
		return;
	}

	const int  posPlusOne = pos + 1;

	if (fromDirection != 'R') {
		BoardState successor = currentState.moveLeft();
		if (successor.getLong() != 0) {
			if (posPlusOne + parrent->h(successor) <= depth) {
				depthFirstSearch(successor, 'L', depth, posPlusOne);
			}
			if (solved) {
				path.set(pos,fromDirection);
				return;
			}
		}
	}

	if (fromDirection != 'L') {
		BoardState successor = currentState.moveRight();
		if (successor.getLong() != 0) {

			if (posPlusOne + parrent->h(successor) <= depth) {
				depthFirstSearch(successor, 'R', depth, posPlusOne);
			}
			if (solved) {
				path.set(pos,fromDirection);
				return;
			}
		}
	}

	if (fromDirection != 'D') {
		BoardState successor = currentState.moveUp();
		if (successor.getLong() != 0) {
			if (posPlusOne + parrent->h(successor) <= depth) {
				depthFirstSearch(successor, 'U', depth, posPlusOne);
			}
			if (solved) {
				path.set(pos,fromDirection);
				return;
			}
		}
	}

	if (fromDirection != 'U') {
		BoardState  successor = currentState.moveDown();
		if (successor.getLong() != 0) {
			if (posPlusOne + parrent->h(successor) <= depth) {
				depthFirstSearch(successor, 'D', depth, posPlusOne);
			}
			if (solved) {
				path.set(pos,fromDirection);
				return;
			}
		}
	}
	return;
}

Worker::~Worker() {
	// TODO Auto-generated destructor stub
}


#include "Worker.h"
#include <stdio.h>
#include <algorithm>

//extern pthread_mutex_t running_mutex;


Worker::Worker() {
	startIndex=0;
	this->parrent = NULL;
	this->depth = -1;
	this->pos = -1;
	this->solved = false;
	this->fromDirection = 'X';

}

Worker::Worker(PuzzleSolver *parrent) {
	startIndex=0;
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

std::string Worker::getSolution(){
	return this->path.getPath();
}

bool Worker::run() {
	depthFirstSearch(currentState, fromDirection, depth, pos);
	bool ret=false;
	if (solved && !terminationFlag) {
		std::string finalPath=finalize(path.getPath());
		this->path.setPath(finalPath);
		ret = solved;
	}
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
	if(startIndex>1)
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
			solved = true;
			path.append(fromDirection);
			printf("Solved\n");
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

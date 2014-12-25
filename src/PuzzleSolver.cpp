/*
 * PuzzleSolver.cpp
 *
 *  Created on: Dec 19, 2014
 *      Author: fedor
 */

#include "PuzzleSolver.h"
#include <stdexcept>
#include <fstream>
#include <map>
#include "Worker.h"
#include <time.h>
#include <omp.h>

using namespace std;

int PuzzleSolver::tilePositions[] = { -1, 0, 0, 1, 2, 1, 2, 0, 1, 3, 4, 2, 3, 5,
		4, 5 };
int PuzzleSolver::tileSubsets[] = { -1, 1, 0, 0, 0, 1, 1, 2, 2, 1, 1, 2, 2, 1,
		2, 2 };
char PuzzleSolver::costTable_15_puzzle_0[4096];
char PuzzleSolver::costTable_15_puzzle_1[16777216];
char PuzzleSolver::costTable_15_puzzle_2[16777216];


bool PuzzleSolver::getSolved() {
	return solved;
}
int PuzzleSolver::h(BoardState& bs) {
	int64_t boardConfig = bs.getLong();
	int index0 = 0, index1 = 0, index2 = 0;
	for (int pos = 16 - 1; pos >= 0; --pos) {
		const int tile = (int) ((boardConfig >> (pos << 2)) & 0xF);
		if (tile != 0) {
			const int subsetNumber = tileSubsets[tile];
			switch (subsetNumber) {
				case 2:
				index2 |= pos << (tilePositions[tile] << 2);
				break;
				case 1:
				index1 |= pos << (tilePositions[tile] << 2);
				break;
				default:
				index0 |= pos << (tilePositions[tile] << 2);
				break;
			}
		}
	}
	return costTable_15_puzzle_0[index0] + costTable_15_puzzle_1[index1]
	+ costTable_15_puzzle_2[index2];
}

Path PuzzleSolver::getPath() {
	return this->path;
}
void PuzzleSolver::setPath(Path& p) {
	this->path = p;
}

PuzzleSolver::PuzzleSolver() {
	initialMovesEstimate = 0;
	movesRequired = 0;
	threadCount = -1;
	solved = false;
}

void PuzzleSolver::reset(const char* _p) {
	BoardState p(_p);
	initialMovesEstimate = 0;
	movesRequired = 0;
	solved = p.isGoal();
	threadCount = -1;
	puzzle = BoardState(p);
	state = BoardState(p);
}

PuzzleSolver::PuzzleSolver(const std::string &_p) {
	BoardState p(_p);
	initialMovesEstimate = 0;
	movesRequired = 0;
	solved = p.isGoal();
	threadCount = -1;
	puzzle = BoardState(p);
	state = BoardState(p);
}

void PuzzleSolver::solveSingleThread() {
	initialMovesEstimate = movesRequired = h(this->puzzle);
	Worker worker(this);
	do {
		printf("Searching path of depth %d...\n", movesRequired);
		Path p(puzzle);
		worker.setConfig(p, movesRequired);
		worker.run();
		if (!worker.isSolved()) {
			movesRequired += 2;
		} else {
			string str = worker.getSolution();
			this->path.setPath(str);
			solved = true;
		}
	}while (!solved);
}

void PuzzleSolver::completeBFS(Path& currentNode) {
	this->path = Path(currentNode);
	this->solved = true;
}

void PuzzleSolver::putToQueue(Path* path, map<int64_t, Path>* m,
		std::list<Path>& list) {
	int64_t hash = path->stateAsL();
	map<int64_t, Path>::iterator it = m->find(hash);
	if (it == m->end()) {
		(*m)[hash] = Path(*path);
		list.push_back(Path(*path));
	} else if (it->second.stateAsL() != path->stateAsL()
			|| it->second.getPath().compare(path->getPath()) != 0
			|| it->second.getDirection() != path->getDirection()) {
		list.push_back(Path(*path));
	}
}

void PuzzleSolver::findStartingPositions(BoardState& state,
		size_t tc, std::list<Path> &list) {
	Path currentNode(state);
	map<int64_t, Path> m;
	if (currentNode.isSolved()) {
		completeBFS(currentNode);
		return;
	}

	if (tc == 1) {
		list.push_back(currentNode);
		return;
	}

	int previousMovesRequired = 0;

	while (!currentNode.isNull()) {
		const char fromDirection = currentNode.getDirection();
		if (fromDirection != 'R') {
			Path left;
			currentNode.moveLeftNode(&left);
			if (!left.isNull()) {
				if (left.isSolved()) {
					completeBFS(left);
					return;
				} else {
					putToQueue(&left, &m, list);
				}
			}
		}
		if (fromDirection != 'L') {
			Path right;
			currentNode.moveRightNode(&right);
			if (!right.isNull()) {
				if (right.isSolved()) {
					completeBFS(right);
					return;
				} else {
					putToQueue(&right, &m, list);
				}
			}
		}

		if (fromDirection != 'D') {
			Path up;
			currentNode.moveUpNode(&up);
			if (!up.isNull()) {
				if (up.isSolved()) {
					completeBFS(up);
					return;
				} else {
					putToQueue(&up, &m, list);
				}
			}
		}

		if (fromDirection != 'U') {
			Path down;
			currentNode.moveDownNode(&down);
			if (!down.isNull()) {
				if (down.isSolved()) {
					completeBFS(down);
					return;
				} else {
					putToQueue(&down, &m, list);
				}
			}
		}
		currentNode = list.front();
		if (!currentNode.isNull()) {
			const int movesRequired = currentNode.size();

			if (movesRequired > previousMovesRequired) {
				previousMovesRequired = movesRequired;
				if (list.size() >= tc) {
					break;
				}
			} else {
				currentNode = list.front();
				list.erase(list.begin());
			}
		}
	}

}




void * PuzzleSolver::runWorker1(void * _arg) {
	PTHREAD_ARG arg = (THREAD_ARG*) _arg;
	Worker worker;
	worker.setConfig(arg->node,	arg->movesRequired);

	int* ret = new int;
	*ret = worker.run() ? 1 : 0;
	if (*ret)
		arg->ret = worker.getSolution();
	return (void*)ret;
}

#define NUMTHREADS 4

void PuzzleSolver::solveMultyThread(int threadCount) {
	/*if(threadCount!=NUMTHREADS)
		throw 10;*/
	printf("Multy thread solving with thread count = %d\n", threadCount);
	std::list<Path> list;
	findStartingPositions(this->puzzle, threadCount, list);
	initialMovesEstimate = movesRequired = h(this->puzzle);
	int numElements = list.size();
	std::list<Path>::iterator pp = list.begin();

	while (!solved) {
		//pthread_t* threads = new pthread_t[numElements];
		int* results=new int[numElements];
		printf("Searching paths of length %d moves\n", movesRequired);
		std::list<Path>::iterator it = list.begin();
		PTHREAD_ARG* args = new PTHREAD_ARG[numElements];
#pragma omp parallel for num_threads(8)
		for (int i = 0; i < numElements; i++) {
			//++it;
			Path node(*it++);
			args[i] = new THREAD_ARG;
			args[i]->movesRequired = movesRequired;
			args[i]->node = node;

			//pthread_t thr;
			//printf("Start thread with initial path %s\n", node.getPath().c_str());
			/*pthread_create(&thr, NULL, &PuzzleSolver::runWorker1,
					(void*) args[i]);
			threads[i] = thr;*/
			printf("Thread %d started\n", omp_get_thread_num());
			results[i]=*(int*)PuzzleSolver::runWorker1((void*)args[i]);
		}
		for (int i = 0; i < numElements; i++) {
			//void * ret=0;
			/*if (int l = pthread_join(threads[i], &ret)) {
				printf("Error thread join: %d\n", l);
			}*/

			if (results[i]) {
				printf("Set solved flag in main thread. Iteration: %d\n",
						movesRequired);
				solved = true;
				this->path.setPath(args[i]->ret);
			}
			delete args[i];

		}
		delete [] args;
		delete [] results;
		if (!solved) {
			movesRequired += 2;
		}
	}
}

void PuzzleSolver::solve(int t) {
	setInitial();
	if (t >1)
		solveMultyThread(t);
	else if (t==1){
		solveSingleThread();
	} else {
		throw 5;
	}
}

void PuzzleSolver::loadStreamCostTable(const string& filename, char* costTable,
		int size) {

	ifstream is(filename);
	if (is.good()) {
		is.read(costTable, size);
		is.close();
	} else {
		printf("Error when loading cost table");
		throw 1;
	}
	is.close();
}

void PuzzleSolver::setInitial() {
	solved = state.isGoal();
	loadStreamCostTable(string("databases/15-puzzle-663-0.db"),
			costTable_15_puzzle_0, 4096);
	loadStreamCostTable(string("databases/15-puzzle-663-1.db"),
			costTable_15_puzzle_1, 16777216);
	loadStreamCostTable(string("databases/15-puzzle-663-2.db"),
			costTable_15_puzzle_2, 16777216);

}

PuzzleSolver::~PuzzleSolver() {
	printf("Destruct PS\n");
}


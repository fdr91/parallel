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

using namespace std;

pthread_mutex_t running_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t solved_mutex = PTHREAD_MUTEX_INITIALIZER;

/*char* PuzzleSolver::costTable_15_puzzle_0 = NULL;
 char* PuzzleSolver::costTable_15_puzzle_1 = NULL;
 char* PuzzleSolver::costTable_15_puzzle_2 = NULL;*/

bool PuzzleSolver::getSolved() {
	return solved;
}
bool PuzzleSolver::setSolved() {
	bool ret = false;
	if (!solved) {
		ret = true;
		solved = true;
	}
	return ret;
}

int PuzzleSolver::h(BoardState bs) {

	// Create three different indexes that contain only the positions of
	// tiles applicable to the corresponding pattern database.
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
void PuzzleSolver::setPath(Path p) {
	this->path = p;
}

PuzzleSolver::PuzzleSolver() {
	initialMovesEstimate = 0;
	movesRequired = 0;
	costTable_15_puzzle_0 = NULL;
	costTable_15_puzzle_1 = NULL;
	costTable_15_puzzle_2 = NULL;
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

/*PuzzleSolver::PuzzleSolver(const PuzzleSolver& i) {
 costTable_15_puzzle_0 = NULL;
 costTable_15_puzzle_1 = NULL;
 costTable_15_puzzle_2 = NULL;
 this->initialMovesEstimate = i.initialMovesEstimate;
 this->movesRequired = i.movesRequired;
 this->path = i.path;
 this->puzzle = i.puzzle;
 this->running_mutex = PTHREAD_MUTEX_INITIALIZER; //???
 this->solved = i.solved;
 this->state = i.state;
 this->threadCount = i.threadCount;
 }*/



PuzzleSolver::PuzzleSolver(const char* _p) {
	BoardState p(_p);
	initialMovesEstimate = 0;
	movesRequired = 0;
	solved = p.isGoal();
	threadCount = -1;
	puzzle = BoardState(p);
	state = BoardState(p);
	costTable_15_puzzle_0 = NULL;
	costTable_15_puzzle_1 = NULL;
	costTable_15_puzzle_2 = NULL;

}

void PuzzleSolver::solveSingleThread() {
	initialMovesEstimate = movesRequired = h(this->puzzle);
	Worker worker(this);
	do {
		printf("Searching path of depth %d...\n", movesRequired);
		Path p;
		worker.setConfig(puzzle, p, 'X', movesRequired, 0);
		worker.run();
		if (!solved) {
			movesRequired += 2;
		}
	} while (!solved);
}

void PuzzleSolver::completeBFS(Path currentNode) {
	this->path = Path(currentNode);
	this->solved = true;
}

void PuzzleSolver::putToQueue(Path* path, map<int64_t, Path>* m, std::list<Path>& list) {
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

void PuzzleSolver::findStartingPositions(BoardState state, int tc, std::list<Path> &list) {
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

typedef struct ThreadArg{
	PuzzleSolver *parrent;
	Path node;
	int movesRequired;
} THREAD_ARG, *PTHREAD_ARG;

int num_threads=0;
volatile bool ssolved=false;

void * runWorker(void * _arg) {
	num_threads++;
	PTHREAD_ARG arg = (THREAD_ARG*)_arg;

	//Worker* dw = (Worker*) arg;
	Worker worker(arg->parrent);
	worker.setConfig(arg->node.getState(), arg->node, arg->node.getDirection(), arg->movesRequired, arg->node.size() - 1);
	/*if(worker.run()){
		ssolved = true;
	}*/

	worker.run();
	num_threads--;
	pthread_exit(0);
	return 0;
}

void PuzzleSolver::solveMultyThread(int threadCount) {
	printf("Multy thread solving with thread count = %d\n", threadCount);
	std::list<Path> list;
	findStartingPositions(this->puzzle, threadCount, list);
	initialMovesEstimate = movesRequired = h(this->puzzle);
	int numElements = list.size();
	std::list<Path>::iterator pp = list.begin();
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	uint64_t sz=0;
	pthread_attr_getstacksize(&attr, &sz);
	pthread_attr_setstacksize(&attr, 8*16777216 );
	pthread_attr_getguardsize(&attr, &sz);
	pthread_attr_setguardsize(&attr, 4*16777216);
	printf("sz = %ld\n", sz);
	ssolved = false;
	while (!solved) {
		pthread_t threads[64];
		printf("Searching paths of length %d moves\n", movesRequired);
		std::list<Path>::iterator it = list.begin();
		for(int i=0; i<numElements; i++, ++it){
			Path node(*it);
			THREAD_ARG arg;
			arg.parrent=this;
			arg.movesRequired = movesRequired;
			arg.node=node;
			pthread_t thr;
			pthread_create(&thr, &attr, &runWorker, (void*)&arg);
			threads[i]=thr;
		}
		for (int i=0; i<numElements; i++) {
			if (int l = pthread_join(threads[i], NULL)) {
				printf("Error thread join: %d\n", l);
			}
		}
		if(num_threads>0)
			throw num_threads;
		if (!solved) {
			movesRequired += 2;
		}
	}
}

void PuzzleSolver::solve(int t) {
	setInitial();
	//if (t == 1) {
		//solveSingleThread();
	//} else if (t > 1) {
	if(t!=0)
		solveMultyThread(t);
	else{
		solveSingleThread();
	}
	//} else {
	//	throw invalid_argument("Wrong thread count");
	//}
}

void PuzzleSolver::loadStreamCostTable(const string filename, char* costTable,
		int size) {

	ifstream is(filename);
	if (is.good()) {
		is.read(costTable, size);
		is.close();
	} else {
		throw 1;
	}
	is.close();
}

void PuzzleSolver::setInitial() {
	solved = state.isGoal();
	if (!costTable_15_puzzle_0 || !costTable_15_puzzle_1
			|| !costTable_15_puzzle_2) {
		costTable_15_puzzle_0 = new char[4096];
		costTable_15_puzzle_1 = new char[16777216];
		costTable_15_puzzle_2 = new char[16777216];
		loadStreamCostTable(string("databases/15-puzzle-663-0.db"),
				costTable_15_puzzle_0, 4096);
		loadStreamCostTable(string("databases/15-puzzle-663-1.db"),
				costTable_15_puzzle_1, 16777216);
		loadStreamCostTable(string("databases/15-puzzle-663-2.db"),
				costTable_15_puzzle_2, 16777216);
	}

}

PuzzleSolver::~PuzzleSolver() {

	printf("Try to free\n");
	if(costTable_15_puzzle_0!=NULL)
		delete[] costTable_15_puzzle_0;
	if(costTable_15_puzzle_1!=NULL)
		delete[] costTable_15_puzzle_1;
	if(costTable_15_puzzle_2!=NULL)
		delete[] costTable_15_puzzle_2;
	costTable_15_puzzle_0=NULL;
	costTable_15_puzzle_1=NULL;
	costTable_15_puzzle_2=NULL;
	printf("Memory is free\n");
}


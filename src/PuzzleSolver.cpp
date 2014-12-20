/*
 * PuzzleSolver.cpp
 *
 *  Created on: Dec 19, 2014
 *      Author: fedor
 */

#include "PuzzleSolver.h"
#include <stdexcept>
#include <fstream>
#include "Worker.h"

using namespace std;

/*char* PuzzleSolver::costTable_15_puzzle_0 = NULL;
 char* PuzzleSolver::costTable_15_puzzle_1 = NULL;
 char* PuzzleSolver::costTable_15_puzzle_2 = NULL;*/

bool PuzzleSolver::getSolved() {
	return solved;
}
bool PuzzleSolver::setSolved() {
	bool ret = false;
	pthread_mutex_lock(&running_mutex);
	if (!solved) {
		ret = true;
		solved = true;
	}
	pthread_mutex_unlock(&running_mutex);
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
	this->path = Path(p);
}

PuzzleSolver::PuzzleSolver() {
	initialMovesEstimate = 0;
	movesRequired = 0;
	costTable_15_puzzle_0 = NULL;
	costTable_15_puzzle_1 = NULL;
	costTable_15_puzzle_2 = NULL;
	threadCount = -1;
	this->running_mutex=PTHREAD_MUTEX_INITIALIZER;
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
	this->running_mutex=PTHREAD_MUTEX_INITIALIZER;
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

void  PuzzleSolver::solveMultyThread(){

}

void PuzzleSolver::solve(int t) {
	setInitial();
	if (t == 1) {
		solveSingleThread();
	} else if (t > 1) {
		solveMultyThread();
	} else {
		throw invalid_argument("Wrong thread count");
	}
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
	delete[] costTable_15_puzzle_0;
	delete[] costTable_15_puzzle_1;
	delete[] costTable_15_puzzle_2;
	printf("Memory is free\n");
}


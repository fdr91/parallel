/*
 * PuzzleSolver.cpp
 *
 *  Created on: Dec 19, 2014
 *      Author: fedor
 */

#include "PuzzleSolver.h"
#include <stdexcept>
#include <fstream>

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
	if (!solved)
		ret = true;
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
	tilePositions = {-1, 0, 0, 1, 2, 1, 2, 0, 1, 3, 4, 2, 3, 5,
		4, 5};
	tileSubsets = {-1, 1, 0, 0, 0, 1, 1, 2, 2, 1, 1, 2, 2, 1, 2,
		2};
	costTable_15_puzzle_0 = NULL;
	costTable_15_puzzle_1 = NULL;
	costTable_15_puzzle_2 = NULL;
	threadCount = -1;
	this->running_mutex=PTHREAD_MUTEX_INITIALIZER;
	solved = false;
}

PuzzleSolver::PuzzleSolver(BoardState& p) {
	this->running_mutex=PTHREAD_MUTEX_INITIALIZER;
	solved = p.isGoal();
	threadCount = -1;
	puzzle = BoardState(p);
	state = BoardState(p);
	costTable_15_puzzle_0 = NULL;
	costTable_15_puzzle_1 = NULL;
	costTable_15_puzzle_2 = NULL;
	tilePositions = [-1, 0, 0, 1, 2, 1, 2, 0, 1, 3, 4, 2, 3, 5,
		4, 5];
	tileSubsets = {-1, 1, 0, 0, 0, 1, 1, 2, 2, 1, 1, 2, 2, 1, 2,
		2};

}

void PuzzleSolver::solveSingleThread() {

}

void PuzzleSolver::solve(int t) {
	setInitial();
	if (t == 1) {
		solveSingleThread();
	} else if (t > 1) {
		printf("Not implemented\n");
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
}


/*
 * PuzzleSolver.h
 *
 *  Created on: Nov 14, 2014
 *      Author: fedor
 */

#ifndef PUZZLESOLVER_H_
#define PUZZLESOLVER_H_

#include <vector>
#include <string>
#include "IDAStar.h"

std::vector<char> getTileArray(std::string* tileOrder);

class PuzzleSolver {

	static int dimension;
	static int numOfTiles;
	static int numOfTilesMinusOne;
	static uint64_t goalState;
	static uint64_t goalStatePositions;

	static void initializeGoalState(int numOfTiles);
	static void loadStreamCostTable(const std::string filename, char* costTable,
			int size);

public:
	static IDAStar algorithm;
	static bool isVerbose;
	static char* costTable_15_puzzle_0;
	static char* costTable_15_puzzle_1;
	static char* costTable_15_puzzle_2;

	static void setVerbose(bool b);
	static bool getVerbose();

	static void initialize(int n);

	static int getDimension();
	static int getNumOfTiles();
	static uint64_t getGoalState();
	static uint64_t getGoalStatePositions();
	static IDAStar getAlgorithm();
	PuzzleSolver();
	PuzzleSolver(std::vector<char> state);
	virtual ~PuzzleSolver();
};

#endif /* PUZZLESOLVER_H_ */

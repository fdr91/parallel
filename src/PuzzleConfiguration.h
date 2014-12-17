/*
 * PuzzleConfiguration.h
 *
 *  Created on: Nov 14, 2014
 *      Author: fedor
 */

#ifndef PUZZLECONFIGURATION_H_
#define PUZZLECONFIGURATION_H_
#include <stdint.h>
#include <string>
#include "IDAStar.h"

class PuzzleConfiguration {

	static int dimension;
	static int numOfTiles;
	static int numOfTilesMinusOne;
	static uint64_t goalState;
	static uint64_t goalStatePositions;

	static void initializeGoalState(int numOfTiles);
	static void loadStreamCostTable(const std::string filename, char* costTable, int size);



public:
	static IDAStar algorithm;
	static bool isVerbose;
	static char* costTable_15_puzzle_0;
	static char* costTable_15_puzzle_1;
	static char* costTable_15_puzzle_2;

	PuzzleConfiguration();
	static void setVerbose(bool b);
	static bool getVerbose();

	static void initialize(int n);

	static int getDimension();
	static int getNumOfTiles();
	static uint64_t getGoalState();
	static uint64_t getGoalStatePositions();

	virtual ~PuzzleConfiguration();
	static IDAStar getAlgorithm();
};

#endif /* PUZZLECONFIGURATION_H_ */

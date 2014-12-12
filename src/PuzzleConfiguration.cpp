/*
 * PuzzleConfiguration.cpp
 *
 *  Created on: Nov 14, 2014
 *      Author: fedor
 */

#include "PuzzleConfiguration.h"
#include "Utility.h"
#include <math.h>
#include <stdint.h>
#include <string>
#include <fstream>
#include <iostream>
#include "IDAStar.h"
#include <stdio.h>

using namespace std;

PuzzleConfiguration::PuzzleConfiguration() {
	// TODO Auto-generated constructor stub

}

bool PuzzleConfiguration::isVerbose;
int PuzzleConfiguration::dimension;
int PuzzleConfiguration::numOfTiles;
int PuzzleConfiguration::numOfTilesMinusOne;
uint64_t PuzzleConfiguration::goalState;
uint64_t PuzzleConfiguration::goalStatePositions;

char* PuzzleConfiguration::costTable_15_puzzle_0 = new char[4096];
char* PuzzleConfiguration::costTable_15_puzzle_1 = new char[16777216];
char* PuzzleConfiguration::costTable_15_puzzle_2 = new char[16777216];

void PuzzleConfiguration::setVerbose(bool isVerbose) {
	PuzzleConfiguration::isVerbose = true;
}

PuzzleConfiguration::~PuzzleConfiguration() {
	// TODO Auto-generated destructor stub
}

void PuzzleConfiguration::initializeGoalState(int numOfTiles) {
	PuzzleConfiguration::goalState = 0;
	const int numOfTilesMinusOne = numOfTiles - 1;
	for (int i = 0; i < numOfTiles; ++i) {
		if (i != numOfTilesMinusOne) {
			int iPlusOne = i + 1;
			goalState |= ((uint64_t) iPlusOne << (i << 2));
		} else {
			goalState |= ((uint64_t) 0 << (i << 2));
		}
	}

	goalStatePositions = getPositionsAsUint64_t(goalState, numOfTilesMinusOne);
}

IDAStar PuzzleConfiguration::algorithm;

IDAStar PuzzleConfiguration::getAlgorithm(){
	return PuzzleConfiguration::algorithm;
}

void PuzzleConfiguration::initialize(int n) {
	numOfTiles = n;
	numOfTilesMinusOne = n - 1;
	dimension = sqrt(n);
	initializeGoalState(numOfTiles);
    loadStreamCostTable(string("databases/15-puzzle-663-0.db"), costTable_15_puzzle_0, 4096);
    loadStreamCostTable(string("databases/15-puzzle-663-1.db"), costTable_15_puzzle_1, 16777216);
    loadStreamCostTable(string("databases/15-puzzle-663-2.db"), costTable_15_puzzle_2, 16777216);
}

bool PuzzleConfiguration::getVerbose() {
	return isVerbose;
}

int PuzzleConfiguration::getDimension() {
	return dimension;
}

int PuzzleConfiguration::getNumOfTiles() {
	return numOfTiles;
}

uint64_t PuzzleConfiguration::getGoalState() {
	return goalState;
}

uint64_t PuzzleConfiguration::getGoalStatePositions() {
	return goalStatePositions;
}

void PuzzleConfiguration::loadStreamCostTable(const string filename,
		char* costTable, int size) {

	ifstream is(filename);
	if(is.good()){
		is.read(costTable, size);
		is.close();
	} else {
		throw 1;
	}
}

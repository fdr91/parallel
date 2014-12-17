/*
 * Node.cpp
 *
 *  Created on: Nov 14, 2014
 *      Author: fedor
 */

#include "Node.h"

#include <stdexcept>

#include "PuzzleConfiguration.h"

using std::invalid_argument;

const int Node::tilePositions[] = { -1, 0, 0, 1, 2, 1, 2, 0, 1, 3, 4, 2, 3, 5,
		4, 5 };
const int Node::tileSubsets[] = { -1, 1, 0, 0, 0, 1, 1, 2, 2, 1, 1, 2, 2, 1, 2,
		2 };

int Node::dimension;
int Node::numOfTiles;
int Node::numOfTilesMinusOne;
uint64_t Node::goalState;
uint64_t Node::goalStatePositions;

void Node::initialize() {
	dimension = PuzzleConfiguration::getDimension();
	numOfTiles = PuzzleConfiguration::getNumOfTiles();
	numOfTilesMinusOne = Node::numOfTiles - 1;
	goalState = PuzzleConfiguration::getGoalState();
	goalStatePositions = PuzzleConfiguration::getGoalStatePositions();

}

int Node::h(const uint64_t boardConfig) {

	if (numOfTiles == 16) {
		// Create three different indexes that contain only the positions of
		// tiles applicable to the corresponding pattern database.
		int index0 = 0, index1 = 0, index2 = 0;
		for (int pos = numOfTilesMinusOne; pos >= 0; --pos) {
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
		return PuzzleConfiguration::costTable_15_puzzle_0[index0]
					+ PuzzleConfiguration::costTable_15_puzzle_1[index1]
					+ PuzzleConfiguration::costTable_15_puzzle_2[index2];
	}
	else throw invalid_argument("Wrong puzzle size");
	return 0;
}

int Node::posOfSpace(const uint64_t boardConfig) {
    for (int i = numOfTiles - 1; i >= 0; --i) {
        if ((char)((boardConfig >> (i << 2)) & 0xF) == 0) {
            return i;
        }
    }
    return -1;
}

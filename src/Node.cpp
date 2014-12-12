/*
 * Node.cpp
 *
 *  Created on: Nov 14, 2014
 *      Author: fedor
 */

#include "Node.h"

#include <stdexcept>

#include "PuzzleSolver.h"

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
	dimension = PuzzleSolver::getDimension();
	numOfTiles = PuzzleSolver::getNumOfTiles();
	numOfTilesMinusOne = Node::numOfTiles - 1;
	goalState = PuzzleSolver::getGoalState();
	goalStatePositions = PuzzleSolver::getGoalStatePositions();

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
		return PuzzleSolver::costTable_15_puzzle_0[index0]
					+ PuzzleSolver::costTable_15_puzzle_1[index1]
					+ PuzzleSolver::costTable_15_puzzle_2[index2];
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

uint64_t Node::moveUp(uint64_t boardConfig, uint64_t posOfSpace) {
	if (posOfSpace < Node::dimension) {
		return 0;
	}
	// Swap tile with space.
	const uint64_t posTimes4 = posOfSpace << 2, posMinusDimTimes4 = (posOfSpace
			- Node::dimension) << 2;
	const uint64_t space = (boardConfig >> posTimes4) & 0xF, tile = (boardConfig
			>> posMinusDimTimes4) & 0xF;

	const uint64_t zeroBitTile = (uint64_t) 0xF << posMinusDimTimes4;
	uint64_t ret =(boardConfig & ~zeroBitTile) | (tile << posTimes4)
			| (space << posMinusDimTimes4);
	return ret;
}
uint64_t Node::moveDown(uint64_t boardConfig, uint64_t posOfSpace) {
    if (posOfSpace >= Node::numOfTiles - Node::dimension) {
        return 0;
    }
    // Swap tile with space.
    const uint64_t posTimes4 = posOfSpace << 2,
              posPlusDimTimes4 = (posOfSpace + Node::dimension) << 2;
    const uint64_t space = (boardConfig >> posTimes4) & 0xF,
               tile = (boardConfig >> posPlusDimTimes4) & 0xF;

    const uint64_t zeroBitTile = (uint64_t)0xF << posPlusDimTimes4;
    uint64_t ret=(boardConfig & ~zeroBitTile) | (tile << posTimes4) |
           (space << posPlusDimTimes4);
    return ret;
}
uint64_t Node::moveLeft(uint64_t boardConfig, uint64_t posOfSpace) {
	if (posOfSpace % Node::dimension == 0) {
		return 0;
	}
	// Swap tile with space.
	const uint64_t posTimes4 = posOfSpace << 2, posMinusOneTimes4 = (posOfSpace
			- 1) << 2;
	const uint64_t space = (boardConfig >> posTimes4) & 0xF, tile = (boardConfig
			>> posMinusOneTimes4) & 0xF;

	const uint64_t zeroBitTile = (uint64_t) 0xF << posMinusOneTimes4;
	uint64_t ret = (boardConfig & ~zeroBitTile) | (tile << posTimes4)
			| (space << posMinusOneTimes4);
	return ret;
}
uint64_t Node::moveRight(uint64_t boardConfig, uint64_t posOfSpace) {
    const uint64_t posPlusOne = posOfSpace + 1;
    if (posPlusOne % Node::dimension == 0) {
        return 0;
    }
    // Swap tile with space.
    const uint64_t posTimes4 = posOfSpace << 2,
              posPlusOneTimes4 = posPlusOne << 2;
    const uint64_t space = (boardConfig >> posTimes4) & 0xF,
               tile = (boardConfig >> posPlusOneTimes4) & 0xF;

    const uint64_t zeroBitTile = (uint64_t)0xF << posPlusOneTimes4;
    uint64_t ret = (boardConfig & ~zeroBitTile) | (tile << posTimes4) |
            (space << posPlusOneTimes4);
    return ret;
}


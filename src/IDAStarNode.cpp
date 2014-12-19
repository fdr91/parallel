/*
 * IDAStarNode.cpp
 *
 *  Created on: Nov 21, 2014
 *      Author: fedor
 */

#include "IDAStarNode.h"
#include "Node.h"
#include "globals.h"

IDAStarNode::IDAStarNode() {
	// TODO Auto-generated constructor stub

}

IDAStarNode::~IDAStarNode() {
	// TODO Auto-generated destructor stub
}

uint64_t IDAStarNode::moveUp(uint64_t boardConfig, uint64_t posOfSpace) {
	if (posOfSpace < DIMENSION) {
		return 0;
	}
	// Swap tile with space.
	const uint64_t posTimes4 = posOfSpace << 2, posMinusDimTimes4 = (posOfSpace
			- DIMENSION) << 2;
	const uint64_t space = (boardConfig >> posTimes4) & 0xF, tile = (boardConfig
			>> posMinusDimTimes4) & 0xF;

	const uint64_t zeroBitTile = (uint64_t) 0xF << posMinusDimTimes4;
	uint64_t ret =(boardConfig & ~zeroBitTile) | (tile << posTimes4)
			| (space << posMinusDimTimes4);
	return ret;
}
uint64_t IDAStarNode::moveDown(uint64_t boardConfig, uint64_t posOfSpace) {
    if (posOfSpace >= NUM_OF_TILES - DIMENSION) {
        return 0;
    }
    // Swap tile with space.
    const uint64_t posTimes4 = posOfSpace << 2,
              posPlusDimTimes4 = (posOfSpace + DIMENSION) << 2;
    const uint64_t space = (boardConfig >> posTimes4) & 0xF,
               tile = (boardConfig >> posPlusDimTimes4) & 0xF;

    const uint64_t zeroBitTile = (uint64_t)0xF << posPlusDimTimes4;
    uint64_t ret=(boardConfig & ~zeroBitTile) | (tile << posTimes4) |
           (space << posPlusDimTimes4);
    return ret;
}
uint64_t IDAStarNode::moveLeft(uint64_t boardConfig, uint64_t posOfSpace) {
	if (posOfSpace % DIMENSION == 0) {
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
uint64_t IDAStarNode::moveRight(uint64_t boardConfig, uint64_t posOfSpace) {
    const uint64_t posPlusOne = posOfSpace + 1;
    if (posPlusOne % DIMENSION == 0) {
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

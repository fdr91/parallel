/*
 * Node.cpp
 *
 *  Created on: Nov 14, 2014
 *      Author: fedor
 */

#include "Node.h"

#include <stdexcept>
#include <string>
#include <fstream>
#include "globals.h"


using std::invalid_argument;

const int Node::tilePositions[] = { -1, 0, 0, 1, 2, 1, 2, 0, 1, 3, 4, 2, 3, 5,
		4, 5 };
const int Node::tileSubsets[] = { -1, 1, 0, 0, 0, 1, 1, 2, 2, 1, 1, 2, 2, 1, 2,
		2 };

void Node::loadStreamCostTable(const std::string filename,
		char* costTable, int size) {

	std::ifstream is(filename.c_str());
	if(is.good()){
		is.read(costTable, size);
		is.close();
	} else {
		throw 1;
	}
}

char*costTable_15_puzzle_0;//[4096];
char* costTable_15_puzzle_1;//[16777216];
char* costTable_15_puzzle_2;//[16777216];

void Node::initialize() {

	costTable_15_puzzle_0 = new char[4096];
	costTable_15_puzzle_1 = new char[16777216];
	costTable_15_puzzle_2 = new char[16777216];
    loadStreamCostTable(std::string("databases/15-puzzle-663-0.db"), costTable_15_puzzle_0, 4096);
    loadStreamCostTable(std::string("databases/15-puzzle-663-1.db"), costTable_15_puzzle_1, 16777216);
    loadStreamCostTable(std::string("databases/15-puzzle-663-2.db"), costTable_15_puzzle_2, 16777216);

}

void Node::clean(){
	delete [] costTable_15_puzzle_0;
	delete [] costTable_15_puzzle_1;
	delete [] costTable_15_puzzle_2;
}

int Node::h(const int64_t boardConfig) {

	if (NUM_OF_TILES == 16) {
		// Create three different indexes that contain only the positions of
		// tiles applicable to the corresponding pattern database.
		int index0 = 0, index1 = 0, index2 = 0;
		for (int pos = NUM_OF_TILES_MIMNUS_ONE; pos >= 0; --pos) {
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
		return costTable_15_puzzle_0[index0]
					+ costTable_15_puzzle_1[index1]
					+ costTable_15_puzzle_2[index2];
	}
	else throw invalid_argument("Wrong puzzle size");
	return 0;
}

int Node::posOfSpace(const int64_t boardConfig) {
    for (int i = NUM_OF_TILES - 1; i >= 0; --i) {
        if ((char)((boardConfig >> (i << 2)) & 0xF) == 0) {
            return i;
        }
    }
    return -1;
}

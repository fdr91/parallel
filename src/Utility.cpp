/*
 * Utility.cpp
 *
 *  Created on: Nov 14, 2014
 *      Author: fedor
 */

#include "Utility.h"
#include <iterator>
#include <string>
#include <stdlib.h>
#include <math.h>
#include <stdexcept>
#include <algorithm>    // std::swap
#include <iostream>
#include <stdlib.h>
#include "PuzzleConfiguration.h"

using namespace std;

std::vector<std::string> inline stringSplit(const std::string &source,
		const char *delimiter = " ", bool keepEmpty = false) {
	std::vector<std::string> results;

	size_t prev = 0;
	size_t next = 0;

	while ((next = source.find_first_of(delimiter, prev)) != std::string::npos) {
		if (keepEmpty || (next - prev != 0)) {
			results.push_back(source.substr(prev, next - prev));
		}
		prev = next + 1;
	}

	if (prev < source.size()) {
		results.push_back(source.substr(prev));
	}

	return results;
}

std::vector<char> getTileArray(std::string* tileOrder) {
	vector<string> tokens = stringSplit(*tileOrder, ",");

	if (tokens.size() != 16)
		throw invalid_argument("Wrong tiles");

	std::vector<char> array;
	for (string str : tokens) {
		int val = atoi(str.c_str());
		if (val > 15 || val < 0)
			throw invalid_argument("Wrong tiles");
		array.push_back((char) val);
	}

	if (!isValidPermutation(array))
		throw invalid_argument("Wrong tiles");

	return array;
}

uint64_t getPositionsAsUint64_t(const uint64_t boardConfig,
		const int numOfTilesMinusOne) {
	// Creates a long that maps tiles(i.e. array index) to position in puzzle.
	uint64_t positions = 0;
	for (int pos = numOfTilesMinusOne; pos >= 0; --pos) {
		const int tile = (int) ((boardConfig >> (pos << 2)) & 0xF);
		positions |= ((uint64_t) pos << (tile << 2));
	}
	return positions;
}

bool isValidPermutation(std::vector<char> state) {
	int numOfTiles = state.size();
	int inversions = 0;
	int dim = (int) sqrt(numOfTiles);

	for (int i = 0; i < numOfTiles; ++i) {
		char iTile = state[i];
		if (iTile != 0) {
			for (int j = i + 1; j < numOfTiles; ++j) {
				char jTile = state[j];
				if (jTile != 0 && jTile < iTile) {
					++inversions;
				}
			}
		} else {
			if ((dim & 0x1) == 0) {
				inversions += (1 + i / dim);
			}
		}
	}
	if ((inversions & 0x1) == 1)
		return false;
	return true;
}

uint64_t arrayToLong(std::vector<char> state) {
	uint64_t value = 0;
	for (int i = 0; i < state.size(); ++i) {
		value |= ((uint64_t) state[i] << (i << 2));
	}
	return value;
}

string byteArrayToString(std::vector<char> state){
    int numOfTiles = state.size();
    string builder;
    for (int i = 0; i < numOfTiles; ++i) {
        if (i != 0) {
            builder.append(",");
        }
        char buf[4];
        sprintf(buf, "%d", state[i]);
        builder.append(buf);
    }
    return builder;

}

void swap(const int i, const int j, char* A) {
	const char temp = A[i];
	A[i] = A[j];
	A[j] = temp;
}

char* getMovedTiles(string pathStr, std::vector<char> initState, char* movedTiles){
	 const int pathLength = pathStr.length(),
	                  boardLength = initState.size(),
	                  dimension = PuzzleConfiguration::getDimension();
	        //char movedTiles[pathLength];
	        char boardConfig[boardLength];

	        int posOfSpace = -1;
	        for (int i = boardLength - 1; i >= 0; --i) {
	            const char tile = initState[i];
	            if (tile == 0) {
	                posOfSpace = i;
	            }
	            boardConfig[i] = tile;
	        }

	        for (int i = 0; i < pathLength; ++i) {
	            const char dir = pathStr.at(i);
	            int posOfTile;
	            if (dir == 'L') {
	                posOfTile = posOfSpace - 1;
	            } else if (dir == 'R') {
	                posOfTile = posOfSpace + 1;
	            } else if (dir == 'U') {
	                posOfTile = posOfSpace - dimension;
	            } else {
	                posOfTile = posOfSpace + dimension;
	            }
	            movedTiles[i] = boardConfig[posOfTile];
	            swap(posOfSpace, posOfTile, boardConfig);
	            posOfSpace = posOfTile;
	        }
	        return movedTiles;
}

std::vector<string> getDirections(std::vector<char> initState){
	const int pathLength = IDAStar::shortestPath.length();
	std::vector<string> directions;
	if (pathLength != 0) {
		char tiles[pathLength];
		getMovedTiles(IDAStar::shortestPath, initState, tiles);
		for (int i = 0; i < pathLength; ++i) {
			const char tile = tiles[i];
			const char dir = IDAStar::shortestPath.at(i);
			string direction;
			if (dir == 'L') {
				direction = "right";
			} else if (dir == 'R') {
				direction = "left";
			} else if (dir == 'U') {
				direction = "down";
			} else {
				direction = "up";
			}
			//const StringBuilder builder = new StringBuilder();
			const int iPlusOne = i + 1;
			char buffer[1024];
			sprintf(buffer, "%2d. %d - %s", iPlusOne, tile, direction.c_str());
			directions.push_back((string)buffer);
		}
	}
	return directions;
}

void displayStats(std::vector<char> initState) {
	const
	int numOfTiles = initState.size();
	cout<<endl;
	cout<<"Puzzle type:          " <<(numOfTiles - 1)  << "-puzzle\n";
	cout<<"Initial permutation:  ";
	cout<<byteArrayToString(initState);
	cout<<"\nGoal state:           ";
	for (int i = 0; i < numOfTiles; ++i) {
		if (i != 0) {
			cout<<",";
		}
		cout<<((PuzzleConfiguration::getGoalState() >> (i << 2)) & 0xF);
	}
	cout<<endl;
	//cout<<	"Elapsed time:         " << DEC_FORMATTER.format(IDAStar.getRunningTimeInSeconds()) << " s";
	//cout<<	"Paths visited:        "	+ INT_FORMATTER.format(IDAStar.numberVisited));
	//cout<<
			//"States explored:      "
		//			+ INT_FORMATTER.format(IDAStar.numberExpanded));
	const int numOfMoves = IDAStar::shortestPath.length();
	if (numOfMoves == 1) {
		cout<<"Shortest path:        " << numOfMoves <<" move";
	} else {
		cout<<"Shortest path:        " <<numOfMoves << " moves\n";
	}
	std::vector<string> directions = getDirections(initState);
	for (int i = 0; i < directions.size(); ++i) {
		cout<<directions[i]<<endl;
	}
}

/*
 * BoardState.cpp
 *
 *  Created on: Dec 19, 2014
 *      Author: fedor
 */

#include "BoardState.h"
#include <math.h>
#include <sstream>
#include <stdexcept>
#include <cstdlib>
//#include <swap>

using namespace std;

BoardState::BoardState() {
	state=0;
}

bool isValidPermutation(std::vector<char> *check){
	std::vector<char> state=*check;
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


void BoardState::getTileArray(std::string* tileOrder, std::vector<char> *tileArray) {
	istringstream f(*tileOrder);
	string s;
	while(getline(f, s, ',')){
		tileArray->push_back((char)atoi(s.c_str()));
	}

	if (tileArray->size() != 16)
		throw invalid_argument("Wrong tiles");

	for(int i=0; i<tileArray->size(); i++){
		char val = tileArray->at(i);
		if (val > 15 || val < 0)
			throw invalid_argument("Wrong tiles");
	}

	if (!isValidPermutation(tileArray))
		throw invalid_argument("Wrong tiles");
	return;
}

int64_t BoardState::arrayToLong(std::vector<char> state) {
	int64_t value = 0;
	for (int i = 0; i < state.size(); ++i) {
		value |= ((int64_t) state[i] << (i << 2));
	}
	return value;
}

void BoardState::set(int64_t t){
	this->state=t;
}

BoardState::BoardState(const BoardState& v) {
	this->state = v.state;
}

int BoardState::posOfSpace(){
    for (int i = 16 - 1; i >= 0; --i) {
        if (((this->state >> (i << 2)) & 0xF) == 0) {
            return i;
        }
    }
    return -1;
}

BoardState::BoardState(std::string str){
	std::vector<char> tileArray;
	getTileArray(&str, &tileArray);
	this->state=arrayToLong(tileArray);
}


BoardState::BoardState(const char* cstr){
	string str(cstr);
	std::vector<char> tileArray;
	getTileArray(&str, &tileArray);
	this->state=arrayToLong(tileArray);
}


BoardState::~BoardState() {
}

BoardState BoardState::moveUp() {
	int64_t spaceP = posOfSpace();
	if (spaceP < 4) {
		return BoardState((int64_t)0);
	}
	// Swap tile with space.
	const uint64_t posTimes4 = spaceP << 2, posMinusDimTimes4 = (spaceP
			- 4) << 2;
	const uint64_t space = (state >> posTimes4) & 0xF, tile = (state
			>> posMinusDimTimes4) & 0xF;

	const uint64_t zeroBitTile = (uint64_t) 0xF << posMinusDimTimes4;
	uint64_t ret =(state & ~zeroBitTile) | (tile << posTimes4)
			| (space << posMinusDimTimes4);
	return BoardState(ret);
}
BoardState BoardState::moveDown() {
	int64_t spaceP = posOfSpace();
    if (spaceP >= size- 4) {
        return BoardState((int64_t)0);
    }
    // Swap tile with space.
    const uint64_t posTimes4 = spaceP << 2,
              posPlusDimTimes4 = (spaceP + 4) << 2;
    const uint64_t space = (state >> posTimes4) & 0xF,
               tile = (state >> posPlusDimTimes4) & 0xF;

    const uint64_t zeroBitTile = (uint64_t)0xF << posPlusDimTimes4;
    uint64_t ret=(state & ~zeroBitTile) | (tile << posTimes4) |
           (space << posPlusDimTimes4);
    return BoardState(ret);
}
BoardState BoardState::moveLeft() {
	int64_t spaceP = posOfSpace();
	if (spaceP % 4  == 0) {
		return BoardState((int64_t)0);
	}
	// Swap tile with space.
	const uint64_t posTimes4 = spaceP << 2, posMinusOneTimes4 = (spaceP
			- 1) << 2;
	const uint64_t space = (state >> posTimes4) & 0xF, tile = (state
			>> posMinusOneTimes4) & 0xF;

	const uint64_t zeroBitTile = (uint64_t) 0xF << posMinusOneTimes4;
	uint64_t ret = (state & ~zeroBitTile) | (tile << posTimes4)
			| (space << posMinusOneTimes4);
	return BoardState(ret);
}
BoardState BoardState::moveRight() {
	int64_t spaceP = posOfSpace();
    const uint64_t posPlusOne = spaceP + 1;
    if (posPlusOne % 4  == 0) {
        return BoardState((int64_t)0);
    }
    // Swap tile with space.
    const uint64_t posTimes4 = spaceP << 2,
              posPlusOneTimes4 = posPlusOne << 2;
    const uint64_t space = (state >> posTimes4) & 0xF,
               tile = (state >> posPlusOneTimes4) & 0xF;

    const uint64_t zeroBitTile = (uint64_t)0xF << posPlusOneTimes4;
    uint64_t ret = (state & ~zeroBitTile) | (tile << posTimes4) |
            (space << posPlusOneTimes4);
    return BoardState(ret);

}

BoardState::BoardState(int64_t t){
	this->state = t;
}


/*void BoardState::swap(BoardState & b)
{
	std::swap(this->goal, b.goal);
	std::swap(this->size, b.size);
	std::swap(this->state, b.state);
}*/

BoardState & BoardState::operator = (BoardState const & num)
{
	if(this != &num)
		this->state=num.state;
	return *this;
}

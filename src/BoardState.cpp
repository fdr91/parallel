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
	vector<string> tokens;// = stringSplit(*tileOrder, ",");
	istringstream f(*tileOrder);
	string s;
	while(getline(f, s, ';')){
		tokens.push_back(s);
	}

	if (tokens.size() != 16)
		throw invalid_argument("Wrong tiles");

	for(std::vector<string>::iterator it = tokens.begin(); it!=tokens.end(); it++){
		int val = atoi((*it).c_str());
		if (val > 15 || val < 0)
			throw invalid_argument("Wrong tiles");
		tileArray->push_back((char) val);
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


BoardState::BoardState(std::string str){
	std::vector<char> tileArray;
	getTileArray(&str, &tileArray);
	this->state=arrayToLong(tileArray);
}

BoardState::~BoardState() {
}


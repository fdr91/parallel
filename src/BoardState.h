/*
 * BoardState.h
 *
 *  Created on: Dec 19, 2014
 *      Author: fedor
 */

#ifndef BOARDSTATE_H_
#define BOARDSTATE_H_

#include <stdint.h>
#include <string>
#include <vector>

class BoardState {
	int64_t state;
	void getTileArray(std::string* tileOrder, std::vector<char> *tileArray);
	int64_t arrayToLong(std::vector<char> state);
public:
	BoardState();
	BoardState(std::string str);
	virtual ~BoardState();
};

#endif /* BOARDSTATE_H_ */

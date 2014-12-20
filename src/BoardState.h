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
	const static int64_t goal=1147797409030816545;
	const static int64_t size=16;

	int64_t arrayToLong(std::vector<char> state);
public:
	static void getTileArray(std::string* tileOrder, std::vector<char> *tileArray);
	BoardState moveUp();
	BoardState moveDown();
	BoardState moveLeft();
	BoardState moveRight();

	BoardState();
	BoardState(const char* cstr);
	BoardState(std::string str);
	BoardState(int64_t);
	int posOfSpace();
	int64_t getLong() {return state;};
	BoardState(const BoardState& val);
	bool isGoal(){return goal==state;};
	virtual ~BoardState();
};

#endif /* BOARDSTATE_H_ */

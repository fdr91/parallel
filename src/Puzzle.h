/*
 * Puzzle.h
 *
 *  Created on: Dec 19, 2014
 *      Author: fedor
 */

#ifndef PUZZLE_H_
#define PUZZLE_H_

#include "BoardState.h"

class Puzzle {
	BoardState state;
public:
	int solve();
	Puzzle();
	Puzzle(char* initialState);
	virtual ~Puzzle();
};

#endif /* PUZZLE_H_ */

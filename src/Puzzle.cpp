/*
 * Puzzle.cpp
 *
 *  Created on: Dec 19, 2014
 *      Author: fedor
 */

#include "Puzzle.h"
#include <string.h>

using namespace std;

Puzzle::Puzzle(char* initialState){
	string s(initialState);
	this->state = BoardState(s);
}



Puzzle::Puzzle() {}
Puzzle::~Puzzle() {}

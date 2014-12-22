/*
 * Path.cpp
 *
 *  Created on: Dec 19, 2014
 *      Author: fedor
 */

#include "Path.h"
#include "BoardState.h"
#include <algorithm>
#include <cstdlib>
#include <stdio.h>
#include <algorithm>

using namespace std;

Path::Path(BoardState& bs) {

	nll = false;
	this->state = BoardState(bs);
	this->p+=('X');
	this->direction = 'X';
}

int64_t Path::stateAsL() {
	return state.getLong();
}

Path::Path() {
	nll = false;
	this->direction = 'X';
}

Path::Path(const Path& path) {
	nll = false;
	this->p = path.p;
	this->direction = path.direction;
	this->state=path.state;
}

bool Path::isSolved() {
	return state.isGoal();
}

int Path::size() {
	return p.size();
}

void Path::setPath(std::string& p){
	this->p= p;
}

Path::~Path() {}

void Path::set(int index, char val) {
	if (this->p.size() < (size_t) index + 1) {
		p.resize(index + 1);
	}
	p.at(index) = val;
}

char Path::get(int i) {
	return p.at(i);
}

void Path::append(char c){
	this->p+=c;
}

std::vector<std::string> Path::getDirections(string& initState) {
	int pathLength = p.length();
	if(*(p.begin())=='X'){
		p.erase(0, 1);
		--pathLength;
	}
	vector<string> directions;
	if (pathLength != 0) {
		string movedTiles;
		getMovedTiles(p, initState, &movedTiles);
		for (int i = 0; i < pathLength; ++i) {
			const char tile = movedTiles.at(i);
			const char dir = p.at(i);
			string direction;
			if (dir == 'L') {
				direction = string("right");
			} else if (dir == 'R') {
				direction = string("left");
			} else if (dir == 'U') {
				direction = string("down");
			} else if (dir == 'D') {
				direction = string("up");
			} else {
				printf("Throw in Path");
				throw 0;
			}
			const int iPlusOne = i + 1;
			char buffer[1024];
			sprintf(buffer, "%2d. %d - %s", iPlusOne, tile, direction.c_str());
			directions.push_back(string(buffer));
		}
	}
	return directions;
}

void Path::getMovedTiles(std::string& pathStr, string _initState,
		string* movedTiles) {

	vector<char> initState;
	BoardState::getTileArray(&_initState,&initState);
	const int pathLength = pathStr.size(), boardLength = initState.size(),
			dimension1 = 4;
	std::string boardConfig;

	int posOfSpace = -1;
	boardConfig.resize(boardLength);
	movedTiles->resize(pathLength);
	for (int i = boardLength - 1; i >= 0; --i) {
		const char tile = initState.at(i);
		if (tile == 0)
			posOfSpace = i;
		boardConfig.at(i) = tile;
	}
	movedTiles->resize(pathLength);
	for (int i = 0; i < pathLength; ++i) {
		const char dir = pathStr.at(i);
		int posOfTile;
		if (dir == 'L') {
			posOfTile = posOfSpace - 1;
		} else if (dir == 'R') {
			posOfTile = posOfSpace + 1;
		} else if (dir == 'U') {
			posOfTile = posOfSpace - dimension1;
		} else if (dir == 'D') {
			posOfTile = posOfSpace + dimension1;
		} else {
			printf("Throw in Path2");
			throw 1;
		}
		movedTiles->at(i) = boardConfig.at(posOfTile);

		char tmp = boardConfig.at(posOfSpace);
		boardConfig.at(posOfSpace) = boardConfig.at(posOfTile);
		boardConfig.at(posOfTile) = tmp;
		posOfSpace = posOfTile;
	}
	return;
}

void Path::setState(int64_t s) {
	state.set(s);
}

void Path::setNull() {
	this->nll = true;
}

char Path::getDirection() {
	return this->direction;
}
void Path::setDirection(char c) {
	direction = c;
}

bool Path::isNull() {
	return nll;
}

#define DIMENSION 4
#define NUM_OF_TILES 16

void Path::cp(Path *ret) {
	ret->direction = this->direction;
	ret->p = this->p;
	ret->nll = this->nll;
	ret->state = this->state;
}


bool Path::moveLeft() {
	const int posOfSpace = state.posOfSpace();
	if (posOfSpace % DIMENSION == 0) {
		this->setNull();
		return false;
	}

	// Swap tile with space.
	const int posTimes4 = posOfSpace << 2, posMinusOneTimes4 = (posOfSpace - 1)
			<< 2;
	const int64_t space = (this->stateAsL() >> posTimes4) & 0xF, tile =
			(this->stateAsL() >> posMinusOneTimes4) & 0xF;

	const int64_t zeroBitTile = (int64_t) 0xF << posMinusOneTimes4;
	this->setState(
			(this->stateAsL() & ~zeroBitTile) | (tile << posTimes4)
					| (space << posMinusOneTimes4));
	this->direction = 'L';

	this->p += ('L');

	return true;
}

bool Path::moveLeftNode(Path* ret) {
	const int posOfSpace = state.posOfSpace();
	if (posOfSpace % DIMENSION == 0) {
		ret->setNull();
		return false;
	}

	// Swap tile with space.
	const int posTimes4 = posOfSpace << 2, posMinusOneTimes4 = (posOfSpace - 1)
			<< 2;
	const int64_t space = (ret->stateAsL() >> posTimes4) & 0xF, tile =
			(ret->stateAsL() >> posMinusOneTimes4) & 0xF;

	const int64_t zeroBitTile = (int64_t) 0xF << posMinusOneTimes4;
	ret->setState(
			(ret->stateAsL() & ~zeroBitTile) | (tile << posTimes4)
					| (space << posMinusOneTimes4));
	ret->direction = 'L';

	ret->p += ('L');

	return true;
}

bool Path::moveRight() {
	const int posOfSpace = state.posOfSpace(), posPlusOne = posOfSpace + 1;
	if (posPlusOne % DIMENSION == 0) {
		this->setNull();
		return false;
	}

	// Swap tile with space.
	const int posTimes4 = posOfSpace << 2, posPlusOneTimes4 = posPlusOne << 2;
	const int64_t space = (this->stateAsL() >> posTimes4) & 0xF, tile =
			(this->stateAsL() >> posPlusOneTimes4) & 0xF;

	const int64_t zeroBitTile = (int64_t) 0xF << posPlusOneTimes4;
	this->setState(
			(this->stateAsL() & ~zeroBitTile) | (tile << posTimes4)
					| (space << posPlusOneTimes4));
	this->direction = 'R';
	this->p += ('R');
	this->nll = false;
	return true;
}

bool Path::moveRightNode(Path* ret) {
	const int posOfSpace = state.posOfSpace(), posPlusOne = posOfSpace + 1;
	if (posPlusOne % DIMENSION == 0) {
		ret->setNull();
		return false;
	}
	cp(ret);

	// Swap tile with space.
	const int posTimes4 = posOfSpace << 2, posPlusOneTimes4 = posPlusOne << 2;
	const int64_t space = (ret->stateAsL() >> posTimes4) & 0xF, tile =
			(ret->stateAsL() >> posPlusOneTimes4) & 0xF;

	const int64_t zeroBitTile = (int64_t) 0xF << posPlusOneTimes4;
	ret->setState(
			(ret->stateAsL() & ~zeroBitTile) | (tile << posTimes4)
					| (space << posPlusOneTimes4));
	ret->direction = 'R';
	ret->p += ('R');
	ret->nll = false;
	return true;
}

bool Path::moveUp() {
	const int posOfSpace = state.posOfSpace();
	if (posOfSpace < DIMENSION) {
		this->nll = true;
		return false;
	}

	// Swap tile with space.
	const int posTimes4 = posOfSpace << 2, posMinusDimTimes4 = (posOfSpace
			- DIMENSION) << 2;
	const int64_t space = (this->stateAsL() >> posTimes4) & 0xF, tile =
			(this->stateAsL() >> posMinusDimTimes4) & 0xF;

	const int64_t zeroBitTile = (int64_t) 0xF << posMinusDimTimes4;
	this->setState( (this->stateAsL() & ~zeroBitTile) | (tile << posTimes4)
			| (space << posMinusDimTimes4));
	this->direction = 'U';
	this->p += ('U');
	this->nll = false;
	return true;
}

bool Path::moveUpNode(Path* ret) {
	const int posOfSpace = state.posOfSpace();
	if (posOfSpace < DIMENSION) {
		ret->nll = true;
		return false;
	}
	cp(ret);

	// Swap tile with space.
	const int posTimes4 = posOfSpace << 2, posMinusDimTimes4 = (posOfSpace
			- DIMENSION) << 2;
	const int64_t space = (ret->stateAsL() >> posTimes4) & 0xF, tile =
			(ret->stateAsL() >> posMinusDimTimes4) & 0xF;

	const int64_t zeroBitTile = (int64_t) 0xF << posMinusDimTimes4;
	ret->setState( (ret->stateAsL() & ~zeroBitTile) | (tile << posTimes4)
			| (space << posMinusDimTimes4));
	ret->direction = 'U';
	ret->p += ('U');
	ret->nll = false;
	return true;
}

std::string Path::getPath(){
	return p;
}

BoardState Path::getState(){
	return this->state;
}

bool Path::moveDown() {
	const int posOfSpace = state.posOfSpace();
	if (posOfSpace >= NUM_OF_TILES - DIMENSION) {
		this->nll = true;
		return false;
	}
	// Swap tile with space.
	const int posTimes4 = posOfSpace << 2, posPlusDimTimes4 = (posOfSpace
			+ DIMENSION) << 2;
	const int64_t space = (this->stateAsL() >> posTimes4) & 0xF, tile =
			(this->stateAsL() >> posPlusDimTimes4) & 0xF;

	const int64_t zeroBitTile = (int64_t) 0xF << posPlusDimTimes4;
	this->setState((this->stateAsL() & ~zeroBitTile) | (tile << posTimes4)
			| (space << posPlusDimTimes4));
	this->direction = 'D';
	this->p += ('D');
	this->nll = false;
	return true;
}

bool Path::moveDownNode(Path* ret) {
	const int posOfSpace = state.posOfSpace();
	if (posOfSpace >= NUM_OF_TILES - DIMENSION) {
		ret->nll = true;
		return false;
	}
	cp(ret);

	// Swap tile with space.
	const int posTimes4 = posOfSpace << 2, posPlusDimTimes4 = (posOfSpace
			+ DIMENSION) << 2;
	const int64_t space = (ret->stateAsL() >> posTimes4) & 0xF, tile =
			(ret->stateAsL() >> posPlusDimTimes4) & 0xF;

	const int64_t zeroBitTile = (int64_t) 0xF << posPlusDimTimes4;
	ret->setState((ret->stateAsL() & ~zeroBitTile) | (tile << posTimes4)
			| (space << posPlusDimTimes4));
	ret->direction = 'D';
	ret->p += ('D');
	ret->nll = false;
	return true;
}

bool Path::checkPath(std::string initState, std::string pstr) {
	Path p(BoardState(initState));

	for(std::string::iterator it = pstr.begin(); it!=pstr.end(); it++){
		switch(*it){
		case 'U':
			//printf("down\n");
			p.moveDown();
			break;
		case 'D':
			//printf("up\n");
			break;
		case 'R':
			//printf("left\n");
			break;
		case 'L':
			//printf("right\n");
			break;
		default: return false;
		}
	}
	return true;
}

Path & Path::operator = (Path const & num)
{
	if(this != &num){
		this->state=num.state;
		this->direction=num.direction;
		this->nll = num.nll;
		this->p = string(num.p);
	}
	return *this;
}

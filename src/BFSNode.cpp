/*
 * BFSNode.cpp
 *
 *  Created on: Nov 14, 2014
 *      Author: fedor
 */

#include "BFSNode.h"
#include <string>
#include <boost/functional/hash.hpp>

#define MAX_PATH 256

using namespace std;

BFSNode::BFSNode() {
	// TODO Auto-generated constructor stub
	isNull = false;

}

BFSNode::~BFSNode() {
	// TODO Auto-generated destructor stub
}

uint64_t BFSNode::getHash() {
	/*boost::hash<BFSNode> hash;
	boost::hash<boardConfig> string_hash;
	this->boardConfig;*/
	return boardConfig;
}

BFSNode::BFSNode(const uint64_t boardConfig) {
	this->boardConfig = boardConfig;
	this->storePath = false;
	this->cost = 0;
	isNull = false;
}

BFSNode::BFSNode(const uint64_t boardConfig, const bool storePath) {
	this->boardConfig = boardConfig;
	this->storePath = storePath;
	this->cost = 0;
	if (storePath) {
		string path;
		path += (direction);
		this->path = path;
	}
	isNull = false;
}

int BFSNode::serialize(char* buffer) {
	/*	int size = sizeof(boardConfig)+sizeof(cost)+sizeof(direction)+sizeof(isNull)+sizeof(storePath)+MAX_PATH*path.length();
	 buffer*/
	return -1;
}

BFSNode::BFSNode(const BFSNode* node) {
	this->boardConfig = node->boardConfig;
	this->direction = node->direction;
	this->cost = node->cost;
	this->storePath = node->storePath;
	if (storePath) {
		this->path = string(node->path);
	}
	isNull = node->isNull;
}

string BFSNode::getShortestPath() {
	return path.substr(1);
}

string BFSNode::getPath() {
	return path;
}

int BFSNode::posOfSpaceF() {
	return Node::posOfSpace(this->boardConfig);
}

bool BFSNode::moveLeftNode(const bool* tilesInSubset, BFSNode* ret) {
	const int posOfSpace = posOfSpaceF();
	if (posOfSpace % dimension == 0) {
		ret->isNull = true;
		return false;
	}
	cp(ret);

	// Swap tile with space.
	const int posTimes4 = posOfSpace << 2, posMinusOneTimes4 = (posOfSpace - 1)
			<< 2;
	const uint64_t space = (ret->boardConfig >> posTimes4) & 0xF, tile =
			(ret->boardConfig >> posMinusOneTimes4) & 0xF;

	const uint64_t zeroBitTile = (uint64_t) 0xF << posMinusOneTimes4;
	ret->boardConfig = ret->boardConfig & ~zeroBitTile | (tile << posTimes4)
			| (space << posMinusOneTimes4);
	ret->direction = 'L';
	if (storePath) {
		ret->path += ('L');
	}

	if (tilesInSubset == NULL || tilesInSubset[(int) tile]) {
		++ret->cost;
	}

	return true;
}

void BFSNode::cp(BFSNode* copy) {
	copy->boardConfig = this->boardConfig;
	copy->direction = this->direction;
	copy->cost = this->cost;
	copy->storePath = this->storePath;
	if (storePath) {
		copy->path = string(this->path);
	}
	isNull = this->isNull;
}

bool BFSNode::moveRightNode(const bool* tilesInSubset, BFSNode* ret) {
	const int posOfSpace = posOfSpaceF(), posPlusOne = posOfSpace + 1;
	if (posPlusOne % dimension == 0) {
		ret->isNull = true;
		return false;
	}
	cp(ret);


	// Swap tile with space.
	const int posTimes4 = posOfSpace << 2, posPlusOneTimes4 = posPlusOne << 2;
	const uint64_t space = (ret->boardConfig >> posTimes4) & 0xF, tile =
			(ret->boardConfig >> posPlusOneTimes4) & 0xF;

	const uint64_t zeroBitTile = (uint64_t) 0xF << posPlusOneTimes4;
	ret->boardConfig = ret->boardConfig & ~zeroBitTile | (tile << posTimes4)
			| (space << posPlusOneTimes4);
	ret->direction = 'R';
	if (storePath) {
		ret->path += ('R');
	}

	if (tilesInSubset == NULL || tilesInSubset[(int) tile]) {
		++ret->cost;
	}
	ret->isNull=false;
	return true;
}

bool BFSNode::moveUpNode(const bool* tilesInSubset, BFSNode* ret) {
	const int posOfSpace = posOfSpaceF();
	if (posOfSpace < dimension) {
		ret ->isNull=true;
		return false;
	}
	cp(ret);

	// Swap tile with space.
	const int posTimes4 = posOfSpace << 2, posMinusDimTimes4 = (posOfSpace
			- dimension) << 2;
	const uint64_t space = (ret->boardConfig >> posTimes4) & 0xF, tile =
			(ret->boardConfig >> posMinusDimTimes4) & 0xF;

	const uint64_t zeroBitTile = (uint64_t) 0xF << posMinusDimTimes4;
	ret->boardConfig = ret->boardConfig & ~zeroBitTile | (tile << posTimes4)
			| (space << posMinusDimTimes4);
	ret->direction = 'U';
	if (storePath) {
		ret->path += ('U');
	}

	if (tilesInSubset == NULL || tilesInSubset[(int) tile]) {
		++ret->cost;
	}
	ret ->isNull=false;
	return true;
}

bool BFSNode::moveDownNode(const bool* tilesInSubset, BFSNode* ret) {
	const int posOfSpace = posOfSpaceF();
	if (posOfSpace >= numOfTiles - dimension) {
		ret->isNull=true;
		return false;
	}
	cp(ret);

	// Swap tile with space.
	const int posTimes4 = posOfSpace << 2, posPlusDimTimes4 = (posOfSpace
			+ dimension) << 2;
	const uint64_t space = (ret->boardConfig >> posTimes4) & 0xF, tile =
			(ret->boardConfig >> posPlusDimTimes4) & 0xF;

	const uint64_t zeroBitTile = (uint64_t) 0xF << posPlusDimTimes4;
	ret->boardConfig = ret->boardConfig & ~zeroBitTile | (tile << posTimes4)
			| (space << posPlusDimTimes4);
	ret->direction = 'D';
	if (storePath) {
		ret->path += ('D');
	}

	if (tilesInSubset == NULL || tilesInSubset[(int) tile]) {
		++ret->cost;
	}
	ret->isNull=false;
	return true;
}

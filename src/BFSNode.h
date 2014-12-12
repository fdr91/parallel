/*
 * BFSNode.h
 *
 *  Created on: Nov 14, 2014
 *      Author: fedor
 */

#ifndef BFSNODE_H_
#define BFSNODE_H_

#include <string>
#include "Node.h"

class BFSNode: public Node {
	/*uint64_t boardConfig;
	 char direction = 'X';
	 int cost;
	 std::string path;
	 bool storePath;
	 bool isNull;*/
public:
	int serialize(char* buffer);
	int getHash();
	uint64_t boardConfig;
	char direction = 'X';
	int cost;
	std::string path;
	bool storePath;
	bool isNull;
	BFSNode();
	virtual ~BFSNode();
	BFSNode(const uint64_t boardConfig);
	BFSNode(const uint64_t boardConfig, const bool storePath);
	BFSNode(const BFSNode* node);
	int hashCode();
	std::string getPath();
	std::string getShortestPath();
	bool moveLeftNode(const bool* tilesInSubset, BFSNode* ret);
	bool moveUpNode(const bool* tilesInSubset, BFSNode* ret);
	bool moveDownNode(const bool* tilesInSubset, BFSNode* ret);
	bool moveRightNode(const bool* tilesInSubset, BFSNode* ret);
	int posOfSpaceF();
	void cp(BFSNode* copy);
};

#endif /* BFSNODE_H_ */

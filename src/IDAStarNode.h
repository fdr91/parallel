/*
 * IDAStarNode.h
 *
 *  Created on: Nov 21, 2014
 *      Author: fedor
 */

#ifndef IDASTARNODE_H_
#define IDASTARNODE_H_

#include <stdint.h>

class IDAStarNode {
public:
	static uint64_t moveUp(uint64_t, uint64_t);
	static uint64_t moveDown(uint64_t, uint64_t);
	static uint64_t moveLeft(uint64_t, uint64_t);
	static uint64_t moveRight(uint64_t, uint64_t);
	IDAStarNode();
	virtual ~IDAStarNode();
};

#endif /* IDASTARNODE_H_ */

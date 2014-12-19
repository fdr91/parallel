/*
 * Node.h
 *
 *  Created on: Nov 14, 2014
 *      Author: fedor
 */

#ifndef NODE_H_
#define NODE_H_

#include <stdint.h>
#include <string>

class Node {
	 // The tile positions array contains the 4-bit position of the tile's location
    // within a 32-bit integer.
    // Example: CostTable0 contains only the positions of tiles 2, 3, and 4.
    //   So, when mapping the configuration 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,0
    //   to an index in the table, you will get:
    //   0000..0000[4 bits for location of tile 4]
    //   [4 bits for location of tile 3][4 bits for location of tile 2]
    // The same technique is applied for the other two cost tables.
    const static int tilePositions[];
    // The subset number of a tile
    //   Tiles 2 - 4 are in subset 0
    //   Tiles 1, 5, 6, 9, 10, 13 are in subset 1
    //   The rest are in subset 2
    const static int tileSubsets[];// = {-1, 1, 0, 0, 0, 1, 1, 2, 2, 1, 1, 2, 2, 1, 2, 2};

	static void loadStreamCostTable(std::string filename, char* costTable, int size);

public:

    static void initialize();
    static void clean();

    static int h(const int64_t boardConfig);

    static int posOfSpace(const int64_t boardConfig);

};

#endif /* NODE_H_ */

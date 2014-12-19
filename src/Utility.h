/*
 * Utility.h
 *
 *  Created on: Nov 14, 2014
 *      Author: fedor
 */

#ifndef UTILITY_H_
#define UTILITY_H_

#include <vector>
#include <string>
#include <stdint.h>

/*class Utility {
public:
	Utility();
	virtual ~Utility();
*/
	std::vector<char> getTileArray(std::string* tileOrder);
	bool isValidPermutation(std::vector<char> state);
	int64_t getPositionsAsint64_t(const int64_t boardConfig,const int numOfTilesMinusOne);
	int64_t arrayToLong(std::vector<char> state);
	void displayStats(std::vector<char> initState);
	std::vector<std::string> getDirections(std::vector<char> initState);
//};

#endif /* UTILITY_H_ */

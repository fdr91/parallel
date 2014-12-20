/*
 * Path.h
 *
 *  Created on: Dec 19, 2014
 *      Author: fedor
 */

#ifndef PATH_H_
#define PATH_H_
#include <string>
#include <vector>


class Path {
	std::string p;

public:
	Path();
	Path(const Path& path);
	char get(int i);
	void set(int index, char val);
	int size();
	std::vector<std::string> getDirections(std::string initState);
	virtual ~Path();
	void getMovedTiles(std::string& pathStr, std::string initState,	std::string* movedTiles);
};

#endif /* PATH_H_ */

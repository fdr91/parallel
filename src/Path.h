/*
 * Path.h
 *
 *  Created on: Dec 19, 2014
 *      Author: fedor
 */

#ifndef PATH_H_
#define PATH_H_
#include <string>


class Path {
	std::string p;

public:
	Path();
	Path(const Path& path);
	void set(int index, char val);
	int size();
	virtual ~Path();
};

#endif /* PATH_H_ */

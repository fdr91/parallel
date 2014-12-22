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
#include "BoardState.h"


class Path {
	std::string p;
	BoardState state;
	char direction;
	bool nll;

public:
	static bool checkPath(std::string initState,std::string pstr);
	Path();
	Path(BoardState& bs);
	Path(const Path& path);
	char get(int i);
	void set(int index, char val);
	int size();
	std::vector<std::string> getDirections(std::string& initState);
	int64_t stateAsL();
	bool isSolved();
	bool isNull();
	void setNull();
	char getDirection();
	void setDirection(char c);
	bool moveLeftNode(Path* left);
	bool moveRightNode(Path* right);
	bool moveDownNode(Path* down);
	bool moveUpNode(Path* up);
	bool moveLeft();
	bool moveRight();
	bool moveDown();
	bool moveUp();
	void cp(Path* ret);
	void setState(int64_t s);
	std::string getPath();
	void setPath(std::string& p);
	BoardState getState();
	virtual ~Path();
	void append(char c);
	void getMovedTiles(std::string& pathStr, std::string initState,	std::string* movedTiles);
	Path & operator = (Path const & num);
};

#endif /* PATH_H_ */

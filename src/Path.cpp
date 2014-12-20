/*
 * Path.cpp
 *
 *  Created on: Dec 19, 2014
 *      Author: fedor
 */

#include "Path.h"
#include "BoardState.h"

using namespace std;

Path::Path() {
}

Path::Path(const Path& path){
	this->p = path.p;
}

int Path::size(){
	return p.size();
}

Path::~Path() {
	// TODO Auto-generated destructor stub
}

void Path::set(int index, char val){
	if(this->p.size()<(size_t)index+1){
		p.resize(index+1);
	}
	p.at(index)=val;
}

char Path::get(int i){
	return p.at(i);
}

std::vector<std::string> Path::getDirections(string initState){
	const int pathLength = p.length() - 1;
	//p=p.substr(1, pathLength);
	p.erase(0,1);
	vector<string> directions;
	if (pathLength != 0) {
		//char* tiles = new char[pathLength];
		//char* tiles = new char[p.length()-1];
		string movedTiles;
		movedTiles.resize(pathLength);
		getMovedTiles(p, initState, &movedTiles);
		for (int i = 0; i < pathLength; ++i) {
			const char tile = movedTiles.at(i);
			const char dir = p.at(i);
			string direction;
			if (dir == 'L') {
				direction = string("right");
			} else if (dir == 'R') {
				direction= string("left");
			} else if (dir == 'U') {
				direction = string("down");
			} else if (dir == 'D') {
				direction= string("up");
			} else {
				throw 0;
			}
			//const StringBuilder builder = new StringBuilder();
			const int iPlusOne = i + 1;
			char* buffer= new char[1024];
			sprintf(buffer, "%2d. %d - %s", iPlusOne, tile, direction.c_str());
			directions.push_back(string(buffer));
			delete [] buffer;
		}
	}
	return directions;
}

void Path::getMovedTiles(std::string& pathStr, string _initState,	string* movedTiles) {
	vector<char> initState;
    BoardState::getTileArray(&_initState, &initState);
	const int pathLength = pathStr.size(), boardLength = initState.size(),
			dimension1 = 4;
	std::string boardConfig;

	int posOfSpace = -1;
	boardConfig.resize(boardLength);
	for (int i = boardLength - 1; i >= 0; --i) {
		const char tile = initState.at(i);
		if (tile == 0)
			posOfSpace = i;
		boardConfig.at(i) = tile;
	}
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
			throw 1;
		}
		movedTiles->at(i)=boardConfig.at(posOfTile);
		char tmp = boardConfig.at(posOfSpace);
		boardConfig.at(posOfSpace) = boardConfig.at(posOfTile);
		boardConfig.at(posOfTile)=tmp;
		posOfSpace = posOfTile;
	}
	return;
}

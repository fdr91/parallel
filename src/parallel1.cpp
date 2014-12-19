//============================================================================
// Name       : parallel1.cpp
// Author      : Fedor Kusov
// Version     :
// Copyright   :
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
using namespace std;
#include <iterator>
#include <vector>

#include "../src/Utility.h"
#include "../src/PuzzleSolver.h"
#include <stdint.h>
//#include <boost/test/included/unit_test.hpp>
#include <stdexcept>
#include "gtest/gtest.h"
#include <sys/time.h>
#include <sys/resource.h>

#define THREAD_COUNT 2

int tstFunc(const char* str){
	string cppstr(str);
	vector<char> tiles = getTileArray(&cppstr);
	//PuzzleSolver ps(tiles, 1);
	PuzzleSolver ps1(tiles, 8);
	//std::vector<std::string> directions = ps.getSolution();
	std::vector<std::string> directions1 = ps1.getSolution();

	/*if(directions.size()!=directions1.size())
		return -1;

	for(int i=0; i<directions.size(); i++){
		if(directions.at(i).compare(directions1.at(i))!=0){
			return i+1;
		}
	}*/
	return 0;
}


int main(int argc, char** argv) {


	rlimit * rl = new rlimit;
	rl->rlim_max = 16777216*16;
	rl->rlim_cur = 16777216*16;
	setrlimit(RLIMIT_STACK, rl);

	return tstFunc("0,4,2,3,13,8,7,6,5,10,11,1,9,12,15,14");
}



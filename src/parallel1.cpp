//============================================================================
// Name        : parallel1.cpp
// Author      : Fedor Kusov
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
using namespace std;
#include <iterator>
#include <vector>

//#include "Utility.h"
#include "PuzzleSolver.h"
#include <stdint.h>


#include <stdexcept>

int main(int argc, char** argv) {
	try{
	if(argc!=2)
		cerr<<"Usage: parallel1 <tile order>";
	string tileOrder(argv[1]);
	vector<char> tiles = getTileArray(new string(argv[1]));

	PuzzleSolver ps(tiles);
	} catch (int e) {
		switch(e){
		case 1:
			cerr<<"File not found excetion!\n";
			break;
		default:
			cerr<<"Unknown exception";
		}
	} catch (std::logic_error & e) {
		cerr<<e.what()<<endl;
	}
	return 0;
}

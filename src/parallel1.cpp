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

#include "Utility.h"
#include "PuzzleSolver.h"
#include <stdint.h>
//#include <boost/test/included/unit_test.hpp>
#include <stdexcept>

#define THREAD_COUNT 8

int main(int argc, char** argv) {
	try {

		if (argc < 2) {
			cerr << "argc should be 2 but it is " << argc;
			cerr << "Usage: parallel1 <tile order>";
		}
		string tileOrder(argv[1]);
		vector<char> tiles = getTileArray(new string(argv[1]));
	/*	for (vector<char>::iterator iter = tiles.begin(); iter != tiles.end();
				iter++)
			cout << (int) *iter << " " << endl;*/

		PuzzleSolver ps(tiles, 1);
		PuzzleSolver ps1(tiles, 2);
		std::vector<std::string> directions = ps.getSolution();
		std::vector<std::string> directions1 = ps1.getSolution();

		for (int i = 0; i < (int)directions.size(); ++i) {
			if (!directions.at(i).compare(directions1.at(i)) == 0) {
				cerr << "wrong_answer\n" << "i=" << i;
				exit(-1);
			}
		}
		cout << "Solved";
	} catch (int e) {
		switch (e) {
		case 1:
			cerr << "File not found excetion!\n";
			break;
		default:
			cerr << "Unknown exception";
		}
	} catch (std::logic_error* e) {
		cerr << e->what() << endl;
	}
	return 0;
}

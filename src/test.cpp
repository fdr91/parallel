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

#include "StdCSolver.h"
#include <stdint.h>
//#include <boost/test/included/unit_test.hpp>
#include <stdexcept>
#include "gtest/gtest.h"

#define THREAD_COUNT 8

/*int tstFunc(const char* str){
	std::string initial("0,4,2,3,13,8,7,6,5,10,11,1,9,12,15,14");
	StdCSolver ps(initial);
	ps.solve(4);
	Path pp = ps.getPath();
	std::string pstr = pp.getPath();
	int ret = strcmp("RDLDRURDDLUURRDLLURULLDRRRULLDRRULLDLURRDDDRULLLDRRR", pstr.c_str());
	return 0;
}*/

int tstFunc1(const char* str, const char* str2, int thr){
	printf(str);
	std::string initial(str);
	StdCSolver ps(initial);
	ps.solve(thr);
	Path pp = ps.getPath();
	std::string pstr = pp.getPath();
	printf("%s\n%s\n", str2, pstr.c_str());
	int ret = strcmp(str2, pstr.c_str());
	return ret;
}


/*TEST(TestMultyThread, Test8){
	 EXPECT_EQ(0, tstFunc("0,4,2,3,13,8,7,6,5,10,11,1,9,12,15,14"));
}*/

TEST(TestMultyThread, Check4){
	EXPECT_EQ(0, tstFunc1("0,4,2,3,13,8,7,6,5,10,11,1,9,12,15,14", "RDLDRURDDLUURRDLLURULLDRRRULLDRRULLDLURRDDDRULLLDRRR", 4));
	EXPECT_EQ(0, tstFunc1("9,5,15,4,2,10,12,3,14,6,7,8,0,13,11,1", "RURUULLDRRRDLDRULDLUUURDDDLULURRDDLULURULDRRDRD", 4));
	//RURUULLDRRRDLDRULDLUUURDDDLULURRDDLULURULDRRDRD
}

TEST(TestMultyThread, Check3){
	EXPECT_EQ(0, tstFunc1("0,4,2,3,13,8,7,6,5,10,11,1,9,12,15,14", "RDLDRURDDLUURRDLLURULLDRRRULLDRRULLDLURRDDDRULLLDRRR", 3));
	EXPECT_EQ(0, tstFunc1("9,5,15,4,2,10,12,3,14,6,7,8,0,13,11,1", "RURUULLDRRRDLDRULDLUUURDDDLULURRDDLULURULDRRDRD",3));
	//RURUULLDRRRDLDRULDLUUURDDDLULURRDDLULURULDRRDRD
}


TEST(TestMultyThread, Check2){
	EXPECT_EQ(0, tstFunc1("0,4,2,3,13,8,7,6,5,10,11,1,9,12,15,14", "RDLDRURDDLUURRDLLURULLDRRRULLDRRULLDLURRDDDRULLLDRRR", 2));
	EXPECT_EQ(0, tstFunc1("9,5,15,4,2,10,12,3,14,6,7,8,0,13,11,1", "RURUULLDRRRDLDRULDLUUURDDDLULURRDDLULURULDRRDRD", 2));
	//RURUULLDRRRDLDRULDLUUURDDDLULURRDDLULURULDRRDRD
}

TEST(TestMultyThread, Check1){
	EXPECT_EQ(0, tstFunc1("0,4,2,3,13,8,7,6,5,10,11,1,9,12,15,14", "RDLDRURDDLUURRDLLURULLDRRRULLDRRULLDLURRDDDRULLLDRRR", 1));
	EXPECT_EQ(0, tstFunc1("9,5,15,4,2,10,12,3,14,6,7,8,0,13,11,1", "RURUULLDRRRDLDRULDLUUURDDDLULURRDDLULURULDRRDRD", 1));
	//RURUULLDRRRDLDRULDLUUURDDDLULURRDDLULURULDRRDRD
}

int main(int argc, char** argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}



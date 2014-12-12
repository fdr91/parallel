/*
 * IDAStar.cpp
 *
 *  Created on: Nov 14, 2014
 *      Author: fedor
 */

#include "IDAStar.h"
#include "Node.h"
#include "PuzzleSolver.h"
//#include "PuzzleConfiguration.h"
#include <iostream>

using namespace std;

IDAStar::IDAStar() {
}

clock_t IDAStar::startTime;
clock_t IDAStar::endTime;
bool IDAStar::solved;
bool IDAStar::running;
volatile uint64_t IDAStar::numberVisited;
volatile uint64_t IDAStar::numberExpanded;
string IDAStar::shortestPath;
int IDAStar::initialMovesEstimate;
int IDAStar::movesRequired;

void IDAStar::initialize() {
    startTime = clock();
    solved = false;
    running = true;
    numberVisited = 0;
    numberExpanded = 0;
}

void IDAStar::markEndTime() {
	endTime = clock();
}

IDAStar::~IDAStar() {

}

float IDAStar::getRunningTimeInSeconds() {
	clock_t diff = endTime-startTime;
	if(diff<0) return 0.0f;
	return diff/(CLOCKS_PER_SEC/1000);
}

float IDAStar::getElapsedTimeInSeconds() {
	clock_t diff = clock()-startTime;
	if(diff<0) return 0.0f;
	return diff/(CLOCKS_PER_SEC/1000);
}

void IDAStar::solve(const uint64_t currentState) {
    Node::initialize();//определяется текущая конфигурация паззла
    initialize();//инициализируются таймеры и счетчики

    solveSingleThreaded(currentState);//запуск солвера

    markEndTime();
    running = false;
}

void IDAStar::start() {
    running = true;
    solved = false;
}

void IDAStar::stop() {
	running = false;
}

void IDAStar::solveSingleThreaded(const uint64_t currentState) {
	initialMovesEstimate = movesRequired = Node::h(currentState);
    DFSWorker dfsWorker;
    do {
        if (PuzzleSolver::getVerbose()) {
            cout<<"\nSearching paths of depth " << movesRequired  << "..."<<endl;
        }
        dfsWorker.setConfig(currentState, "X", movesRequired, 0);
        dfsWorker.run();
        if (!solved) {
            movesRequired += 2;
        }
    } while (running);
}

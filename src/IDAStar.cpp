/*
 * IDAStar.cpp
 *
 *  Created on: Nov 14, 2014
 *      Author: fedor
 */

#include "IDAStar.h"
#include "PuzzleConfiguration.h"
#include <iostream>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <mqueue.h>
#include <hashtable.h>
#include <map>
#include <cstring>

#define QNAME "qqq1"

using namespace std;

IDAStar::IDAStar() {
	// TODO Auto-generated constructor stub

}

clock_t IDAStar::startTime;
clock_t IDAStar::endTime;
bool IDAStar::solved;
bool IDAStar::running;
volatile uint64_t IDAStar::numberVisited;
volatile uint64_t IDAStar::numberExpanded;
//string IDAStar::shortestPath;
int IDAStar::initialMovesEstimate;
int IDAStar::movesRequired;
char IDAStar::shortestPath[];

void IDAStar::initialize() {
	startTime = clock();
	solved = false;
	running = true;
	numberVisited = 0;
	numberExpanded = 0;
	/*string str;-*/
	memset(shortestPath, 0, sizeof(shortestPath));
	initialMovesEstimate = 0;
	movesRequired = 0;
}

void IDAStar::markEndTime() {
	endTime = clock();
}

void IDAStar::completeBFS(BFSNode node) {
	solved = true;
	string sp = node.getShortestPath();
	memset(shortestPath, 0, sizeof(shortestPath));
	for(int i=0; i<sp.size(); i++){
		shortestPath[i] = sp.at(i);
	}
	if (PuzzleConfiguration::getVerbose()) {
		cout << "done." << endl;
	}
}

IDAStar::~IDAStar() {
	// TODO Auto-generated destructor stub
}

float IDAStar::getRunningTimeInSeconds() {
	clock_t diff = endTime - startTime;
	if (diff < 0)
		return 0.0f;
	return diff / (CLOCKS_PER_SEC / 1000);
}

float IDAStar::getElapsedTimeInSeconds() {
	clock_t diff = clock() - startTime;
	if (diff < 0)
		return 0.0f;
	return diff / (CLOCKS_PER_SEC / 1000);
}

void IDAStar::solve(const uint64_t currentState, int threadCount) {
	Node::initialize(); //определяется текущая конфигурация паззла
	initialize(); //инициализируются таймеры и счетчики

	if (threadCount == 1)
		solveSingleThreaded(currentState); //запуск солвера
	else
		solveMultyThreaded(currentState, threadCount);

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
	cout << "Single threaded solving\n";
	initialMovesEstimate = movesRequired = Node::h(currentState);
	workers = new DFSWorker[1];
	DFSWorker dfsWorker;
	// Add to array so GUI can poll it for the stats in real time.
	workers[0] = dfsWorker;
	do {
		if (PuzzleConfiguration::isVerbose) {
			cout << "\nSearching paths of depth " << movesRequired << "..."
					<< endl;
		}
		dfsWorker.setConfig(currentState, "X", movesRequired, 0);
		dfsWorker.run();
		if (!solved) {
			movesRequired += 2;
		}
	} while (running);
}

void IDAStar::openQueue(mqd_t& q) {
	q = mq_open(QNAME, O_RDWR);
}

void IDAStar::solveMultyThreaded(const uint64_t currentState, int threadCount) {
	char buffer[64];
	sprintf(buffer, "Multy thread solving with thread count = %d\n",
			threadCount);
	cout << buffer;
	initialMovesEstimate = movesRequired = Node::h(currentState);
	workers = new DFSWorker[1];
	DFSWorker dfsWorker;
	// Add to array so GUI can poll it for the stats in real time.
	workers[0] = dfsWorker;
	do {
		if (PuzzleConfiguration::isVerbose) {
			cout << "\nSearching paths of depth " << movesRequired << "..."
					<< endl;
		}
		dfsWorker.setConfig(currentState, "X", movesRequired, 0);
		dfsWorker.run();
		if (!solved) {
			movesRequired += 2;
		}
	} while (running);
	//solveSingleThreaded(currentState);
}

void IDAStar::putToQueue(BFSNode* node, map<int, bool>* m) {
	int hash = node->getHash();
	map<int, bool>::iterator it = m->find(hash);
	if (it == m->end()) {
		(*m)[hash] = true;
		queue.push(node);
	}
}

void IDAStar::findStartingPositions(uint64_t currentState, int howMany) {
	BFSNode currentNode(currentState, true);
	currentNode.cost = 0;
	map<int, bool> m;

	if (currentNode.boardConfig == Node::goalState) {
		completeBFS(currentNode);
		return;
	}

	if (howMany == 1) {
		m[currentNode.getHash()] = true;
		queue.push(currentNode);
		return;
	}

	int previousMovesRequired = 0;
	while (!currentNode.isNull) {
		const char fromDirection = currentNode.direction;
		if (fromDirection != 'R') {
			BFSNode left;
			currentNode.moveLeftNode(NULL, &left);
			if (!left.isNull) {
				++numberExpanded;
				if (left.boardConfig == Node::goalState) {
					completeBFS(left);
					return;
				} else {
					putToQueue(&left, &m);
				}
			}
		}
		if (fromDirection != 'L') {
			BFSNode right;
			right = currentNode.moveRightNode(NULL, &right);
			if (!right.isNull) {
				++numberExpanded;
				if (right.boardConfig == Node::goalState) {
					completeBFS(right);
					return;
				} else {
					putToQueue(&right, &m);
				}
			}
		}
		if (fromDirection != 'D') {
			BFSNode up;
			currentNode.moveUpNode(NULL, &up);
			if (!up.isNull) {
				++numberExpanded;
				if (up.boardConfig == Node::goalState) {
					completeBFS(up);
					return;
				} else {
					putToQueue(&up, &m);
				}
			}
		}
		if (fromDirection != 'U') {
			BFSNode down;
			currentNode.moveDownNode(NULL, &down);
			if (down.isNull) {
				++numberExpanded;
				if (down.boardConfig == Node::goalState) {
					completeBFS(down);
					return;
				} else {
					putToQueue(&down, &m);
				}
			}
		}

		currentNode = queue.front();
		if (!currentNode.isNull) {
			const int movesRequired = currentNode.cost;

			if (movesRequired > previousMovesRequired) {
				previousMovesRequired = movesRequired;
				if (queue.size() >= howMany) {
					break;
				}
			} else {
				currentNode = queue.front();
				queue.pop();
			}
		}
	}
}

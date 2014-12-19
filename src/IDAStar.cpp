/*
 * IDAStar.cpp
 *
 *  Created on: Nov 14, 2014
 *      Author: fedor
 */
#define _MULTI_THREADED
#include "IDAStar.h"
#include <iostream>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <mqueue.h>
#include <hashtable.h>
#include <map>
#include <cstring>
#include <sstream>
#include <stdio.h>
#include <pthread.h>

#include "globals.h"

#define QNAME "qqq1"

using namespace std;

bool gruning=true;
bool gsolved=false;

IDAStar::IDAStar() {
	// TODO Auto-generated constructor stub

}

clock_t IDAStar::startTime;
clock_t IDAStar::endTime;
volatile int64_t IDAStar::numberVisited;
volatile int64_t IDAStar::numberExpanded;
int IDAStar::initialMovesEstimate;
int IDAStar::movesRequired;
std::vector<char> IDAStar::shortestPath;
pthread_mutex_t IDAStar::running_mutex;


void IDAStar::initialize() {
	startTime = clock();
	gsolved = false;
	gruning = true;
	numberVisited = 0;
	numberExpanded = 0;
	/*string str;-*/
	shortestPath.clear();
	initialMovesEstimate = 0;
	movesRequired = 0;
}

void IDAStar::markEndTime() {
	endTime = clock();
}

void IDAStar::completeBFS(BFSNode node) {
	gsolved = true;
	string sp = node.getShortestPath();
	shortestPath.clear();
	for (int i = 0; i < sp.size(); i++) {
		shortestPath[i] = sp.at(i);
	}
	if (IS_VERBOSE) {
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

void IDAStar::solve(const int64_t currentState, int threadCount) {
	Node::initialize(); //определяется текущая конфигурация паззла
	initialize(); //инициализируются таймеры и счетчики

	if (threadCount == 1)
		solveSingleThreaded(currentState); //запуск солвера
	else
		solveMultyThreaded(currentState, threadCount);

	markEndTime();
	Node::clean();
	gruning = false;
}

void IDAStar::solveSingleThreaded(const int64_t currentState) {
	cout << "Single threaded solving\n";
	initialMovesEstimate = movesRequired = Node::h(currentState);
	workers = new DFSWorker[1];
	DFSWorker dfsWorker;
	// Add to array so GUI can poll it for the stats in real time.
	workers[0] = dfsWorker;
	do {
		if (IS_VERBOSE) {
			cout << "\nSearching paths of depth " << movesRequired << "..."
			<< endl;
		}
		dfsWorker.setConfig(currentState, "X", movesRequired, 0);
		dfsWorker.run();
		if (!gsolved) {
			movesRequired += 2;
		}
	} while (gruning);
	delete [] workers;
}

void IDAStar::openQueue(mqd_t& q) {
	q = mq_open(QNAME, O_RDWR);
}

void * runWorker(void * arg){
	DFSWorker* dw = (DFSWorker*)arg;
	dw->run();
	pthread_exit(0);
}

bool checkRun(){
	bool ret;
	pthread_mutex_lock(&IDAStar::running_mutex);
	ret = gruning;
	pthread_mutex_unlock(&IDAStar::running_mutex);
	return ret;
}

void IDAStar::solveMultyThreaded(const int64_t currentState, int threadCount) {

	printf("Multy thread solving with thread count = %d\n", threadCount);
	findStartingPositions(currentState, threadCount);
	initialMovesEstimate = movesRequired = Node::h(currentState);

	const unsigned int numElements = queue.size();
	workers = new DFSWorker[numElements];
	pthread_t* threads = new pthread_t[numElements];
	if(!gsolved){
		while (checkRun()) {
			if (IS_VERBOSE) {
				printf("Searching paths of length %d moves\n", movesRequired);
			}
			// Add to array so GUI can poll it for the stats in real time.
			for (int i = 0; i < numElements; i++){
				DFSWorker tmp;
				workers[i] = tmp;
			}

			int p=0;
			while (p<numElements) {
				BFSNode	node = queue.front();
				queue.pop();
				queue.push(node);
				string currentPath = node.getPath();
				DFSWorker worker = workers[p];
				worker.setConfig(node.boardConfig, currentPath, movesRequired,
						currentPath.length() - 1);
				pthread_t thr;
				pthread_create(&thr, NULL, &runWorker, &worker);
				threads[p]=thr;
				p++;
			}
			for(unsigned int k=0; k<numElements; k++){
				if(int l = pthread_join(threads[k], NULL)){
					printf("Error thread join: %d", l);
				}
			}

			if (!gsolved) {
				movesRequired += 2;
			}
		}
	}
	delete [] threads;
	delete [] workers;
	threads = NULL;
	workers = NULL;
	printf("Done");
}

void IDAStar::putToQueue(BFSNode* node, map<int64_t, BFSNode>* m) {
	int hash = node->getHash();
	map<int64_t, BFSNode>::iterator it = m->find(hash);
	if (it == m->end()) {
		(*m)[hash] = node;
		queue.push(node);
	} else if(it->second.boardConfig!=node->boardConfig || it->second.path.compare(node->path)!=0 ||  it->second.direction!=node->direction){
		queue.push(node);
	}
}

void IDAStar::findStartingPositions(int64_t currentState, int howMany) {
	BFSNode currentNode(currentState, true);
	currentNode.cost = 0;
	map<int64_t, BFSNode> m;

	if (currentNode.boardConfig == GOAL_STATE) {
		completeBFS(currentNode);
		return;
	}

	if (howMany == 1) {
		m[currentNode.getHash()] = currentNode;
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
				if (left.boardConfig == GOAL_STATE) {
					completeBFS(left);
					return;
				} else {
					putToQueue(&left, &m);
				}
			}
		}
		if (fromDirection != 'L') {
			BFSNode right;
			currentNode.moveRightNode(NULL, &right);
			if (!right.isNull) {
				++numberExpanded;
				if (right.boardConfig == GOAL_STATE) {
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
				if (up.boardConfig == GOAL_STATE) {
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
			if (!down.isNull) {
				++numberExpanded;
				if (down.boardConfig == GOAL_STATE) {
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

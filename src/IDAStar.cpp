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
#include <sstream>

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
	for (int i = 0; i < sp.size(); i++) {
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

void * runWorker(void * arg){
	DFSWorker* dw = (DFSWorker*)arg;
	dw->run();
	pthread_exit(0);
}


void IDAStar::solveMultyThreaded(const uint64_t currentState, int threadCount) {
	char buffer[64];
	sprintf(buffer, "Multy thread solving with thread count = %d\n",
			threadCount);
	cout << buffer;
	findStartingPositions(currentState, threadCount);
	std::queue<BFSNode> queue_back1;
	char* etalon = "-1168372231105084636 R XRRR\n"
			"-1168372231222511836 D XRRD\n"
			"-1168372231100681596 L XRDL\n"
			"-1168372231223561596 R XRDR\n"
			"-1168372918297742716 D XRDD\n"
			"-1168372231223889331 R XDRR\n"
			"-1168372231109594611 U XDRU\n"
			"-1168372918298070451 D XDRD\n"
			"-1168372896825527731 R XDDR\n"
			"-1170905488716123571 D XDDD\n";
	ostringstream sss;
	for(int i=0; i<queue.size(); i++){
		BFSNode	node = queue.front();
		queue_back1.push(node);
		queue.pop();
		sss<<(int64_t)node.boardConfig<<" "<<node.direction<<" "<<node.path<<endl;
	}
	const char* css = sss.str().c_str();
	cout<<"css\n"<<css<<endl;
	cout<<"etalon\n"<<etalon<<endl;
	int line = 1;
	for(int i=0; i<strlen(etalon);i ++){
		if(css[i]!=etalon[i]){
			printf("Nesovpaden in line %d\n", line);
			break;
		} else if(css[i]=='\n'){
			line++;
		}
	}
	initialMovesEstimate = movesRequired = Node::h(currentState);
	workers = new DFSWorker[threadCount];
	DFSWorker dfsWorker;
	// Add to array so GUI can poll it for the stats in real time.
	for (int i = 0; i < threadCount; i++)
		workers[i] = dfsWorker;
	/*do {
		if (PuzzleConfiguration::isVerbose) {
			cout << "\nSearching paths of depth " << movesRequired << "..."
					<< endl;
		}
		dfsWorker.setConfig(currentState, "X", movesRequired, 0);
		dfsWorker.run();
		if (!solved) {
			movesRequired += 2;
		}
	} while (running);*/
	do {
		if (PuzzleConfiguration::isVerbose) {
			printf("Searching paths of length %d moves\n", movesRequired);
		}
		int i=-1;
		pthread_t threads[threadCount];
		std::queue<BFSNode> queue_back;
		while (queue.size()) {
			++i;
			printf("Start thread %d\n", i);
			BFSNode	node = queue.front();
			queue_back.push(node);
			queue.pop();
			string currentPath = node.getPath();
			DFSWorker worker = workers[i];
			worker.setConfig(node.boardConfig, currentPath, movesRequired,
					currentPath.length() - 1);
			pthread_t thr;
			pthread_create(&thr, NULL, &runWorker, &worker);
			threads[i]=thr;
		}
		queue=queue_back;
		printf("Join threads\n");
		for(int i=0; i<threadCount; i++)
			pthread_join(threads[i], NULL);

		if (!solved) {
			movesRequired += 2;
		}
	} while (running);
	printf("Done");
//	delete workers;
	//solveSingleThreaded(currentState);
}

void IDAStar::putToQueue(BFSNode* node, map<uint64_t, BFSNode>* m) {
	int hash = node->getHash();
	cout<<node->boardConfig<<" "<<node->direction<<" "<<node->path<<endl;
	map<uint64_t, BFSNode>::iterator it = m->find(hash);
	if (it == m->end()) {
		(*m)[hash] = node;
		queue.push(node);
	} else if(it->second.boardConfig!=node->boardConfig || it->second.path.compare(node->path)!=0 ||  it->second.direction!=node->direction){
		queue.push(node);
	}
}

void IDAStar::findStartingPositions(uint64_t currentState, int howMany) {
	BFSNode currentNode(currentState, true);
	currentNode.cost = 0;
	map<uint64_t, BFSNode> m;

	if (currentNode.boardConfig == Node::goalState) {
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
			currentNode.moveRightNode(NULL, &right);
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
			if (!down.isNull) {
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

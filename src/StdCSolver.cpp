/*
 * StdCSolver.cpp
 *
 *  Created on: Dec 19, 2014
 *      Author: fedor
 */

#include "StdCSolver.h"
#include <stdexcept>
#include <fstream>
#include <map>
#include "Worker.h"
#include <time.h>
#include <unistd.h>

using namespace std;

int StdCSolver::tilePositions[] = { -1, 0, 0, 1, 2, 1, 2, 0, 1, 3, 4, 2, 3, 5,
		4, 5 };
int StdCSolver::tileSubsets[] = { -1, 1, 0, 0, 0, 1, 1, 2, 2, 1, 1, 2, 2, 1,
		2, 2 };
char StdCSolver::costTable_15_puzzle_0[4096];
char StdCSolver::costTable_15_puzzle_1[16777216];
char StdCSolver::costTable_15_puzzle_2[16777216];
std::queue<Path> StdCSolver::q;
std::mutex StdCSolver::qMutex;
std::mutex StdCSolver::pMutex;
bool StdCSolver::solved;
int  StdCSolver::movesRequired;
std::mutex StdCSolver::m;
std::unique_lock<std::mutex> StdCSolver::cond_lock;
std::condition_variable StdCSolver::cond_var;
std::mutex StdCSolver::m1;
std::unique_lock<std::mutex> StdCSolver::cond_lock1;
std::condition_variable StdCSolver::cond_var1;
bool StdCSolver::notified1;
bool StdCSolver::notified;
std::list<Path> StdCSolver::list;
std::list<Path>::iterator StdCSolver::it;

Path StdCSolver::path;


bool StdCSolver::getSolved() {
	return solved;
}
int StdCSolver::h(BoardState& bs) {
	int64_t boardConfig = bs.getLong();
	int index0 = 0, index1 = 0, index2 = 0;
	for (int pos = 16 - 1; pos >= 0; --pos) {
		const int tile = (int) ((boardConfig >> (pos << 2)) & 0xF);
		if (tile != 0) {
			const int subsetNumber = tileSubsets[tile];
			switch (subsetNumber) {
				case 2:
				index2 |= pos << (tilePositions[tile] << 2);
				break;
				case 1:
				index1 |= pos << (tilePositions[tile] << 2);
				break;
				default:
				index0 |= pos << (tilePositions[tile] << 2);
				break;
			}
		}
	}
	return costTable_15_puzzle_0[index0] + costTable_15_puzzle_1[index1]
	+ costTable_15_puzzle_2[index2];
}

Path StdCSolver::getPath() {
	return this->path;
}
void StdCSolver::setPath(Path& p) {
	this->path = p;
}

StdCSolver::StdCSolver() {
	initialMovesEstimate = 0;
	movesRequired = 0;
	threadCount = -1;
	solved = false;
}

void StdCSolver::reset(const char* _p) {
	BoardState p(_p);
	initialMovesEstimate = 0;
	movesRequired = 0;
	solved = p.isGoal();
	threadCount = -1;
	puzzle = BoardState(p);
	state = BoardState(p);
}

StdCSolver::StdCSolver(const std::string &_p) {
	BoardState p(_p);
	initialMovesEstimate = 0;
	movesRequired = 0;
	solved = p.isGoal();
	threadCount = -1;
	puzzle = BoardState(p);
	state = BoardState(p);
}

void StdCSolver::solveSingleThread() {
	initialMovesEstimate = movesRequired = h(this->puzzle);
	Worker worker;
	do {
		printf("Searching path of depth %d...\n", movesRequired);
		Path p(puzzle);
		worker.setConfig(p, movesRequired);
		worker.run();
		if (!worker.isSolved()) {
			movesRequired += 2;
		} else {
			string str = worker.getSolution();
			this->path.setPath(str);
			solved = true;
		}
	}while (!solved);
}

void StdCSolver::completeBFS(Path& currentNode) {
	this->path = Path(currentNode);
	this->solved = true;
}

void StdCSolver::putToQueue(Path* path, map<int64_t, Path>* m,
		std::list<Path>& list) {
	int64_t hash = path->stateAsL();
	map<int64_t, Path>::iterator it = m->find(hash);
	if (it == m->end()) {
		(*m)[hash] = Path(*path);
		list.push_back(Path(*path));
	} else if (it->second.stateAsL() != path->stateAsL()
			|| it->second.getPath().compare(path->getPath()) != 0
			|| it->second.getDirection() != path->getDirection()) {
		list.push_back(Path(*path));
	}
}

void StdCSolver::findStartingPositions(BoardState& state, size_t tc, std::list<Path> &list) {
	Path currentNode(state);
	map<int64_t, Path> m;
	if (currentNode.isSolved()) {
		completeBFS(currentNode);
		return;
	}

	if (tc == 1) {
		list.push_back(currentNode);
		return;
	}

	int previousMovesRequired = 0;

	while (!currentNode.isNull()) {
		const char fromDirection = currentNode.getDirection();
		if (fromDirection != 'R') {
			Path left;
			currentNode.moveLeftNode(&left);
			if (!left.isNull()) {
				if (left.isSolved()) {
					completeBFS(left);
					return;
				} else {
					putToQueue(&left, &m, list);
				}
			}
		}
		if (fromDirection != 'L') {
			Path right;
			currentNode.moveRightNode(&right);
			if (!right.isNull()) {
				if (right.isSolved()) {
					completeBFS(right);
					return;
				} else {
					putToQueue(&right, &m, list);
				}
			}
		}

		if (fromDirection != 'D') {
			Path up;
			currentNode.moveUpNode(&up);
			if (!up.isNull()) {
				if (up.isSolved()) {
					completeBFS(up);
					return;
				} else {
					putToQueue(&up, &m, list);
				}
			}
		}

		if (fromDirection != 'U') {
			Path down;
			currentNode.moveDownNode(&down);
			if (!down.isNull()) {
				if (down.isSolved()) {
					completeBFS(down);
					return;
				} else {
					putToQueue(&down, &m, list);
				}
			}
		}
		currentNode = list.front();
		if (!currentNode.isNull()) {
			const int movesRequired = currentNode.size();

			if (movesRequired > previousMovesRequired) {
				previousMovesRequired = movesRequired;
				if (list.size() >= tc) {
					break;
				}
			} else {
				currentNode = list.front();
				list.erase(list.begin());
			}
		}
	}

}




void * StdCSolver::runWorker1(void * _arg) {
	PTHREAD_ARG1 arg = (THREAD_ARG1*) _arg;
	Worker worker;
	worker.setConfig(arg->node,	arg->movesRequired);

	int* ret = new int;
	*ret = worker.run() ? 1 : 0;
	if (*ret)
		arg->ret = worker.getSolution();
	return ret;
}

int StdCSolver::runWorker1(Path node, int movesRequired, string* retStr) {
	Worker worker;
	worker.setConfig(node,	movesRequired);

	int ret = worker.run() ? 1 : 0;
	if (ret)
		*retStr = worker.getSolution();
	return ret;
}
int workC=0;
bool notified1=false;
//TODO
void  StdCSolver::poolFunction() {

	string* str=new string;
	Path p;
	while (!notified) {  // loop to avoid spurious wakeups
		cond_var1.wait(cond_lock);
	}
	while(!solved) {
		//cond_var.wait(cond_lock);
		while (!notified) {  // loop to avoid spurious wakeups
		      cond_var.wait(cond_lock);
		}
		bool f=false;
		qMutex.lock();
		if(it!=list.end()) {
			p = (*(it++));
			f=true;
		}
		qMutex.unlock();
		if(f){
			workC++;
			int ret = runWorker1(p, movesRequired,  str);
			if(ret){
				pMutex.lock();
				if(!solved){
					printf("Found result: %s, length: %d\n", str->c_str(), (int)str->length());
					solved=true;
					path.setPath(*str);
				} else {
					if(path.getPath().length()>str->length()){
						path.setPath(*str);
						printf("Found result: %s, length: %d\n", str->c_str(), (int)str->length());
					}
				}
				pMutex.unlock();
			}
			workC--;
			if(workC==0)
				cond_var1.notify_all();
		} else {
			cond_var1.notify_all();
		}
	}
	return;
}



void StdCSolver::solveMultyThread(int threadCount) {



	findStartingPositions(this->puzzle, threadCount, list);



	initialMovesEstimate = movesRequired = h(this->puzzle);
	cond_lock = std::unique_lock<std::mutex>(this->m);
	cond_lock1 = std::unique_lock<std::mutex>(this->m1);
	for(int i=0; i<threadCount; i++)
		this->threadPool.push_back(std::thread(poolFunction));
	notified=false;
	cond_var1.notify_all();
	while(!solved){
		qMutex.lock();
		it=list.begin();
		qMutex.unlock();
		notified=true;
		cond_var.notify_all();
		printf("Searching paths of length %d moves\n", movesRequired);
		cond_var1.wait(cond_lock1);
		if (!solved) {
			movesRequired += 2;
		}
	}
	printf("End work\n");
	for(std::vector<std::thread>::iterator it = threadPool.begin(); it!=threadPool.end(); it++){
		if(!(*it).joinable())
			printf("The thread isn't joinable\n");
		else
			(*it).join();
	}
	printf("Exit");
}

void StdCSolver::solve(int t) {
	setInitial();

	if (t >1)
		solveMultyThread(t);
	else if (t==1){
		solveSingleThread();
	} else {
		throw 5;
	}
}

void StdCSolver::loadStreamCostTable(const string& filename, char* costTable,
		int size) {

	ifstream is(filename);
	if (is.good()) {
		is.read(costTable, size);
		is.close();
	} else {
		printf("Error when loading cost table");
		throw 1;
	}
	is.close();
}

void StdCSolver::setInitial() {

	solved = state.isGoal();
	loadStreamCostTable(string("databases/15-puzzle-663-0.db"),
			costTable_15_puzzle_0, 4096);
	loadStreamCostTable(string("databases/15-puzzle-663-1.db"),
			costTable_15_puzzle_1, 16777216);
	loadStreamCostTable(string("databases/15-puzzle-663-2.db"),
			costTable_15_puzzle_2, 16777216);

}

StdCSolver::~StdCSolver() {
	printf("Destruct PS\n");
}


/*
 * Path.cpp
 *
 *  Created on: Dec 19, 2014
 *      Author: fedor
 */

#include "Path.h"

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

// Yucheng Lin
// env.h
// Enviroment Setting

#ifndef ENV_H
#define ENV_H

#include <vector>
#include <cstdlib>
#include <iostream>

using namespace std;

class env{
	public:
		env();
		void fillVec();
		void display();
		void copyVec( vector <vector <int> >& );
		int getVecsize();

	private:
		vector < vector <int> > buildings;
		int vecSize;
		int maxHeight;

};

#endif
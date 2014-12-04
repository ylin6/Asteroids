// Yucheng Lin
// Env.h
// Environment Implementation

#include "env.h"

env::env(){
	srand( time( NULL ) );
	vecSize = 20;
	maxHeight = 5;
}

void env::copyVec(vector< vector<int> > &vecCopy){
	for (int i = 0; i < vecSize; i++){
		vector <int> temp;
		for ( int j = 0; j < vecSize; j++){
			temp.push_back( buildings[i][j]);
		}
		vecCopy.push_back(temp);
	}
}

void env::fillVec(){
	int r;
	int c;

	for ( r = 0; r < vecSize; r++){
		vector <int> col;
		for ( c = 0; c < vecSize; c++){

			
			if ( c % 2 == 0 || r % 2 == 0 ){
				col.push_back(0);
			}

			else {
				col.push_back( rand() % maxHeight );
			}

			
		}

		buildings.push_back(col);
		
	}
	cout<<"vector filled"<<endl;
}

void env::display(){
	int i;
	int j;

	for ( i = 0; i < vecSize; i++){

		for ( j = 0; j < vecSize; j++){
			cout<<buildings[i][j]<<" ";
		}

		cout<<endl;
	}
}

int env::getVecsize(){
	return vecSize;
}
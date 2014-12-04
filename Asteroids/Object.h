// Yucheng Lin
// object.h
// 10.31.14

#ifndef OBJECT_H
#define OBJECT_H

#include "Angel.h"
#include <vector>
#include <string>
#include <iostream>
#include <stdio.h> 

using namespace std;
class Object{
	public:
		Object();
		void readFile(string);
		void loadMaterial(string);
		void processData(vector <vec3>&, vector<vec3>&, vector<vec2>&);
		void loadObj(string, vector <vec3>&, vector<vec3>&, vector<vec2>&);
		void printInfo();

		vec3 getKa();
		vec3 getKd();
		vec3 getKs();
		double getShine();
		double getD();
		double getIlum();



	private:
		vector < unsigned int > vertexIndices, uvIndices, normalIndices;
		vector < vec3 > tmpVertices, tmpNormals;
		vector < vec2 > tmpUvs;

		int faceCount;
		int uvCount;
		int vCount;
		int vnCount;

		vec3 Ka;
		vec3 Kd;
		vec3 Ks;
		double sh;
		char* tex;
		double d;
		double ilum;
};

#endif
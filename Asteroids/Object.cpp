// object.cpp
// Yucheng Lin
// object implementation
#include "Object.h"

Object::Object(){
	faceCount = 0;
	vnCount = 0;
	vCount = 0;
	uvCount = 0;
}

void Object::loadMaterial(string fileName){
	FILE * file = fopen(fileName.c_str(), "r");
	if(file == NULL){
		printf("Cannot read file");
		exit(0);
	}

	int flag = 1;
	
	while(flag){
		
		char lineHeader[228];
		int res = fscanf(file,"%s", lineHeader);

		if (feof(file)){
			flag = 0;
		}

		if (strcmp(lineHeader, "Ka") == 0){
			fscanf(file, "%f %f %f", &Ka.x, &Ka.y, &Ka.z);
		}

		else if (strcmp(lineHeader, "Kd") == 0){
			fscanf(file, "%f %f %f", &Kd.x, &Kd.y, &Kd.z);
		}

		else if (strcmp(lineHeader, "Ks") == 0){
			fscanf(file, "%f %f %f", &Ks.x, &Ks.y, &Ks.z);
		}

		else if (strcmp(lineHeader,"Ns") == 0){
			fscanf(file, "%f", &sh);
		}

		else if (strcmp(lineHeader, "d") == 0){
		
			fscanf(file,"%f",&d);
		}

		else if (strcmp(lineHeader, "illum") == 0){
			fscanf(file, "%f", &ilum);
		}
		
		else if (strcmp(lineHeader,"map_Kd") == 0){
			//fscanf(file,"%s", tex);
		}
	}
	fclose(file);


}

void Object::readFile(string fileName){
	FILE * file = fopen(fileName.c_str(), "r");
	if(file == NULL){
		printf("Cannot read file");
		exit(0);
	}

	while(1){
		char lineHeader[228];
		int res = fscanf(file, "%s", lineHeader);
		if (feof(file)){
			break;
		}
	

		if ( strcmp (lineHeader, "v") == 0){
			vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			tmpVertices.push_back(vertex);
			vCount++;
		}

		else if ( strcmp (lineHeader, "vt" ) == 0 ){
			vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			tmpUvs.push_back(uv);
			uvCount++;
		}

		else if ( strcmp (lineHeader, "vn") == 0 ){
			vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			tmpNormals.push_back(normal);
			vnCount++;
		}
		
		else if ( strcmp( lineHeader, "f") == 0){
			string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches =  fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);	
			
			if( matches == 9){
				vertexIndices.push_back(vertexIndex[0]);
				vertexIndices.push_back(vertexIndex[1]);
				vertexIndices.push_back(vertexIndex[2]);
				uvIndices.push_back(uvIndex[0]);
				uvIndices.push_back(uvIndex[1]);
				uvIndices.push_back(uvIndex[2]);
				normalIndices.push_back(normalIndex[0]);
				normalIndices.push_back(normalIndex[1]);
				normalIndices.push_back(normalIndex[2]);
				faceCount++;
			}
			/*
			int matches = fscanf(file,"%d//%d %d//%d %d//%d\n", &vertexIndex[0], &normalIndex[0], &vertexIndex[1], &normalIndex[1], &vertexIndex[2], &normalIndex[2] );
			cout<<matches<<endl;
			/*else {
				
				vertexIndices.push_back(vertexIndex[0]);
				vertexIndices.push_back(vertexIndex[1]);
				vertexIndices.push_back(vertexIndex[2]);
				normalIndices.push_back(normalIndex[0]);
				normalIndices.push_back(normalIndex[1]);
				normalIndices.push_back(normalIndex[2]);
				faceCount++;
			}*/
		}

		else if ( strcmp (lineHeader, "mtllib") == 0){
			char fileName[80];
			fscanf(file,"%s",fileName );
			loadMaterial(fileName);
		}

		else if ( strncmp ("usemtl", lineHeader, 7) == 0){

		}
		
	}
	fclose(file);
}

void Object::processData(vector < vec3 > &outVertices, vector< vec3 > &outNormals, vector< vec2 > &outUvs){
	for (unsigned int i = 0; i < vertexIndices.size(); i++){
		unsigned int vertexIndex = vertexIndices[i];
		vec3 vertex = tmpVertices[vertexIndex - 1];
		outVertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < normalIndices.size(); i++){
		unsigned int normalIndex = normalIndices[i];
		vec3 vertex = tmpNormals[normalIndex - 1];
		outNormals.push_back(vertex);
	}

	for (unsigned int i = 0; i < uvIndices.size(); i++){
		unsigned int uvIndex = uvIndices[i];
		vec2 vertex = tmpUvs[uvIndex - 1];
		outUvs.push_back(vertex);
	}
}


vec3 Object::getKd(){
	return Kd;
}

vec3 Object::getKa(){
	return Ka;
}

vec3 Object::getKs(){
	return Ks;
}

double Object::getShine(){
	return sh;
}
double Object::getD(){
	return d;
}
double getIlum(){
	//return ilum;
}

void Object::printInfo(){
	cout<<"Vertices : "<<vCount<<endl;
	cout<<"Normals : "<<vnCount<<endl;
	cout<<"Texture Vertices : "<<uvCount<<endl;
	cout<<"Faces : "<<faceCount<<endl<<endl;
}

void Object::loadObj(string fileName, vector < vec3 > &outVertices, vector< vec3 > &outNormals, vector< vec2 > &outUvs){
	readFile(fileName);
	processData(outVertices, outNormals, outUvs);
	cout<<"File: "<<fileName<<endl;
	printInfo();
}
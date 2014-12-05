// particle.cpp
// Yucheng Lin
// 11.20.14

#include "particle.h"
#include <cstdlib>


particle::particle(){
	velocity = 3;
	position = vec3(rand()%250, rand()%500, rand()%300+150);
	alive = 0;
	lifespan = 6.0;
	direction = vec3( (2.0 * (float)rand () /  (float)RAND_MAX )- 1.0, (2.0 * (float)rand () /  (float)RAND_MAX )- 1.0, (2.0 * (float)rand () /  (float)RAND_MAX )- 1.0 );
}
particle::particle(float x, float y, float z, float v){
	velocity = v;
	position = vec3(x,y,z);
	alive = 0;
	lifespan = 1.0;
	direction = vec3( (2.0 * (float)rand () /  (float)RAND_MAX )- 1.0, (2.0 * (float)rand () /  (float)RAND_MAX )- 1.0, (2.0 * (float)rand () /  (float)RAND_MAX )- 1.0 );
}

void particle::setPos(vec3 pos){
	position = pos;
}

vec3 particle::getPos(){
	return position;
}
void particle::setV(float v){
	velocity = v;
}

float particle::getAlive(){
	return alive;
}

float particle::getLifespan(){
	return lifespan;
}

void particle::setAlive(float a){
	alive = a;
}

float particle::getV(){
	return velocity;
}

vec3 particle::getDir(){
	return direction;
}
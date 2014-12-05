// particle.h
// particle class

#ifndef PARTICLE_H 
#define PARTICLE_H

#include "Angel.h"
class particle{
	public:
		particle();
		particle(float, float, float, float);
		vec3 getPos();
		void setPos(vec3);
		void setV(float);
		float getV();
		float getAlive();
		void setAlive(float);
		float getLifespan();
		vec3 getDir();

	private:
		float alive;
		float lifespan;
		vec3 position;
		float velocity;
		vec3 direction;
};

#endif
// Game.h
// Yucheng Lin

#ifndef GAME_H
#define GAME_H

#include "Angel.h"
#include "Object.h"
#include <iostream>
#include <stack>
#include "particle.h"
#include <cstdlib>
#include <cmath>

#include "fmodAPI/inc/fmod.hpp"
#include "fmodAPI/inc/fmod.h"

#define STARCOUNT 700
#define JET_PARTS 5000
#define MAX_PARTICLES 100
#define GRAVITY 1
#define STEP 0.1

class Game;
static Game* instance;

class Game{
	public:
		Game();
		void play();

	private:
		// Helper Functions
		void setupDisplayCallback();
		void setupReshapeCallback();
		void setupKeyPressCallback();
		void setupKeyReleaseCallback();
		void setupIdleCallback();
		void setupMouseCallback();

		void createSphere(int);
		void drawSphere(double, float, float, float, mat4);
		void initReset();
		void drawMenu(string);
		void soundCheck();
		void cleanUp();
		
		GLuint loadBMP(string);
		float distance(float, float, float, float, float, float);
		void createParticles();
		void createExplosion();
		void createExplosion2(float, float, float);
		void createStars();
		void createJet();
		void addParticleJ(int);
		void addParticle(int, float);
		void advanceExplosion();
		void advanceExplosion2();
		void advanceStars();
		void advanceParticles();
		void advanceParticleJ();
		void advanceBullets();
		void drawExplosion(mat4);
		void drawExplosion2(mat4);
		void drawExplosion3(mat4);
		void drawParticles(mat4);
		void drawStars(mat4);
		void drawParticlesJ(mat4);
		void drawBullets(mat4);
		void quad(int, int, int, int);
		void makeCube();
		void init();
		void printw(float, float, float, char*, ...);
		void display();
		void idle();
		void keyboard(int, int, int);
		void keyRelease(int,int, int);
		void mouse(int, int, int, int);
		void reshape(int, int);

		static void displayCallback(){
			instance->display();
		}

		static void reshapeCallback( int w, int h ){
			instance->reshape(w,h);
		}

		static void KeyPressCallback( int key, int x, int y ){
			instance->keyboard(key, x, y);
		}

		static void KeyReleaseCallback( int key, int x, int y ){
			instance->keyRelease(key, x, y);
		}

		static void idleCallback(){
			instance->idle();
		}

		static void mouseCallback(int button, int state, int x, int y){
			instance->mouse(button, state, x, y);
		}


		// Variables

		GLuint buffer;
		GLuint buffer2;
		GLuint buffer3;
		GLuint buffer4;

		GLuint texture;
		GLuint loc;
		GLuint vNormal;
		GLuint vTexCoord;

		GLuint texLoc;

		GLfloat flame;
		GLfloat sky;
		GLvoid *font_style = GLUT_BITMAP_TIMES_ROMAN_24;
		// Environment Variables
		Object* Ship;
		Object* Roid;

		// PARTICLE SYSTEM VARIABLES
		particle explosion[JET_PARTS];
		particle explosion2[STARCOUNT];
		particle rain[MAX_PARTICLES];
		particle jet[JET_PARTS];
		particle stars[JET_PARTS];
		particle bullets[2];
		GLint flag;
		// Matrix Stack
		std::stack<mat4> mv_stack;

		// Window Variables
		int winW;
		int winH;
		int texFlag; 
		// Camera and Projection Variables
		float speed;
		float directionZ;
		float directionY;
		float directionR;
		GLfloat radius;
		GLfloat theta;
		GLfloat phi;
		GLuint program;

		GLuint model_view;

		GLfloat fovy;
		GLfloat aspect;

		GLfloat zNear; 
		GLfloat zFar;

		GLuint projection;

		float vX;
		float vY;
		float vZ;
		int rX;

		// Cube Variables
		int Index;

		vec3 normals[42];
		vec3 points[42];
		vec2 tex_coords[42];

		vector <vec3> sphereV;
		vector <vec3> vertices;
		vector <vec3> norm;
		vector <vec2> uvs;

		vector <vec3> points2;
		vector <vec3> normals2;
		vector <vec2> tex_coords2;

		// 	FMOD variables
		FMOD::System *fmodSystem;
		FMOD::Channel *themeChannel;
		FMOD::Channel *jetChannel;
		FMOD::Channel *channel;
		FMOD_RESULT result;

		FMOD::Sound *beep;
		FMOD::Sound *explosionSound1;
		FMOD::Sound *explosionSound2;
		FMOD::Sound *slotFill;
		FMOD::Sound *jetSounds;
		FMOD::Sound *menu;
		FMOD::Sound *gameSound;
		FMOD::Sound *lzr;

		// Game Variables
		int bFlag; // flag whether or not to draw the bullet
		float etime;
		int speedUp;
		int death;
		float blast;
		float angleX;
		float angleY;
		float angleZ;
		int camLock;
		int state;	
		int drawEx2; // Flag whether or not to draw asteroid explosion
		int ammo;
		float eX;
		float eY;
		float eZ;
		int sign;
		float bg;
		bool debug;
};

#endif 
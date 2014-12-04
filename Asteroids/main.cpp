// main.cpp
// Yucheng Lin
// 11.02.14

#include "Angel.h"
#include "Object.h"
#include <iostream>
#include <stack>
#include "particle.h"
#include <cstdlib>
#include "SOIL/include/SOIL/SOIL.h"
#include <cmath>


#define STARCOUNT 700
#define JET_PARTS 5000
#define MAX_PARTICLES 100
#define GRAVITY 1
#define STEP 0.1

// GLOBAL VARIABLES _________________________________________________
GLuint buffer;
GLuint buffer2;
GLuint buffer3;

GLuint texture;
GLuint loc;
GLuint vNormal;
GLuint vTexCoord;

GLuint texLoc;
GLuint texLoc2;

GLuint loc2;
GLuint vNormal2;
GLuint vTexCoord2;
GLfloat flame;
GLvoid *font_style = GLUT_BITMAP_TIMES_ROMAN_24;
// Environment Variables
Object* Ship = new Object();
Object* Roid = new Object();

// PARTICLE SYSTEM VARIABLES
particle explosion[JET_PARTS];
particle rain[MAX_PARTICLES];
particle jet[JET_PARTS];
particle stars[JET_PARTS];
GLint flag;
// Matrix Stack
std::stack<mat4> mv_stack;

// Window Variables
int winW = 700;
int winH = 700;
int texFlag; 
// Camera and Projection Variables
float speed = 1.0;
float directionZ = 0;
float directionY = 0;
float directionR = 0;
GLfloat radius = 100;
GLfloat theta = 0;
GLfloat phi = 0;
GLuint program;
const GLfloat dr = 5.0 * DegreesToRadians;

GLuint model_view;

GLfloat fovy = 30.0;
GLfloat aspect;

GLfloat zNear = 1.0, zFar = 1000;

GLuint projection;

float vX = 450;
float vY = 80;
float vZ = 300;
int rX = 0;
// Cube Variables
int Index = 0;

vec3 normals[36];
vec3 points[36];
vec2 tex_coords[36];

vector <vec3> vertices;
vector <vec3> norm;
vector <vec2> uvs;

vector <vec3> points2;
vector <vec3> normals2;
vector <vec2> tex_coords2;

// Game Variables
float etime = 0;
int speedUp = 0;
int death = 0;
// FUNCTIONS ______________________________________________________

float distance(float x1, float y1, float z1, float x2, float y2, float z2){
	return ( sqrt( pow((x1-x2),2) + pow( (y1-y2),2) + pow((z1-z2),2) ) );
}

void createParticles(){
	for ( int i = 0; i < MAX_PARTICLES; i++){
		particle newParticle;
		rain[i] = newParticle;
	}
}

void createExplosion(){
	for ( int i = 0; i < JET_PARTS; i++){
		particle newParticle(vX, vY, vZ, 2);
		explosion[i] = newParticle;
	}
}

void advanceExplosion(){
	int i;
	for ( i = 0; i < JET_PARTS; i++){
		explosion[i].setPos(explosion[i].getPos()+vec3(rand() % 3 + stars[i].getV(), rand() % 3 + stars[i].getV(),rand() % 3 + stars[i].getV()));
		stars[i].setV(stars[i].getV() * GRAVITY );	
		
		if( distance(explosion[i].getPos().x, explosion[i].getPos().y, explosion[i].getPos().z, vX, vY, vZ) > 400){
			exit(0);
		}
	}
}

void drawExplosion(mat4 mav){
	for (int i = 0; i < JET_PARTS; i++ ){
		mv_stack.push(mav);
			mat4 instance = Translate(explosion[i].getPos().x, explosion[i].getPos().y, explosion[i].getPos().z ) *  Scale(0.5, 0.5 ,0.5);
			glUniformMatrix4fv(model_view, 1, GL_TRUE, mav*instance);
			glDrawArrays(GL_TRIANGLES, 0, 3);
			mav = mv_stack.top();
		mv_stack.pop();
	}
}
void createStars(){
		for ( int i = 0; i < STARCOUNT; i++){
		particle newParticle(rand()%250, rand()%500, rand()%300+150, 1);
		stars[i] = newParticle;
	}
}

void advanceStars(){
	int i;
	for ( i = 0; i < STARCOUNT; i++){
		stars[i].setPos(stars[i].getPos()+vec3(stars[i].getV(), 0,0));

		if (stars[i].getPos().x > 550){
			particle newParticle(rand()%250, rand()%500, rand()%300+150, stars[i].getV());
			stars[i] = newParticle;
		
		}

		if( etime < 3 && stars[i].getV() < 4 ){
			stars[i].setV(stars[i].getV() + GRAVITY );
		}


	}
}

void createJet(){
	for (int i = 0; i < JET_PARTS; i++){
		particle newParticle(vX - (rand() % 4) / 5.0 + 4, vY + (rand() % 5) / 10.0 -2 , vZ + (rand() % 5) / 10.0 + 4, rand()%4 + 1);
		jet[i] = newParticle;
	}
}

void addParticleJ(int i){
	particle newParticle(vX  - (rand() % 4)/5.0 + 4, vY + (rand() % 5) / 10.0 -2 , vZ + (rand() % 5) / 10.0 + 4, (rand()%8) + 1);
	jet[i] = newParticle;
}

void addParticle(int i, float v){
	particle newParticle(rand()%250, rand()%500, rand()%300+150, (rand() % 3) + v);
	rain[i] = newParticle;
}


void advanceParticleJ(){
	int i;
	for ( i = 0; i < JET_PARTS; i++){
		jet[i].setPos(jet[i].getPos()+vec3(jet[i].getV(), 0,0));
		//rain[i].setV(rain[i].getV() + GRAVITY * rain[i].getAlive() 
		if (jet[i].getPos().x > 470){
			
			if(i % 2 == 0)addParticleJ(i);
			else{
				particle newParticle(vX - (rand() % 4)/5.0 + 4, vY + (rand() % 5) / 10.0 -2 , vZ + (rand() % 5) / 10.0 - 5.6, (rand()%8) + 1);
				jet[i] = newParticle;
			}
		
		}

	}
}

GLuint loadTexture(string texname) {
	GLuint tex = SOIL_load_OGL_texture(texname.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);   
	glBindTexture(GL_TEXTURE_2D, tex);
   	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
   	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
   	return tex;
}

void advanceParticles(){
	int i;
	for ( i = 0; i < MAX_PARTICLES; i++){
		rain[i].setPos(rain[i].getPos()+vec3(rain[i].getV(), 0,0));
		//rain[i].setV(rain[i].getV() + GRAVITY * rain[i].getAlive() );
		rain[i].setAlive(rain[i].getAlive() + STEP);

		if (rain[i].getPos().x > 550){
			addParticle(i, rain[i].getV() + etime *.002);
		
		}

		//cout<<distance(rain[i].getPos().x, rain[i].getPos().y, rain[i].getPos().z, vX, vY, vZ )<<endl;
		if( distance(rain[i].getPos().x, rain[i].getPos().y, rain[i].getPos().z, vX, vY, vZ ) <= 15.0){
			cout<<"HIT"<<endl;
			death = 1;
			//exit(0);
		}


	}
}

void quad( int a, int b, int c, int d ){
    vec3 vertices[8] = {
	vec3( -0.5, -0.5,  0.5), //0
	vec3( -0.5,  0.5,  0.5), //1
	vec3(  0.5,  0.5,  0.5), //2
	vec3(  0.5, -0.5,  0.5), //3
	vec3( -0.5, -0.5, -0.5), //4
	vec3( -0.5,  0.5, -0.5), //5
	vec3(  0.5,  0.5, -0.5), //6
	vec3(  0.5, -0.5, -0.5)  //7
    };

    vec4 u = vertices[b] - vertices[a];
    vec4 v = vertices[c] - vertices[b];

    vec3 n = normalize( cross (u,v));
  	srand (time(NULL));
  	texFlag = rand() % 2;
  	float Tx;
  	if (texFlag == 0) Tx = 5.0;
  	else Tx = 3.0;
  	normals[Index] = n;
    points[Index] = vertices[a];
    tex_coords[Index] = vec2( 0.0, 0.0 );
    Index++;
    
    normals[Index] = n;
    points[Index] = vertices[b];
    tex_coords[Index] = vec2( 0.0, Tx );
    Index++;

    normals[Index] = n;
    points[Index] = vertices[c];
    tex_coords[Index] = vec2( Tx, Tx );
    Index++;
    
    normals[Index] = n;
    points[Index] = vertices[a];
    tex_coords[Index] = vec2( 0.0, 0.0 );
    Index++;
    
    normals[Index] = n;
    points[Index] = vertices[c];
    tex_coords[Index] = vec2( Tx, Tx);
    Index++;
    
    normals[Index] = n;
    points[Index] = vertices[d];
    tex_coords[Index] = vec2( Tx, 0.0 );
    Index++;
}

void makeCube(){
	quad( 1, 0, 3, 2 );
    quad( 2, 3, 7, 6 );
    quad( 3, 0, 4, 7 );
    quad( 6, 5, 1, 2 );
    quad( 4, 5, 6, 7 );
    quad( 5, 4, 0, 1 );
}



void drawParticles(mat4 mav){
	for (int i = 0; i < MAX_PARTICLES; i++ ){
		mv_stack.push(mav);
			mat4 instance = Translate(rain[i].getPos().x, rain[i].getPos().y, rain[i].getPos().z ) *  Scale(10,10,10);
			glUniformMatrix4fv(model_view, 1, GL_TRUE, mav*instance);
			glDrawArrays(GL_TRIANGLES, 0, points2.size());
			mav = mv_stack.top();
		mv_stack.pop();
	}	
}

void drawStars(mat4 mav){
	for (int i = 0; i < STARCOUNT; i++ ){
		mv_stack.push(mav);
			mat4 instance = Translate(stars[i].getPos().x, stars[i].getPos().y, stars[i].getPos().z ) *  Scale(0.25,0.25,0.25);
			glUniformMatrix4fv(model_view, 1, GL_TRUE, mav*instance);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			mav = mv_stack.top();
		mv_stack.pop();
	}	
}

void drawParticlesJ(mat4 mav){
	for (int i = 0; i < JET_PARTS; i++ ){
		glUniform1f(flame, (jet[i].getPos().x-460) / 10);
		mv_stack.push(mav);
			mat4 instance = Translate(jet[i].getPos().x, jet[i].getPos().y, jet[i].getPos().z );
			glUniformMatrix4fv(model_view, 1, GL_TRUE, mav*instance);
			glDrawArrays(GL_POINTS, 0, 1);
			mav = mv_stack.top();
		mv_stack.pop();
	}	
}

GLuint loadBMP(string fileName){
	unsigned char header[45];
	unsigned int dataPos;
	unsigned int width, height;
	unsigned int imageSize;
	unsigned char* data;

	FILE * file = fopen(fileName.c_str(),"rb");
	if (!file){
		cout<<"Could not open BMP"<<endl;
		exit(0);
	}

	if (fread(header,1,54, file ) != 54){
		cout<<"Could Not Read File"<<endl;
		exit(0);
	}

	if ( header[0] != 'B' || header[1] != 'M'){
		cout<<"Not BMP file"<<endl;
		exit(0);
	}

	dataPos    = *(int*)&(header[0x0A]);
	imageSize  = *(int*)&(header[0x22]);
	width      = *(int*)&(header[0x12]);
	height     = *(int*)&(header[0x16]);

	if (imageSize == 0){
		imageSize = width & height * 3;
	}

	if (dataPos == 0){
		dataPos = 54;
	} 

	data = new unsigned char [imageSize];

	fread(data,1,imageSize,file);
	fclose(file);

	GLuint textureID;
	glGenTextures(1, &textureID);

	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width , height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	return textureID;
}

void init(){
	makeCube();
	createExplosion();
	createParticles(); 
	createJet();  
	createStars();
	Ship->loadObj("ship.obj", vertices, norm, uvs);
	Roid->loadObj("asteroid.obj", points2, normals2, tex_coords2);
	GLuint vao;
	glGenVertexArraysAPPLE(1, &vao);
	glBindVertexArrayAPPLE( vao );


	
	glGenBuffers(1,&buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec3) + norm.size() * sizeof(vec3) + uvs.size() * sizeof(vec2), &vertices[0], GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), &vertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(uvs), &uvs);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(uvs), sizeof(norm) ,&norm);
	
	program = InitShader("vShader.glsl", "fShader.glsl");
	glUseProgram( program );

	flag = glGetUniformLocation(program, "flag");
	flame = glGetUniformLocation(program, "flame");

	loc = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer( loc, 3, GL_FLOAT, GL_FALSE, 0, 0 );

	vTexCoord = glGetAttribLocation(program, "vTexCoord");
	glEnableVertexAttribArray(vTexCoord);
	glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET( sizeof(vertices) ));

	vNormal = glGetAttribLocation( program, "vNormal");
	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET( sizeof(vertices) + sizeof(uvs) ));
	vec4 light_position( 0 , 0.0, 0, 1.0 );
    vec4 light_ambient( 0.2, 0.2, 0.2, 1.0 );
    vec4 light_diffuse( 1.0, 1.0, 1.0, 1.0 );
    vec4 light_specular( 1.0, 1.0, 1.0, 1.0 );

    vec4 material_ambient( .2, .2, .2, 1.0 );
    vec4 material_diffuse( .3, .3, .3, 1.0 );
    vec4 material_specular( .4, 0.4, 0.4, 1.0 );
    float  material_shininess = 10.0;

    vec4 ambient_product = light_ambient * material_ambient;
    vec4 diffuse_product = light_diffuse * material_diffuse;
    vec4 specular_product = light_specular * material_specular;

    glUniform4fv( glGetUniformLocation(program, "AmbientProduct"),
		  1, ambient_product );
    glUniform4fv( glGetUniformLocation(program, "DiffuseProduct"),
		  1, diffuse_product );
    glUniform4fv( glGetUniformLocation(program, "SpecularProduct"),
		  1, specular_product );
	
    glUniform4fv( glGetUniformLocation(program, "LightPosition"),
		  1, light_position );

    glUniform1f( glGetUniformLocation(program, "Shininess"),
		 material_shininess );

	model_view = glGetUniformLocation( program, "model_view" );
    projection = glGetUniformLocation( program, "projection" );
    texLoc = glGetUniformLocation(program, "texMap");
    texLoc2 = glGetUniformLocation(program, "texMap2");
 
 	
    GLuint vao2;
	glGenVertexArraysAPPLE(1, &vao2);
	glBindVertexArrayAPPLE( vao2 );

    glGenBuffers(1, &buffer2);
    glBindBuffer(GL_ARRAY_BUFFER, buffer2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normals) + sizeof(points) + sizeof(tex_coords), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(normals), normals);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(normals),sizeof(tex_coords), tex_coords);


    GLuint vao3;
    glGenVertexArraysAPPLE(1, &vao3);
    glBindVertexArrayAPPLE(vao3);

    glGenBuffers(1, &buffer3);
    glBindBuffer(GL_ARRAY_BUFFER, buffer3);
    glBufferData(GL_ARRAY_BUFFER, normals2.size() * sizeof(vec3) + points2.size() * sizeof(vec3) + tex_coords2.size() * sizeof(vec2), &points2[0], GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points2), &points2);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(points2), sizeof(normals2), &normals2);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(points2) + sizeof(normals2),sizeof(tex_coords2), &tex_coords2);
	
	//glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0,0.0,0.0,1.0);

} 

void printw (float x, float y, float z, char* format, ...){
    va_list arg_list;
    char str[256];
	int i;
    
    va_start(arg_list, format);
    vsprintf(str, format, arg_list);
    va_end(arg_list);
    
    glRasterPos3f (x, y, z);

    for (i = 0; str[i] != '\0'; i++){
        glutBitmapCharacter(font_style, str[i]);
    }
}

void display(){
	etime = .001 * glutGet(GLUT_ELAPSED_TIME);
	
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	vec4 eye;
	eye = vec4(radius * cos(theta) + vX, vY + 10, radius * sin(theta) + vZ, 1.0);

	vec4 at(vX, vY, vZ, 0);
	vec4 up(0.0, 1.0, 0.0, 0.0);

	glUniform1i(flag, 7);
	printw(0, 1.0, 100.0,"%d", (int)(etime * 10));

	mat4 mv = LookAt(eye, at, up);
	glUniformMatrix4fv(model_view, 1, GL_TRUE, mv);

	mat4 p = Perspective(fovy,-aspect, zNear, zFar);
	glUniformMatrix4fv(projection, 1, GL_TRUE, p);

	//glDisable(GL_CULL_FACE);
	//Bind Buffer for Car
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer( loc, 3, GL_FLOAT, GL_FALSE, 0, 0 );

	glEnableVertexAttribArray(vTexCoord);
	glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET( sizeof(vertices) ));

	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET( sizeof(vertices) + sizeof(uvs) ));
	//texture = loadBMP("ship.bmp");
	glUniform1i(flag, 0);
	mv_stack.push(mv);
		vZ = vZ + directionZ * speed;
		vY = vY + directionY * speed;

		if(vZ > 450) vZ = 450;
		if(vZ < 150) vZ = 150;
		if(vY > 500) vY = 500;
		if(vY < 0) vY = 0;
		if ( rX < 10 && rX > -10 ){
			rX = rX + directionR * speed;
		}

		if( directionR == 0 && rX != 0){
			if ( rX > 0){
				rX--;	
			}

			else{
				rX++;
			}
			
		}

		mat4 instance = Translate(vX, vY, vZ) * RotateX(rX);
		glUniformMatrix4fv(model_view, 1, GL_TRUE, mv*instance);
		if( death == 0){
    		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    	}
    	mv = mv_stack.top();
    mv_stack.pop();
	
    //glEnable(GL_CULL_FACE);
    
    
   	//Bind Buffer for enviroment
	glBindBuffer(GL_ARRAY_BUFFER, buffer2);
	
    glEnableVertexAttribArray(loc);
	glVertexAttribPointer( loc, 3, GL_FLOAT, GL_FALSE, 0, 0 );

	
	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET( sizeof(points) ) );


	glEnableVertexAttribArray(vTexCoord);
	glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET( sizeof(points) + sizeof(normals) ) );
	
	glUniform1i(flag, 0);
	//texture = loadBMP("building.bmp");
	//glActiveTexture(GL_TEXTURE0);
	//glUniform1i(texLoc, 0);
	//glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    //glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

    //drawCity(mv);
    if(death == 1){
    	advanceExplosion();
    	drawExplosion(mv);
    }
    if ( death == 0) advanceParticleJ();
    glUniform1i(flag, 1);
    drawParticlesJ(mv);
   	mv_stack.push(mv);
		mat4 instance2 = Translate(450, 80, 300) * Scale(1000, 1000, 1000);
		glUniformMatrix4fv(model_view, 1, GL_TRUE, mv*instance2);
		//texture = loadBMP("road1.bmp");
		//glActiveTexture(GL_TEXTURE0);
		//glUniform1i(texLoc, 0);
		glUniform1i(flag, 2);
    	glDrawArrays(GL_TRIANGLES, 12, 6);
    	glUniform1i(flag, 5);
    	if (texFlag == 0 ) texture = loadBMP("sky.bmp");
    	else texture = loadBMP("sky2.bmp");
    	//texture = loadTexture("sky2.BMP");
		//glActiveTexture(GL_TEXTURE0);
		//glUniform1i(texLoc, 0);
		//glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
    	//glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
    	glDrawArrays(GL_TRIANGLES, 0,6 );
    	glDrawArrays(GL_TRIANGLES, 12, 24);

    	mv = mv_stack.top();
    mv_stack.pop();
  
  	glUniform1i(flag, 6);
  	advanceStars();  	
  	drawStars(mv);

    glUniform1i(flag, 3);
	glBindBuffer(GL_ARRAY_BUFFER, buffer3);
	
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer( loc, 3, GL_FLOAT, GL_FALSE, 0, 0 );

	
	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET( sizeof(points2) ) );


	glEnableVertexAttribArray(vTexCoord);
	glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET( sizeof(points2) + sizeof(normals2) ) );
	
	if ( etime > 3){
    advanceParticles();
    glUniform1i(flag, 4);
    drawParticles(mv);
    }


	glutSwapBuffers();
}

void idle(){
	glutPostRedisplay();
}

void keyboard( int key, int x, int y ){
    switch( key ) {
	case 033: // Escape Key
	case 'q': case 'Q':
	    exit( EXIT_SUCCESS );
	    break;
	case GLUT_KEY_UP:
		directionY = 1;
		break;
	case GLUT_KEY_DOWN:
		directionY = -1;
		break;
	case GLUT_KEY_LEFT:
		directionR = -1;
		directionZ = -1;
		break;
	case GLUT_KEY_RIGHT:
		directionR = 1;
		directionZ = 1;
		break;
	case 'j':
		directionR = -1;
		break;
	case 'l':
		directionR = 1;;
		break;
	case 'i':
		//vX++;
		radius--;
		break;
	case 'o':
		//vX--;
		radius++;
		break;

    }
    cout<<"vZ : "<<vZ<<endl;
    cout<<"vX : "<<vX<<endl;
    cout<<"vY : "<<vY<<endl;
    //glutPostRedisplay();
}

void keyRelease(int key, int x, int y){
	switch(key){
		case GLUT_KEY_UP:
		case GLUT_KEY_DOWN:
			directionY = 0;
			break;
		case GLUT_KEY_RIGHT:
		case GLUT_KEY_LEFT:
			directionR = 0;
			directionZ = 0;
			break;
		case 'j':
		case 'l':
			directionR = 0;
			break;
		default:
			break;
	}
}


void mouse(int button, int state, int x, int y){
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		theta +=dr;
	}

	else if ( button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
		theta -=dr;
	}

	//glutPostRedisplay();
}

void reshape(int width, int height){
	glViewport (0,0, width, height);
	aspect = GLfloat(width)/height;
}

// GLUT MAIN LOOP ____________________________________________________________
int main( int argc, char**argv){

	glutInit( &argc, argv);
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
	glutInitWindowSize(winH, winW);
	glutCreateWindow("Asteroids");

	init();
	glutIdleFunc(idle);
	glutDisplayFunc( display );
	glutReshapeFunc( reshape );
	glutSpecialFunc( keyboard );
	glutSpecialUpFunc( keyRelease);
	glutMouseFunc(mouse);

	glutMainLoop();

	return 0;

}
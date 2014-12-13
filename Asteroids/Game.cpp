// main.cpp
// Yucheng Lin
// 11.02.14

#include "Game.h"


// Constructor
Game::Game(){
	Ship = new Object();
	Roid = new Object();

	winH = 700;
	winW = 700;
	speed = 1.0;
	radius = 100;
	fovy = 30.0;
	zNear = 1.0;
	zFar = 1000;
	vX = 450;
	vY = 80;
	vZ = 300;
	state = 0;
	death = 0;
	drawEx2 = 0;
	etime = 0;
	bFlag = 0;
	bg = 0;
	sign = 1;
	debug = false;

	result = FMOD::System_Create( &fmodSystem );
	result = fmodSystem->init( 32, FMOD_INIT_NORMAL, NULL );

	result = fmodSystem->createSound( "music/explosion-01.mp3", FMOD_SOFTWARE, 0, &explosionSound1 );
	result = fmodSystem->createSound( "music/explosion-02.mp3", FMOD_SOFTWARE, 0, &explosionSound2 );
	result = fmodSystem->createSound( "music/slotFill.mp3", FMOD_SOFTWARE, 0, &slotFill );
	result = fmodSystem->createSound( "music/lzr.mp3", FMOD_SOFTWARE, 0, &lzr);
	result = fmodSystem->createStream( "music/menu.mp3",FMOD_SOFTWARE | FMOD_LOOP_NORMAL, 0, &menu);
	result = fmodSystem->createStream( "music/jet.mp3",FMOD_SOFTWARE | FMOD_LOOP_NORMAL, 0, &jetSounds);

	result = fmodSystem->playSound(FMOD_CHANNEL_FREE, menu, false, &themeChannel);

}

// FUNCTIONS _____________________________________________________________________________________________________________
void Game::setupDisplayCallback(){
	instance = this;
   	::glutDisplayFunc(Game::displayCallback);
	::glutIdleFunc(Game::displayCallback);
}

void Game::setupReshapeCallback(){
  	instance = this;
  	::glutReshapeFunc(Game::reshapeCallback);
}

void Game::setupKeyPressCallback(){
  	instance = this;
  	::glutSpecialFunc(Game::KeyPressCallback);
}

void Game::setupKeyReleaseCallback(){
  	instance = this;
  	::glutSpecialUpFunc(Game::KeyReleaseCallback);
}

void Game::setupIdleCallback(){
  	instance = this;
   	::glutIdleFunc(Game::displayCallback);
}

void Game::setupMouseCallback(){
	instance = this;
	::glutMouseFunc(Game::mouseCallback);
}

void Game::initReset(){
	createParticles();
	createStars();
	createJet();

	speed = 1.0;
	directionZ = 0;
	directionY = 0;
	directionR = 0;
	radius = 100;
	theta = 0;
	fovy = 30;
	zNear = 1.0;
	zFar = 1000;
	vX = 450;
	vY = 80;
	vZ = 300;
	rX = 0;
	Index = 0;
	etime = 0.0;
	speedUp = 0;
	death = 0;
	blast = 0;
	angleX = 0;
	angleY = 0;
	angleZ = 0;
	camLock = 0;
	bFlag = 0;
	drawEx2 = 0;

	state = 1;

}
 
void Game::cleanUp() {

	// release FMOD Ex resources
	result = explosionSound1->release();
	result = slotFill->release();
	result = explosionSound2->release();
	result = jetSounds->release();
	result = menu->release();
	result = lzr->release();

}

void Game::createSphere(int Radius){
double PI = 3.14159;
float X1,Y1,X2,Y2,Z1,Z2;
float inc1,inc2,inc3,inc4,inc5,Radius1,Radius2;
int Resolution = 20;
	for(int w = 0; w < Resolution; w++) {
        for(int h = (-Resolution/2); h < (Resolution/2); h++){

 
			inc1 = (w/(float)Resolution)*2*PI;
			inc2 = ((w+1)/(float)Resolution)*2*PI;
			inc3 = (h/(float)Resolution)*PI;
			inc4 = ((h+1)/(float)Resolution)*PI;

			X1 = sin(inc1);
			Y1 = cos(inc1);
			X2 = sin(inc2);
			Y2 = cos(inc2);

// store the upper and lower radius, remember everything is going to be drawn as triangles
			Radius1 = Radius*cos(inc3);
			Radius2 = Radius*cos(inc4);

 


			Z1 = Radius*sin(inc3);
			Z2 = Radius*sin(inc4);

			sphereV.push_back(vec3(Radius1*X1,Z1,Radius1*Y1));
			sphereV.push_back(vec3(Radius1*X2,Z1,Radius1*Y2));
			sphereV.push_back(vec3(Radius2*X2,Z2,Radius2*Y2));



			sphereV.push_back(vec3(Radius1*X1,Z1,Radius1*Y1));
			sphereV.push_back(vec3(Radius2*X2,Z2,Radius2*Y2));
			sphereV.push_back(vec3(Radius2*X1,Z2,Radius2*Y1));

		}
	}
}

float Game::distance(float x1, float y1, float z1, float x2, float y2, float z2){
	return ( sqrt( pow((x1-x2),2) + pow( (y1-y2),2) + pow((z1-z2),2) ) );
}

// Creat Partices for asteroids, Explosion, Stars, and Jet Fuel
void Game::createParticles(){
	for ( int i = 0; i < MAX_PARTICLES; i++){
		particle newParticle;
		rain[i] = newParticle;
	}
}

void Game::createExplosion2(float x, float y, float z){
	for ( int i = 0; i < STARCOUNT; i++){
		particle newParticle(x,y,z, 3);
		explosion2[i] = newParticle;
	}
}
void Game::createExplosion(){
	for ( int i = 0; i < JET_PARTS; i++){
		particle newParticle(vX, vY, vZ, 2);
		explosion[i] = newParticle;
	}
}

void Game::createStars(){
		for ( int i = 0; i < STARCOUNT; i++){
		particle newParticle(rand()%250, rand()%500, rand()%300+150, 1);
		stars[i] = newParticle;
	}
}

void Game::createJet(){
	for (int i = 0; i < JET_PARTS; i++){
		particle newParticle(vX - (rand() % 4) / 5.0 + 4, vY + (rand() % 5) / 10.0 -2 , vZ + (rand() % 5) / 10.0 + 4, rand()%4 + 1);
		jet[i] = newParticle;
	}
}

// Regenerate new particles
void Game::addParticleJ(int i){
	particle newParticle(vX  - (rand() % 4)/5.0 + 4, vY + (rand() % 5) / 10.0 -2 , vZ + (rand() % 5) / 10.0 + 4, (rand()%8) + 1);
	jet[i] = newParticle;
}

void Game::addParticle(int i, float v){
	particle newParticle(rand()%250, rand()%500, rand()%300+150, (rand() % 3) + v);
	rain[i] = newParticle;
}

// Advance the particles
void Game::advanceExplosion(){
	int i;
	for ( i = 0; i < JET_PARTS; i++){
		float deltaX = (explosion[i].getV() + rand() % 3)/3;
		float deltaY = (explosion[i].getV() + rand() % 3)/3;
		float deltaZ = (explosion[i].getV() + rand() % 3)/3;
		explosion[i].setPos(explosion[i].getPos()+vec3(deltaX * explosion[i].getDir().x, deltaY * explosion[i].getDir().y, deltaZ * explosion[i].getDir().z) );
		explosion[i].setV(explosion[i].getV() * GRAVITY );	
		
		if( distance(explosion[i].getPos().x, explosion[i].getPos().y, explosion[i].getPos().z, vX, vY, vZ) > 200){
			state = 3;
		}
	}
}

void Game::advanceExplosion2(){
	int i;
	for ( i = 0; i < STARCOUNT; i++){
		float deltaX = explosion2[i].getV() + rand() % 4;
		float deltaZ = explosion2[i].getV() + rand() % 4;
		explosion2[i].setPos(explosion2[i].getPos()+vec3(deltaX * explosion2[i].getDir().x, explosion2[i].getDir().y, deltaZ * explosion2[i].getDir().z));
		explosion2[i].setV(explosion2[i].getV() * GRAVITY );	
		
		if( distance(explosion2[i].getPos().x, explosion2[i].getPos().y, explosion2[i].getPos().z, eX, eY, eZ) > 70){
			drawEx2 = 0;
			break;
		}
	}
}

void Game::advanceBullets(){
	bullets[0].setPos(bullets[0].getPos()+vec3(-bullets[0].getV(), 0, -0.05));
	bullets[1].setPos(bullets[1].getPos()+vec3(-bullets[1].getV(), 0, 0.05));

	if( abs( bullets[0].getPos().x - vX ) > 250 ){
		bFlag = 0;
	}
}
void Game::advanceStars(){
	int i;
	for ( i = 0; i < STARCOUNT; i++){
		stars[i].setPos(stars[i].getPos()+vec3(stars[i].getV(), 0,0));

		if (stars[i].getPos().x > 550){
			particle newParticle(rand()%250, rand()%500, rand()%300+150, stars[i].getV() + etime * 0.002);
			stars[i] = newParticle;
		
		}

		if( etime < 3 && stars[i].getV() < 4 ){
			stars[i].setV(stars[i].getV() + GRAVITY );
		}


	}
}

void Game::advanceParticleJ(){
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

void Game::advanceParticles(){
	int i;
	for ( i = 0; i < MAX_PARTICLES; i++){
		rain[i].setPos(rain[i].getPos()+vec3(rain[i].getV(), 0,0));
		rain[i].setAlive(rain[i].getAlive() + STEP);
		int r = rain[i].getRadius() + 4.5;

		if (rain[i].getPos().x > 550){
			addParticle(i, rain[i].getV() + etime *.002);
		
		}

		if(death < 1){
			if( distance(rain[i].getPos().x, rain[i].getPos().y, rain[i].getPos().z, vX, vY, vZ ) <= r){
				death = 1;
				jetChannel->setPaused(true);
				camLock = 1;
				directionY = 0;
				directionZ = 0;
				if(drawEx2 == 0) createExplosion2(rain[i].getPos().x, rain[i].getPos().y, rain[i].getPos().z);
				drawEx2 = 1;
				eX = rain[i].getPos().x;
				eY = rain[i].getPos().y;
				eZ = rain[i].getPos().z;
				addParticle(i, rain[i].getV() + etime *.002);
				break;
			}

			
		}
		
		if( (distance(rain[i].getPos().x, rain[i].getPos().y, rain[i].getPos().z, bullets[0].getPos().x, bullets[0].getPos().y, bullets[0].getPos().z ) <= r-6 || distance(rain[i].getPos().x, rain[i].getPos().y, rain[i].getPos().z, bullets[1].getPos().x, bullets[1].getPos().y, bullets[1].getPos().z) <= r-6)  && bFlag == 1){
				result = fmodSystem->playSound(FMOD_CHANNEL_FREE, explosionSound2, false, &channel);
				if(drawEx2 == 0) createExplosion2(rain[i].getPos().x, rain[i].getPos().y, rain[i].getPos().z);
				drawEx2 = 1;
				eX = rain[i].getPos().x;
				eY = rain[i].getPos().y;
				eZ = rain[i].getPos().z;
				bFlag = 0;
				addParticle(i, rain[i].getV() + etime *.002);
		}

	}
}

// Draw Particles
void Game::drawExplosion(mat4 mav){

	
	glUniform1i(flag,0);
	for (int i = 0; i < 30; i++ ){
		angleZ += rand() % 10;
		angleY += rand() % 10;
		angleZ += rand() % 10;
		mv_stack.push(mav);
			mat4 instance = Translate(explosion[i].getPos().x, explosion[i].getPos().y, explosion[i].getPos().z ) *  RotateZ(angleZ) * RotateX(angleX) * RotateY(angleY) * Scale(rand() % 8 + 1, rand() % 8 + 1  ,rand() % 8 + 1);
			glUniformMatrix4fv(model_view, 1, GL_TRUE, mav*instance);
			glDrawArrays(GL_TRIANGLES, 6, 3);
			mav = mv_stack.top();
		mv_stack.pop();
	}

}
void Game::drawExplosion3(mat4 mav){

	
	glUniform1i(flag,0);
	for (int i = 0; i < STARCOUNT; i++ ){
		angleZ += rand() % 10;
		angleY += rand() % 10;
		angleZ += rand() % 10;
		double j = (rand() % 8 + 1) / 8 ;
		mv_stack.push(mav);
			mat4 instance = Translate(explosion2[i].getPos().x, explosion2[i].getPos().y, explosion2[i].getPos().z ) * RotateX(angleZ) * RotateY(angleY) * RotateZ (angleZ) * Scale(j,j,j);
			glUniformMatrix4fv(model_view, 1, GL_TRUE, mav*instance);
			if(i % 2 == 0) glDrawArrays(GL_TRIANGLES, 0, 36);
			else glDrawArrays(GL_TRIANGLES, 6, 3);
			mav = mv_stack.top();
		mv_stack.pop();
	}

}
void Game::drawExplosion2(mat4 mav){

	glUniform1i(flag, 1);
	for (int i = 0; i < JET_PARTS; i++ ){
		blast = distance(vX, vY, vZ, explosion[i].getPos().x, explosion[i].getPos().y, explosion[i].getPos().z );
		glUniform1f(flame, blast * .03);
		mv_stack.push(mav);
			mat4 instance = Translate(explosion[i].getPos().x, explosion[i].getPos().y, explosion[i].getPos().z );
			glUniformMatrix4fv(model_view, 1, GL_TRUE, mav*instance);
			glDrawArrays(GL_POINTS, 0, 1);
			mav = mv_stack.top();
		mv_stack.pop();
	}
}


void Game::drawParticles(mat4 mav){
	for (int i = 0; i < MAX_PARTICLES; i++ ){
		if(debug == false){
			mv_stack.push(mav);
				mat4 instance = Translate(rain[i].getPos().x, rain[i].getPos().y, rain[i].getPos().z ) *  Scale(rain[i].getRadius(),rain[i].getRadius(),rain[i].getRadius());
				glUniformMatrix4fv(model_view, 1, GL_TRUE, mav*instance);
				glDrawArrays(GL_TRIANGLES, 0, points2.size());
				mav = mv_stack.top();
			mv_stack.pop();
		}

		else{
			drawSphere(rain[i].getRadius(),rain[i].getPos().x, rain[i].getPos().y, rain[i].getPos().z, mav);
		}
	}	
}

void Game::drawBullets(mat4 mav){
	mv_stack.push(mav);
		mat4 instance = Translate(bullets[0].getPos().x, bullets[0].getPos().y, bullets[0].getPos().z ) *  Scale(20 ,0.25, 0.25);
		glUniformMatrix4fv(model_view, 1, GL_TRUE, mav*instance);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		mav = mv_stack.top();
	mv_stack.pop();
	mv_stack.push(mav);
		mat4 instance2 = Translate(bullets[1].getPos().x, bullets[1].getPos().y, bullets[1].getPos().z ) *  Scale(20 ,0.25, 0.25);
		glUniformMatrix4fv(model_view, 1, GL_TRUE, mav*instance2);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		mav = mv_stack.top();
	mv_stack.pop();
}
void Game::drawStars(mat4 mav){
	for (int i = 0; i < STARCOUNT; i++ ){
		mv_stack.push(mav);
			mat4 instance = Translate(stars[i].getPos().x, stars[i].getPos().y, stars[i].getPos().z ) *  Scale(0.25,0.25,0.25);
			glUniformMatrix4fv(model_view, 1, GL_TRUE, mav*instance);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			mav = mv_stack.top();
		mv_stack.pop();
	}	
}

void Game::drawParticlesJ(mat4 mav){
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

void Game::quad( int a, int b, int c, int d ){
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

void Game::makeCube(){
	quad( 1, 0, 3, 2 );
    quad( 2, 3, 7, 6 );
    quad( 3, 0, 4, 7 );
    quad( 6, 5, 1, 2 );
    quad( 4, 5, 6, 7 );
    quad( 5, 4, 0, 1 );

    points[36] = vec3(-1, 1, 0.0);
    points[37] = vec3(-1, -1, 0.0);
    points[38] = vec3(1, -1, 0.0);
    points[39] = vec3(-1, 1, 0.0);
    points[40] = vec3(1, -1, 0.0);
    points[41] = vec3(1, 1, 0.0);

    tex_coords[36] = vec2(0,0);
    tex_coords[37] = vec2(0,1);
    tex_coords[38] = vec2(1,1);
    tex_coords[39] = vec2(0,0);
    tex_coords[40] = vec2(1,1);
    tex_coords[41] = vec2(1,0);

}


GLuint Game::loadBMP(string fileName){
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

void Game::drawMenu(string fileName){
	glBindBuffer(GL_ARRAY_BUFFER, buffer2);
	
    glEnableVertexAttribArray(loc);
	glVertexAttribPointer( loc, 3, GL_FLOAT, GL_FALSE, 0, 0 );

	
	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET( sizeof(points) ) );


	glEnableVertexAttribArray(vTexCoord);
	glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET( sizeof(points) + sizeof(normals) ) );
	texture = loadBMP(fileName);
	glUniform1i(flag, 8);
	glDrawArrays(GL_TRIANGLES, 36, 6);
}
void Game::init(){

	// Initiate buffers
	createSphere(1);
	makeCube();
	createExplosion();
	createParticles(); 
	createJet();  
	createStars();
	Ship->loadObj("ship.obj", vertices, norm, uvs);
	Roid->loadObj("asteroid.obj", points2, normals2, tex_coords2);

	// Space Ship Buffer
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
	sky = glGetUniformLocation(program, "sky");

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
  
 	// Skybox buffers
    GLuint vao2;
	glGenVertexArraysAPPLE(1, &vao2);
	glBindVertexArrayAPPLE( vao2 );

    glGenBuffers(1, &buffer2);
    glBindBuffer(GL_ARRAY_BUFFER, buffer2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normals) + sizeof(points) + sizeof(tex_coords), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(normals), normals);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(normals),sizeof(tex_coords), tex_coords);

    // Asteroid Buffers
    GLuint vao3;
    glGenVertexArraysAPPLE(1, &vao3);
    glBindVertexArrayAPPLE(vao3);

    glGenBuffers(1, &buffer3);
    glBindBuffer(GL_ARRAY_BUFFER, buffer3);
    glBufferData(GL_ARRAY_BUFFER, normals2.size() * sizeof(vec3) + points2.size() * sizeof(vec3) + tex_coords2.size() * sizeof(vec2), &points2[0], GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points2), &points2);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(points2), sizeof(normals2), &normals2);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(points2) + sizeof(normals2),sizeof(tex_coords2), &tex_coords2);
	
	GLuint vao4;
    glGenVertexArraysAPPLE(1, &vao4);
    glBindVertexArrayAPPLE(vao4);

	glGenBuffers(1, &buffer4);
	glBindBuffer(GL_ARRAY_BUFFER, buffer4);
	glBufferData(GL_ARRAY_BUFFER, sphereV.size() * sizeof(vec3), &sphereV[0], GL_STATIC_DRAW);

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0,0.0,0.0,1.0);

} 

void Game::printw (float x, float y, float z, char* format, ...){
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

void Game::drawSphere(double r, float x, float y, float z, mat4 mav){

	glBindBuffer(GL_ARRAY_BUFFER, buffer4);
	
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer( loc, 3, GL_FLOAT, GL_FALSE, 0, 0 );
	mv_stack.push(mav);
		mat4 instance = Translate(x, y, z) * Scale(r,r,r);
		glUniformMatrix4fv(model_view, 1, GL_TRUE, mav*instance);
		glDrawArrays(GL_LINES, 0, sphereV.size());
		mav = mv_stack.top();
	mv_stack.pop();
}
void Game::display(){

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	if( state == 1){
		 
		if (bg > 1) sign = -1;
		else if ( bg < 0) sign =1;
		bg = bg + (0.003 * sign);
		glUniform1f(sky,bg);
		// Set Up Projection and View 
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

		//Bind Buffer for Space Ship
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glEnableVertexAttribArray(loc);
		glVertexAttribPointer( loc, 3, GL_FLOAT, GL_FALSE, 0, 0 );

		glEnableVertexAttribArray(vTexCoord);
		glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET( sizeof(vertices) ));

		glEnableVertexAttribArray(vNormal);
		glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET( sizeof(vertices) + sizeof(uvs) ));

	
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
			if(death == 0) glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    		mv = mv_stack.top();
   		mv_stack.pop();
		
		glUniform1i(flag, 6);
    	if( debug ) drawSphere(4.5, vX,vY,vZ, mv);
   		//Bind Buffer for enviroment, stars, explosions and jet flames
		glBindBuffer(GL_ARRAY_BUFFER, buffer2);
	
    	glEnableVertexAttribArray(loc);
		glVertexAttribPointer( loc, 3, GL_FLOAT, GL_FALSE, 0, 0 );

	
		glEnableVertexAttribArray(vNormal);
		glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET( sizeof(points) ) );


		glEnableVertexAttribArray(vTexCoord);
		glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET( sizeof(points) + sizeof(normals) ) );
	
		if(bFlag == 1){
			glUniform1i(flag, 9);
			advanceBullets();
			drawBullets(mv);
		}

		if( drawEx2 == 1){
			//glUniform1i(flag, 1);
			advanceExplosion2();
			drawExplosion3(mv);
		}

		glUniform1i(flag, 0);
    	if(death == 1){
    		createExplosion();
    		death++;
    		glUniform1i(flag,1);
    		result = fmodSystem->playSound(FMOD_CHANNEL_FREE, explosionSound1, false, &channel);
    		result = fmodSystem->playSound(FMOD_CHANNEL_FREE, explosionSound2, false, &channel);
    		drawExplosion(mv);
    		drawExplosion2(mv);
    		advanceExplosion();
    	
    	}

    	else if ( death == 0){
    		etime += 0.1;
			advanceParticleJ();
    		glUniform1i(flag, 1);
    		drawParticlesJ(mv);
    	}

    	else{
    		glUniform1i(flag,1);
    		advanceExplosion();
    		drawExplosion(mv);
    		drawExplosion2(mv);
   		}
   		
   		glUniform1i(flag, 6);
   		mv_stack.push(mv);
			mat4 instance2 = Translate(450, 80, 300) * Scale(1000, 1000, 1000);
			glUniformMatrix4fv(model_view, 1, GL_TRUE, mv*instance2);
			glUniform1i(flag, 2);
    		glDrawArrays(GL_TRIANGLES, 12, 6);
    		glUniform1i(flag, 5);
    		if (texFlag == 0 ) texture = loadBMP("images/sky.bmp");
    		else texture = loadBMP("images/sky2.bmp");
    		glDrawArrays(GL_TRIANGLES, 0,6 );
    		glDrawArrays(GL_TRIANGLES, 12, 24);
    		mv = mv_stack.top();
    	mv_stack.pop();
  	

  		glUniform1i(flag, 6);
  		advanceStars();  	
  		drawStars(mv);

  		// Bind buffer for asteroids
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
    	if(debug == false)glUniform1i(flag, 4);
    	else glUniform1i(flag,6);
    	drawParticles(mv);
    	}
	}

	else if ( state == 0){
		drawMenu("images/menu_asteroid.bmp");
	}

	else if ( state == 3){
		drawMenu("images/gameover_asteroid.bmp");
	}

	else if ( state == 2){
		drawMenu("images/p_asteroid.bmp");
	}

	else if ( state == 4){
		drawMenu("images/control_asteroid.bmp");
	}

	glutSwapBuffers();
}

void Game::idle(){
	glutPostRedisplay();
}

void Game::soundCheck(){
	if ( directionZ != 0 || directionY != 0 ){
		result = jetChannel->setVolume(0.3);
	}

	else if ( directionZ == 0 && directionY == 0){
		result = jetChannel->setVolume(0.2);
	}
}
void Game::keyboard( int key, int x, int y ){
    switch( key ) {
	case 033: // Escape Key
	case 'q': case 'Q':
		cleanUp();
	    exit(0);
	    break;
	case GLUT_KEY_UP:
		if(camLock != 1 && state == 1){
			directionY = 1;
		}
		break;
	case GLUT_KEY_DOWN:
		if(camLock != 1 && state == 1){
			directionY = -1;
		}
		break;
	case GLUT_KEY_LEFT:
		if(camLock != 1 && state == 1){ 
			directionR = -1;
			directionZ = -1;
		}
		break;
	case GLUT_KEY_RIGHT:
		if(camLock != 1 && state == 1) {
			directionR = 1;
			directionZ = 1;
		}
		break;
	case 'j':
		theta--;
		break;
	case 'k':
		theta++;
		break;
	case 32:
		if(state == 1 && bFlag == 0 && death == 0){
			bFlag = 1;

			particle newBullet(vX, vY, vZ + 5, 4);
			particle newBullet2(vX, vY, vZ - 5, 4);
			bullets[0] = newBullet;
			bullets[1] = newBullet2;
			result = fmodSystem->playSound(FMOD_CHANNEL_FREE, lzr, false, &channel);
		}
		break;
	case 'p':
		if (state == 1) {
			jetChannel->setPaused(true);
			state = 2;
		}	
		else if(state == 2){
			result = fmodSystem->playSound(FMOD_CHANNEL_FREE, jetSounds, false, &jetChannel);
			state =1 ;
		}
		break;
	default:
		break;
	case 'w':
		debug = ! debug;
		break;
    }

    soundCheck();
   
}

void Game::keyRelease(int key, int x, int y){
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
		default:
			break;
	}

	soundCheck();
}


void Game::mouse(int button, int buttonState, int x, int y){
	
	if (button == GLUT_LEFT_BUTTON && buttonState == GLUT_DOWN && state == 0 && y > 500 && y < 555){
		result = fmodSystem->playSound(FMOD_CHANNEL_FREE, slotFill, false, &channel);
		state = 1;
		result = fmodSystem->playSound(FMOD_CHANNEL_FREE, jetSounds, false, &jetChannel);
		result = jetChannel->setVolume(0.2);

	}

	else if (button == GLUT_LEFT_BUTTON && buttonState == GLUT_DOWN && state == 0 && y > 560 && y < 605){
		result = fmodSystem->playSound(FMOD_CHANNEL_FREE, slotFill, false, &channel);
		state = 4;
	}
	else if (button == GLUT_LEFT_BUTTON && buttonState == GLUT_DOWN && state == 0 && y > 610 && y < 665){
		result = fmodSystem->playSound(FMOD_CHANNEL_FREE, slotFill, false, &channel);
		cleanUp();
		exit(0);
	}

	else if (button == GLUT_LEFT_BUTTON && buttonState == GLUT_DOWN && state == 4 && y > 590 && y < 640){
		result = fmodSystem->playSound(FMOD_CHANNEL_FREE, slotFill, false, &channel);
		state = 0;
	}

	else if (button == GLUT_LEFT_BUTTON && buttonState == GLUT_DOWN && state == 3 && y > 500 && y < 565){
		result = fmodSystem->playSound(FMOD_CHANNEL_FREE, slotFill, false, &channel);
		initReset();
		state = 1;
		result = fmodSystem->playSound(FMOD_CHANNEL_FREE, jetSounds, false, &jetChannel);
		result = jetChannel->setVolume(0.2);
	}

	else if (button == GLUT_LEFT_BUTTON && buttonState == GLUT_DOWN && state == 3 && y > 600 && y < 665){
		result = fmodSystem->playSound(FMOD_CHANNEL_FREE, slotFill, false, &channel);
		cleanUp();
		exit(0);
	}

	else if (button == GLUT_LEFT_BUTTON && buttonState == GLUT_DOWN && state == 2 && y > 580 && y < 635){
		result = fmodSystem->playSound(FMOD_CHANNEL_FREE, slotFill, false, &channel);
		cleanUp();
		exit(0);
	} 

	else if (button == GLUT_LEFT_BUTTON && buttonState == GLUT_DOWN && state == 2 && y > 425 && y < 490){
		result = fmodSystem->playSound(FMOD_CHANNEL_FREE, slotFill, false, &channel);
		initReset();
		state = 1;
		result = fmodSystem->playSound(FMOD_CHANNEL_FREE, jetSounds, false, &jetChannel);
		result = jetChannel->setVolume(0.2);
	}

	else if (button == GLUT_LEFT_BUTTON && buttonState == GLUT_DOWN && state == 2 && y > 500 && y < 565){
		result = fmodSystem->playSound(FMOD_CHANNEL_FREE, jetSounds, false, &jetChannel);
		result = jetChannel->setVolume(0.2);
		state = 1;
	}	
	

}

void Game::reshape(int width, int height){
	glViewport (0,0, width, height);
	aspect = GLfloat(width)/height;
}

// GLUT MAIN LOOP ____________________________________________________________
void Game::play(){

	char *argv[] = {"Asteroids"};
	int argc = 1;
	glutInit( &argc, argv);
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
	glutInitWindowSize(winH, winW);
	glutCreateWindow("Asteroids");

	init();
	setupMouseCallback();
	setupKeyPressCallback();
	setupKeyReleaseCallback();
	setupDisplayCallback();
	setupIdleCallback();
	setupReshapeCallback();
	

	glutMainLoop();

	return;
}
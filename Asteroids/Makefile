GCC_OPTIONS=-Wall -pedantic -I ../include
GL_OPTIONS=-framework GLUT -framework OpenGL
OPTIONS=$(GCC_OPTIONS) $(GL_OPTIONS)

all: main

main: main2.o Object.o env.o particle.o Game.o
	g++  -g main2.o Object.o env.o particle.o Game.o ../Common/initShader.o $(OPTIONS) fmodAPI/lib/libfmodex.dylib -o main

Game.o: Game.cpp Game.h
	g++ -c Game.cpp

main2.o: main2.cpp 
	g++ -c main2.cpp

Object.o: Object.cpp Object.h
	g++ -c Object.cpp

env.o: env.cpp env.h
	g++ -c env.cpp

particle.o: particle.cpp particle.h
	g++ -c particle.cpp

clean:
	rm -rf *o main
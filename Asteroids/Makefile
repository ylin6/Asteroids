GCC_OPTIONS=-Wall -pedantic -I ../include
GL_OPTIONS=-framework GLUT -framework OpenGL -framework Cocoa -lSOIL
OPTIONS=$(GCC_OPTIONS) $(GL_OPTIONS)
LIBPATH := -LSOIL/lib

all: main

main: main.o Object.o env.o particle.o
	g++  -g main.o Object.o env.o particle.o ../Common/initShader.o $(OPTIONS) $(LIBPATH) -o main

main.o: main.cpp 
	g++ -c main.cpp

Object.o: Object.cpp Object.h
	g++ -c Object.cpp

env.o: env.cpp env.h
	g++ -c env.cpp

particle.o: particle.cpp particle.h
	g++ -c particle.cpp

clean:
	rm -rf *o main
CC=g++
CFLAGS=-c -Wall
LDFLAGS=-lGL -lGLU -lglut

default: all

all: dom_sets

dom_sets: node.o key.o main.o
	$(CC) node.o key.o main.o -o dom_sets $(LDFLAGS)

node.o: node.cpp
	$(CC) $(CFLAGS) node.cpp

key.o: key.cpp
	$(CC) $(CFLAGS) key.cpp

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

clean:
	rm -rf *.o dom_sets

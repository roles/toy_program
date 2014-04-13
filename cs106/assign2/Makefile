CC=g++
CS106LIB=../lib/cs106lib.a
LDFLAGS=-lm
CPPFLAGS=-g -I../include -std=c++0x
VPATH=../include ../lib
PROG=partA partB

all: $(PROG)

$(PROG): % : %.o maze.o $(CS106LIB)
	$(CC) $^ $(LDFLAGS) -o $@

.PHONY: clean all
clean:
	rm -f *.o $(PROG)

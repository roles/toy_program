CC=g++
CS106LIB=../lib/cs106lib.a
LDFLAGS=-lm $(CS106LIB)
CPPFLAGS=-g -I../include -std=c++0x
VPATH=../include ../lib
PROG=warmupA warmupB recursion

all: $(PROG)

$(PROG): % : %.o lexicon.o $(CS106LIB)
	$(CC) $^ $(LDFLAGS) -o $@

.PHONY: clean
clean:
	rm -f *.o $(PROG)

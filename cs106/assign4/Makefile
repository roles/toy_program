CC=g++
CS106LIB=../lib/cs106lib.a
LDFLAGS=-lm $(CS106LIB)
CPPFLAGS=-g -I../include -std=c++0x
VPATH=../include ../lib
PROG=boggle

all: $(PROG)

$(PROG): % : %.o lexicon.o gboggle.o $(CS106LIB)
	$(CC) $^ $(LDFLAGS) -o $@

.PHONY: clean
clean:
	rm -f *.o $(PROG)

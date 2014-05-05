CC=g++
CPPFLAGS=-std=c++0x -g -I../include
LDFLAGS=-lm

dc: main.o parser.o error.o table.o lexer.o
	$(CC) $(LDFLAGS) $^ -o $@

clean:
	rm -rf *.o dc

CC=g++
LDFLAGS=-lm
CFLAGS=-g -I../include -I../lib -std=c++0x
CS106LIB=../lib/cs106lib.a

all: part1 part2 part3 part4 part5

part1: part1.cpp
	${CC} ${LDFLAGS} ${CFLAGS} part1.cpp ${CS106LIB} -o part1

part2: part2.cpp
	${CC} ${LDFLAGS} ${CFLAGS} part2.cpp ${CS106LIB} -o part2

part3: part3.cpp
	${CC} ${LDFLAGS} ${CFLAGS} part3.cpp ${CS106LIB} -o part3

part4: part4.cpp
	${CC} ${LDFLAGS} ${CFLAGS} part4.cpp ${CS106LIB} -o part4

part5: part5.cpp
	${CC} ${LDFLAGS} ${CFLAGS} part5.cpp ${CS106LIB} -o part5

clean:
	rm -f part1 part2 part3 part4 part5

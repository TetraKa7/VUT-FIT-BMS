CC=g++
CFLAGS=-std=c++11 -Wall -pedantic -O2
LIBS=-lm

all: bms1


bms1A: bms1.cpp
	$(CC) $(CFLAGS) $^ -o $@ 

clean:
	rm -f *.o bms1 *.txt

archive:
	zip xskuto00.zip Makefile *.cpp

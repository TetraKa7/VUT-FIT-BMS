CC=g++
CFLAGS=-std=c++11 -Wall -pedantic -O2
LIBS=-lm

all: bms2A bms2B


bms2A: bms2A.cpp
	$(CC) $(CFLAGS) $^ -o $@

bms2B: bms2B.cpp
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f *.o bms2A bms2B file.out file.out.err file.out.err.ok file2.out file2.out.err file2.out.err.ok

cleanTest:
	rm -f file.out file.out.err file.out.err.ok file2.out file2.out.err file2.out.err.ok test1 test2

archive:
	zip xskuto00.zip Makefile *.cpp rs rs_base
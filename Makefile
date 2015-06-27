CC = clang++
CFLAGS = -O2 -std=c++11 -Wall

all: main

main: main.o md5.o
	$(CC) $(CFLAGS) main.o md5.o -o main

main.o: main.cpp account_system.h
	$(CC) $(CFLAGS) -c main.cpp

md5.o: md5.h md5.cpp
	$(CC) $(CFLAGS) -c md5.cpp

clean:
	rm -rf *.o main

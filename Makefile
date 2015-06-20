CC = clang++
CFLAGS = -O2 -std=c++11 -Wall

all: main

main: main.o account.o md5.o
	$(CC) $(CFLAGS) main.o md5.o account.o -o main

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

account.o: account.h account.cpp
	$(CC) $(CFLAGS) -c account.cpp

md5.o: md5.h md5.cpp
	$(CC) $(CFLAGS) -c md5.cpp

clean:
	rm -rf *.o main

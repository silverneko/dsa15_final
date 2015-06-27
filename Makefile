CC = clang++
CFLAGS = -O2 -std=c++11 -Wall

all: final_project

final_project: main.o md5.o
	$(CC) $(CFLAGS) main.o md5.o -o final_project

main.o: main.cpp account_system.h
	$(CC) $(CFLAGS) -c main.cpp

md5.o: md5.h md5.cpp
	$(CC) $(CFLAGS) -c md5.cpp

clean:
	rm -rf *.o main final_project

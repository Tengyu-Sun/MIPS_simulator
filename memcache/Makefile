CC=g++
FLAG=-c -std=c++11
all: main.o Memory.o Memcache.o Cache.o
	$(CC) main.o Memory.o Memcache.o Cache.o -o main
main.o: main.cpp
	$(CC) $(FLAG) main.cpp
Memory.o: Memory.cpp
	$(CC) $(FLAG) main.cpp
Memcache.o: Memcache.cpp
	$(CC) $(FLAG) Memcache.cpp
Cache.o: Cache.cpp
	$(CC) $(FLAG) Cache.cpp
clean:
	rm *.o main

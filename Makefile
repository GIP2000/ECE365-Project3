all: hash.o heap.o graph.o useDijkstras.out

useDijkstras.out: useDijkstras.o heap.o hash.o graph.o
	g++ -o useDijkstras.out useDijkstras.o heap.o hash.o graph.o

useDijkstras.o: useDijkstras.cpp heap.h hash.h heap.cpp hash.cpp graph.o
	g++ -c useDijkstras.cpp

graph.o: hash.cpp hash.h heap.h heap.cpp
	g++ -c graph.cpp

heap.o: heap.cpp heap.h hash.h hash.cpp heap.h
	g++ -c heap.cpp

hash.o: hash.cpp hash.h
	g++ -c hash.cpp

debug:
	g++ -g -o useDijkstras.out useDijkstras.cpp hash.cpp heap.cpp graph.cpp

clean:
	rm -f *.out *.o *.stackdump *~

backup:
	test -d backups || mkdir backups
	cp *.cpp backups
	cp *.h backups
	cp Makefile backups

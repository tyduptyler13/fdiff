CXX=clang++
LIBS=`pkg-config capnp openssl --libs`

all: diff

clean:
	rm diff
	rm -rf *.o

storage.o: storage.capnp.c++ storage.capnp.h
	$(CXX) --std=c++11 -c storage.capnp.c++ -o storage.o

diff: diff.h diff.cc storage.o
	$(CXX) -o diff -std=c++11 -Wall -Wextra -pedantic -O2 -g diff.cc storage.o $(LIBS)

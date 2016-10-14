CXX=clang++
LIBS=`pkg-config capnp openssl --libs`

all: fdiff

clean:
	rm fdiff
	rm -rf *.o

storage.o: storage.capnp.c++ storage.capnp.h
	$(CXX) --std=c++11 -c storage.capnp.c++ -o storage.o

fdiff: fdiff.h fdiff.cc storage.o
	$(CXX) -o fdiff -std=c++11 -Wall -Wextra -pedantic -O2 -g fdiff.cc storage.o $(LIBS)

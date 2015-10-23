CXX=clang++

all: diff

clean:
	rm diff

diff: diff.h diff.cc
	$(CXX) -o diff -std=c++11 diff.cc -I.



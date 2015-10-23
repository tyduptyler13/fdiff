
#include "diff.h"

#include <thread>
#include <mutex>
#include <functional>
#include <queue>

using namespace std;
using namespace MyUPlay;

template <typename T, typename Container>
bool Diff<T, Container>::diff(const Container& a, const Container& b, unsigned threads){

	vector<vector<Change>> paths;
	mutex pathLock;

	vector<thread> threadPool;
	queue<unsigned> tasks;
	mutex taskLock;

	//Find matches in the list, while branching to find the optimal match.
	const function<void(const Container& a, const Container& b, vector<Change>&, unsigned)> matcher
	       	= [&](const Container& a, const Container& b, vector<Change>& changes, unsigned start){

		unsigned posA = start; //Current position for recording.
		unsigned recA = 0; //Recorded to. (For avoiding rerecording changes.)
		unsigned recB = 0;

		for (; posA < a.size(); ++posA){

			for (unsigned posB = recB; posB < b.size(); ++posB) {

				if (a[posA] == b[posB]){ //Match

					if (posB != rec){ //There must be new content in b.
						changes.emplace_back(Change(
					}

				}

			}

		}

	};

	//Retrieves the next task for a thread to munch on.
	function<void()> poolBoy = [&](){

		while(true){

			taskLock.lock();

			if (tasks.size() == 0) break;

			unsigned point = tasks.front();
			tasks.pop();

			taskLock.unlock();

			
			vector<Change> changes;
			pathLock.lock();
			paths.push(changes);
			pathLock.unlock();

			matcher(a[point], b);

		}

	};

	//Starting matching.

	for (unsigned i = 0; i < threads; ++i){
		threadPool.push_back(thread(&poolBoy));
	}

	//Wait for matching.

	for (thread& t : threadPool){
		t.join();
	}

	//Done with matching.

}


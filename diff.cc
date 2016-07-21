
#include "diff.h"

#include <fstream>
#include <exception>
#include <algorithm>

using namespace std;
using namespace MyUPlay;

const vector<Diff::Match>& Diff::getMatches() const {
	return matches;
}

const vector<Diff::Addition>& Diff::getAdditions() const {
	return additions;
}

Diff::Diff(string a, string b, bool binary){

	ifstream fileA;
	ifstream fileB;
	if (binary){
		fileA.open(a, ios::binary);
		fileB.open(b, ios::binary);
	} else {
		fileA.open(a);
		fileB.open(b);
	}

	if (!fileA.is_open() || !fileB.is_open() || !fileA.good() || !fileB.good()){
		throw runtime_error("At least one of the files could not be opened properly");
	}

	const string dataA = string(istreambuf_iterator<char>(fileA), istreambuf_iterator<char>()),
	      dataB = string(istreambuf_iterator<char>(fileB), istreambuf_iterator<char>());

	for (size_t i = 0; i + 8 < dataB.length(); ++i){

		string piece = dataB.substr(i, 8); //Min match length is 8

		Match m;

		for (size_t pos = dataA.find(piece); pos != string::npos; pos = dataA.find(piece, pos)){
			//positive match

			unsigned int length = 8;

			for (unsigned int posA = pos + 8, posB = i + 8; posA < dataA.length() && posB < dataB.length() && dataA[posA] == dataB[posB]; ++posA, ++posB, ++length){} //Gets the full length of the match.

			if (m.length < length){
				m.length = length;
				m.startA = pos;
				m.startA = i;
			}

		}

		if (m.length != 0){
			matches.push_back(m);
			i += m.length; //Skip the rest of the checks.
			//The above could be removed to improve possible matches, but isn't needed.
		}

	}

	//Add additions.
	for (unsigned match = 0, last = 0; match < matches.size(); ++match){

		const Match& m = matches[match];

		if (m.startB != last){ //Missing before.
			additions.push_back(Addition(last, dataB.substr(last, m.startB - last)));
		}

		last = m.startB + m.length;

	}


}

int main(int argc, char** argv){

}


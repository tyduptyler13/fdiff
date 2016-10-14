
#include "diff.h"
#include "storage.capnp.h"

#include <fstream>
#include <exception>
#include <algorithm>
#include <iostream>
#include <regex>

#include <openssl/sha.h>

#include <capnp/message.h>
#include <capnp/serialize-packed.h>

using namespace std;
using namespace MyUPlay;

Diff::Diff(string a, string b, bool binary){

	ifstream fileA;
	ifstream fileB;
	if (binary){
		fileA.open(a.c_str(), ios::binary); //Enables reading even the null character.
		fileB.open(b.c_str(), ios::binary);
	} else {
		fileA.open(a.c_str()); //Standard
		fileB.open(b.c_str());
	}

	if (!fileA.is_open() || !fileB.is_open() || !fileA.good() || !fileB.good()){
		throw runtime_error("At least one of the files could not be opened properly");
	}

	//Read the files into memory. We will be scanning them a lot so reading from disk probably won't work well.
	const string dataA = string(istreambuf_iterator<char>(fileA), istreambuf_iterator<char>()),
	      dataB = string(istreambuf_iterator<char>(fileB), istreambuf_iterator<char>());

	length = dataB.size();

	for (size_t i = 0; i + 8 < dataB.length(); ++i){

		string piece = dataB.substr(i, 8); //Min match length is 8

		Match m;

		for (size_t pos = dataA.find(piece); pos != string::npos; pos = dataA.find(piece, m.startA + m.length)){ //We will try several matches and get the best one.
			//positive match

			unsigned int length = 8;

			for (unsigned int posA = pos + 8, posB = i + 8; posA < dataA.length() && posB < dataB.length() && dataA[posA] == dataB[posB]; ++posA, ++posB, ++length){} //Gets the full length of the match.

			if (m.length < length){
				m.length = length;
				m.startA = pos;
				m.startB = i;
			}

		}

		if (m.length != 0){
			matches.push_back(m);
			i += m.length - 1; //Skip the rest of the checks.
			//The above could be removed to improve possible matches, but isn't needed.
		} //If this is not executed, repeat for a single character shift of the window.

	}

	//Add additions.
	for (unsigned match = 0, last = 0; match < matches.size(); ++match){

		const Match& m = matches[match];

		if (m.startB != last){ //Missing before.
			additions.push_back(Addition(last, dataB.substr(last, m.startB - last)));
		}

		last = m.startB + m.length;

	}

	SHA256_CTX sha256;
	SHA256_Init(&sha256);
	SHA256_Update(&sha256, dataB.c_str(), dataB.size());
	SHA256_Final(hash.data(), &sha256);

}

class FStream : public kj::OutputStream {

	ofstream out;

public:

	FStream(const char* file) : out(file) {}

	void write(const void* buffer, size_t size) override {
		out.write((const char*)buffer, size);
	}

};

static const regex helpReg("-h|--help", regex_constants::icase);
static const regex patchReg("-p");
static const regex binaryReg("-b");

int main(int argc, char** argv){

	bool help = false;
	bool patch = false;
	bool binary = false;

	for (int i = 0; i < argc; ++i){
		if (regex_match(argv[i], helpReg)){
			help = true;
		} else if (regex_match(argv[i], patchReg)){
			patch = true;
		} else if (regex_match(argv[i], binaryReg)){
			binary = true;
		}
	}

	if (help){
		cout << "Use -h or --help to display this message." << endl
				<< "diff [original] [new] [target.patch] (options) - Produces a patch file between the two files. original -> new." << endl
				<< "diff -p [patchfile] [original] [new target] - Will migrate original to new using patch file." << endl
				<< "Options:" << endl
				<< "-b - Enables binary files. If this isn't used with binary data, it could cause problems with null bytes." << endl;
		return 0;
	} else if (argc < 4) {
		cout << "Not enough arguments. Use -h or --help for help." << endl;
		return -1;
	}

	if (patch){
		cout << "Patching is not yet implemented." << endl;
	} else {
		Diff diff(argv[1], argv[2], binary);

		capnp::MallocMessageBuilder message;

		Patch::Builder patch = message.initRoot<Patch>();
		capnp::Data::Builder hash = patch.initHash(32);
		const array<unsigned char, 32>& h = diff.getHash();
		for (unsigned i = 0; i < 32; ++i){
			hash[i] = h[i];
		}

		patch.setLength(diff.getLength());

		const vector<Diff::Match>& matches = diff.getMatches();

		capnp::List<Copy>::Builder copies = patch.initCopyList(matches.size());

		for (unsigned i = 0; i < matches.size(); ++i){
			Copy::Builder copy = copies[i];
			copy.setOrigin(matches[i].startA);
			copy.setDest(matches[i].startB);
			copy.setLength(matches[i].length);
		}

		const vector<Diff::Addition>& additions = diff.getAdditions();

		capnp::List<Create>::Builder creates = patch.initCreateList(additions.size());

		for (unsigned i = 0; i < additions.size(); ++i){
			Create::Builder create = creates[i];
			create.setDest(additions[i].startB);
			capnp::Data::Builder data = create.initData(additions[i].data.size());
			const string& dat = additions[i].data;
			for (unsigned i = 0; i < dat.size(); ++i){
				data[i] = dat[i];
			}
		}

		FStream io(argv[3]);

		writeMessage(io, message);

	}

	return 0;

}


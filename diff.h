#ifndef MYUPLAY_DIFF
#define MYUPLAY_DIFF

#include <vector>
#include <string>
#include <array>

namespace MyUPlay {

	class Diff {

		public:

			/*Places where the files are matched.
			 *  (Moves cost very little, exact matches have lowest cost, deletes cost nothing)
			 */
			struct Match { //Places where things were moved or remain in a different document.
				unsigned int startA;
				unsigned int startB;
				unsigned int length = 0;
			};

			struct Addition { //Places where new content was created.
				unsigned int startB;
				std::string data;

				Addition(unsigned int startB, std::string data) : startB(startB), data(data){}
			};

		private:

			std::vector<Match> matches;
			std::vector<Addition> additions;
			std::array<unsigned char, 32> hash;
			unsigned length; //New files output length.

		public:

			void patch(std::string out);
			const std::vector<Match>& getMatches() const {
				return matches;
			}
			const std::vector<Addition>& getAdditions() const {
				return additions;
			}
			const std::array<unsigned char, 32>& getHash() const {
				return hash;
			}
			unsigned getLength() const {
				return length;
			}

			Diff(std::string a, std::string b, bool binary = false);

	};

}

#endif


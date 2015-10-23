#ifndef MYUPLAY_DIFF
#define MYUPLAY_DIFF

#include <vector>
#include <ostream>

namespace MyUPlay {

	template <typename T, typename Container = std::vector<T>>
	class Diff {

		private:

		struct Change {
			unsigned startA, endA;
			unsigned startB, endB;
			Container content;
		};

		std::vector<Change> changes;

		public:

		bool diff(const Container& a, const Container& b, unsigned threads);

		void generatePatch(const std::ostream&) const; 

	};

}

#endif


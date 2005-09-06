// Matthew Henderson, 4.9.2005 (Swansea)

/*!
  \file AssociativeContainers.hpp
  \brief Various associative containers.
*/

#ifndef ASSOCIATIVECONTAINERS_09646l

#define ASSOCIATIVECONTAINERS_09646l

#include <string>
#include <set>
#include <functional>
#include <algorithm>

namespace OKlib {

  namespace SearchDataStructures {

    template <class Range>
    class AssociativePrefixContainer {
    private:
      struct SortLexicographical : std::binary_function<const Range&, const Range&,bool> {
        // ####################
      };

      typedef typename std::set<Range, SortLexicographical> SetRanges;
      SetRanges prefix_set;
    public:
      typedef typename SetRanges::const_iterator iterator;
      iterator begin() const {
	return prefix_set.begin();
      };
      iterator end() const {
	return prefix_set.end();
      };
      AssociativePrefixContainer() {};
      std::pair<iterator,bool> insert(const Range& x) {
	return prefix_set.insert(x);
      };
      iterator first_extension(const Range& r) const; // ####################
    };

  }

}

#endif

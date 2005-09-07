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

#include <boost/range/functions.hpp>

namespace OKlib {

  namespace SearchDataStructures {

    template <class Range>
    class AssociativePrefixContainer {
    private:
      struct SortLexicographical : std::binary_function<const Range&, const Range&, bool> {
	bool operator()(const Range& arg1, const Range& arg2) const {
	  return std::lexicographical_compare(boost::begin(arg1),boost::end(arg1),boost::begin(arg2),boost::end(arg2));
}
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
      iterator first_extension(const Range& r) const {
	const iterator& lower_bound(prefix_set.lower_bound(r));
        const iterator& end(prefix_set.end());
        if (lower_bound == end)
          return end;
        if (std::equal(boost::begin(r), boost::end(r), boost::begin(*lower_bound)))
          return lower_bound;
        else
          return end;
      };
    };

  }

}

#endif

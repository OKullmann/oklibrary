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
      iterator begin() const; // ###########
      iterator end() const; // #############
      AssociativePrefixContainer() {}
      // insert ###############
      iterator first_extension(const Range& r) const; // ####################
    };

  }

}

#endif

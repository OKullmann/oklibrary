// Matthew Henderson, 4.9.2005 (Swansea)

/*!
  \namespace OKlib::SearchDataStructures
  \brief Data structures with additional search functionality.
*/

/*!
  \file AssociativeContainers.hpp
  \brief Associative containers.
*/

#ifndef ASSOCIATIVECONTAINERS_09646l

#define ASSOCIATIVECONTAINERS_09646l

#include <string>
#include <set>
#include <functional>
#include <algorithm>
#include <utility>

#include <boost/range/functions.hpp>
#include <boost/utility.hpp>

namespace OKlib {

  namespace SearchDataStructures {

    /*!
      \class AssociativePrefixContainer
      \brief Class for associative container of ranges with lexicographic ordering.
      \todo Create a concept (including providing appropriate container functionality).
    */

    template <class Range>
    class AssociativePrefixContainer {
      /*!
        \class SortLexicographical
        \brief Functor for lexicographic ordering of two range arguments.
      */
      struct SortLexicographical : std::binary_function<const Range&, const Range&, bool> {
	bool operator()(const Range& arg1, const Range& arg2) const {
	  return std::lexicographical_compare(boost::begin(arg1), boost::end(arg1), boost::begin(arg2), boost::end(arg2));
}
      };
      
      typedef typename std::set<Range, SortLexicographical> SetRanges;
    
      SetRanges prefix_set;

    public:

      AssociativePrefixContainer() {};

      AssociativePrefixContainer(const Range& range) {
         typedef typename boost::range_iterator<Range>::type iterator_t;
         for(iterator_t begin(boost::begin(range)); begin!=boost::end(range); ++begin)
           prefix_set.insert(*begin);
       };

      typedef typename SetRanges::const_iterator iterator;
      typedef iterator const_iterator;
      typedef std::pair<iterator,bool> checked_iterator_type;

      iterator begin() const {
	return prefix_set.begin();
      }
      iterator end() const {
	return prefix_set.end();
      }

      std::pair<iterator,bool> insert(const Range& x) {
	return prefix_set.insert(x);
      }

      iterator first_extension(const Range& r) const {
	const iterator& lower_bound(prefix_set.lower_bound(r));
        const iterator& end(prefix_set.end());
        if (lower_bound == end)
          return end;
        if (boost::size(r) > boost::size(*lower_bound))
          return end;
        if (std::equal(boost::begin(r), boost::end(r), boost::begin(*lower_bound)))
          return lower_bound;
        else
          return end;
      }

       checked_iterator_type first_extension_uniqueness_checked(const Range& r) const {
       const iterator& first(first_extension(r));
       const iterator& end(prefix_set.end());
        if (first == end)
          return checked_iterator_type(first, true);
        const iterator& next(boost::next(first));
        if (next == end)
          return checked_iterator_type(first, true);
        if (boost::size(r) > boost::size(*next))
          return checked_iterator_type(first, true);
        if (std::equal(boost::begin(r), boost::end(r), boost::begin(*next)))
          return checked_iterator_type(first, false);
        else
          checked_iterator_type(first, true);
      }
    };

  }

}

#endif

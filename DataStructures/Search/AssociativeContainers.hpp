// Matthew Henderson, 4.9.2005 (Swansea)
/* Copyright 2005 - 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file DataStructures/Search/AssociativeContainers.hpp
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

    // ##############################################################

    /*!
      \class AssociativePrefixContainer
      \brief Class for associative container of ranges with lexicographic ordering.

      \todo In a new module Iterators a metafunktion InverseRange<Range>
      should be implemented (in sub-module RangeAdaptors).
      \todo The lexicographical order on ranges should become independent
      (in module OrderRelations).
      \todo Add more natural functionality (for example ==, !=).
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
    
      SetRanges prefix_set; // range_type ???????????????????

      template <class Range2>
      void prefix_set_assign(const Range2& range) {
        prefix_set.clear();
        prefix_set.insert(boost::begin(range), boost::end(range));
      }

    public:

      typedef Range prefix_type;
      typedef typename SetRanges::const_iterator iterator;
      typedef iterator const_iterator;
      typedef std::pair<iterator,bool> checked_iterator_type;
      typedef typename SetRanges::size_type size_type;

      AssociativePrefixContainer() {};

      template <class Range2>
      AssociativePrefixContainer(const Range2& range) {
        prefix_set_assign(range);
      }

      template <class Range2>
      void assign(const Range2& range) {
        prefix_set_assign(range);
      }
      
      iterator begin() const {
	return prefix_set.begin();
      }
      iterator end() const {
	return prefix_set.end();
      }

      size_type size() const {
        return prefix_set.size();
      }

      std::pair<iterator,bool> insert(const Range& x) {
	return prefix_set.insert(x);
      }

      iterator first_extension(const Range& r) const {
        const iterator& lower_bound(prefix_set.lower_bound(r));
        const iterator& end(prefix_set.end());
        if (lower_bound == end)
          return end;
        if (boost::distance(r) > boost::distance(*lower_bound))
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
        if (boost::distance(r) > boost::distance(*next))
          return checked_iterator_type(first, true);
        if (std::equal(boost::begin(r), boost::end(r), boost::begin(*next)))
          return checked_iterator_type(first, false);
        else
          return checked_iterator_type(first, true);
      }
    };

    // ##############################################################

  }
}

#endif

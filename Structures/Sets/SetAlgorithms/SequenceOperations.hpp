// Oliver Kullmann, 12.6.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Structures/Sets/SetAlgorithms/SequenceOperations.hpp
  \brief Basic operations for sequences

  Contains: sum of ranges.

  \todo Moving Sum_sizes fully to the range concept (accepting only ranges as input)
*/

#ifndef SEQUENCEOPERATIONS_iKnB547
#define SEQUENCEOPERATIONS_iKnB547

#include <functional>
#include <numeric>

#include <boost/range/functions.hpp>
#include <boost/range/metafunctions.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/iterator/transform_iterator.hpp>

namespace OKlib {
  namespace SetAlgorithms {

    template <class Range>
    struct Size : std::unary_function<const Range&, typename boost::range_size<Range>::type> {
      typename boost::range_size<Range>::type operator() (const Range& r) const {
        using boost::distance;
        return distance(r);
      }
    };
    // ToDo: moving to a module for function objects?

    template <typename InputIterator_sets>
    struct Sum_sizes : std::binary_function<InputIterator_sets, InputIterator_sets, typename boost::range_size<typename InputIterator_sets::value_type>::type> {
      typename boost::range_size<typename InputIterator_sets::value_type>::type operator() (const InputIterator_sets begin, const InputIterator_sets end) const {
        typedef typename boost::range_size<typename InputIterator_sets::value_type>::type size_type;
        typedef typename InputIterator_sets::value_type Range;
        typedef Size<Range> size_functor;
        typedef boost::transform_iterator<size_functor, InputIterator_sets> transform_iterator;
        return std::accumulate(transform_iterator(begin), transform_iterator(end), size_type(0));
      }
    };
    // ToDo: moving to a module for function objects?

    template <typename InputIterator_sets>
    inline typename boost::range_size<typename InputIterator_sets::value_type>::type sum_sizes(const InputIterator_sets begin, const InputIterator_sets end) {
      return Sum_sizes<InputIterator_sets>()(begin, end);
    }

    // -----------------------------------------------------------------------------------------------------------------------------------

  }

}

#endif

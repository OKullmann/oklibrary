// Oliver Kullmann, 19.5.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Structures/Sets/SetAlgorithms/BasicSetOperations.hpp
  \brief Classes Union<InputIterator_sets, OutputIterator> and
  Intersection<InputIterator_sets, OutputIterator> for the computation
  of unions and intersections of sequences of sets.
*/

#ifndef BASICSETOPERATIONS_lJnB45
#define BASICSETOPERATIONS_lJnB45

#include <set>
#include <utility>
#include <vector>
#include <algorithm>
#include <iterator>
#include <cassert>
#include <functional>

#include <boost/range/functions.hpp>
#include <boost/range/metafunctions.hpp>

#include <OKlib/General/IteratorHandling.hpp>

namespace OKlib {
  namespace SetAlgorithms {

    /*!
      \class Disjoint
      \brief Unary predicate checking whether a set B is disjoint
      to the given set A
    */

    template <class Set>
    struct Disjoint : std::unary_function<const Set&, bool> {
      typedef Set set_type;
      typedef typename set_type::size_type size_type;
      typedef typename set_type::const_iterator iterator;
      const set_type& A;
      const size_type a;
      const iterator begin;
      const iterator end;
      Disjoint(const set_type& A) :
        A(A), a(A.size()), begin(A.begin()), end(A.end()) {}
      bool operator() (const set_type& B) const {
        if (a == 0) return true;
        if (B.size() == 0) return true;
        if (*--iterator(end) < *B.begin() or *--(B.end()) < *begin)
          return true;
        IteratorHandling::Advance_Count<typename set_type::value_type> ci;
        std::set_intersection(begin,end, B.begin(),B.end(), ci);
        return (ci.count() == 0);
      }
    };

    /*!
      \class Union
      \brief Functor: Given a range (input iterators) of ordered sequences, which are given as input-iterator-ranges, and an output iterator, compute the union.
    */

    template <typename InputIterator_sets, typename OutputIterator>
    struct Union {
      // ToDo: Adding specification of Union as functor class
      // ToDo: Adding concept etc.
      // InputIterator_sets must be an input iterator
      // InputIterator_sets::value_type must be a model of the boost::range concept
      // The range iterators are input iterators.
      // OutputIterator must be an output iterator.
      // Comment: When OutputIterator is std::back_insert_iterator etc., then return value is useless (except for further insertions).
      // ToDo: When OutputIterator is an *InputIterator*, then we can realise laziness (same for intersection)!
    private :
      typedef typename InputIterator_sets::value_type range_type;
      typedef typename boost::range_const_iterator<range_type>::type InputIterator_elements;
      typedef std::pair<InputIterator_elements, InputIterator_elements> Range;
      struct comparison : std::binary_function<Range, Range, bool> {
        bool operator() (const Range& r1, const Range& r2) const {
          assert(r1.first != r1.second);
          assert(r2.first != r2.second);
          return *r1.first < *r2.first;
        }
      };
      typedef std::multiset<Range, comparison> Multiset;
      typedef typename Multiset::iterator multiset_iterator;
      typedef typename InputIterator_elements::value_type value_type;
      typedef std::vector<Range> Vector;
      typedef typename Vector::iterator vector_iterator;
    public :
      Union() {} // to enable constant objects
      OutputIterator operator() (const InputIterator_sets begin_sets, const InputIterator_sets end_sets, OutputIterator out) const {
        Multiset first_elements;
        for (InputIterator_sets i = begin_sets; i != end_sets; ++i) {
          using boost::begin;
          using boost::end;
          using boost::empty;
          const range_type& r(*i);
          if (not empty(r))
            first_elements.insert(Range(begin(r), end(r)));
        }
        Vector  to_be_updated;
        to_be_updated.reserve(first_elements.size());
        for (; not first_elements.empty(); to_be_updated.clear()) {
          const multiset_iterator& begin_new_elements(first_elements.begin());
          const Range& r(*begin_new_elements);
          assert(r.first != r.second);
          *(out++) = *(r.first);
          const multiset_iterator& end_new_elements(first_elements.upper_bound(r));
          assert(first_elements.lower_bound(r) == begin_new_elements);
          assert(begin_new_elements != end_new_elements);
          std::copy(begin_new_elements, end_new_elements, std::back_inserter(to_be_updated));
          first_elements.erase(begin_new_elements, end_new_elements);
          const vector_iterator& update_end(to_be_updated.end());
          for (vector_iterator i(to_be_updated.begin()); i != update_end; ++i) {
            Range& r(*i);
            if (++r.first != r.second)
              first_elements.insert(r);
          }
        }
        return out;
      }
    };

    template <typename InputIterator_sets, typename OutputIterator>
    inline OutputIterator union_sets(const InputIterator_sets begin_sets, const InputIterator_sets end_sets, const OutputIterator out) {
      return Union<InputIterator_sets, OutputIterator>()(begin_sets, end_sets, out);
    }

    // --------------------------------------------------------------------------------------------------------------------------

        /*!
      \class Intersection
      \brief Functor: Given a range (input iterators) of ordered sequences, which are given as input-iterator-ranges, and an output iterator, compute the intersection.
    */
    
    template <typename InputIterator_sets, typename OutputIterator>
    struct Intersection {
      // ToDo: see Union
      // ToDo: factor out common definitions
      // ToDo: If the range iterators are random access iterators, then we have a better algorithm.
    private :
      typedef typename InputIterator_sets::value_type range_type;
      typedef typename boost::range_const_iterator<range_type>::type InputIterator_elements;
      typedef std::pair<InputIterator_elements, InputIterator_elements> Range;
      struct comparison : std::unary_function<Range, bool> {
        bool operator() (const Range& r1, const Range& r2) const {
          return *(r1.first) < *(r2.first);
        }
      };
      typedef std::multiset<Range, comparison> Multiset;
      typedef typename Multiset::iterator multiset_iterator;
      typedef typename Multiset::size_type multi_set_size_type;
      typedef typename InputIterator_elements::value_type value_type;
      typedef std::vector<Range> Vector;
      typedef typename Vector::iterator vector_iterator;
    public :
      Intersection() {} // to enable constant objects
      OutputIterator operator() (const InputIterator_sets begin, const InputIterator_sets end, OutputIterator out) const {
        assert(begin != end);
        Multiset first_elements;
        for (InputIterator_sets i = begin; i != end; ++i) {
          using boost::begin;
          using boost::end;
          using boost::empty;
          const range_type& r(*i);
          if (not empty(r))
            first_elements.insert(Range(begin(r), end(r)));
          else
            return out;
        }
        Vector to_be_updated;
        to_be_updated.reserve(first_elements.size());
        for (;; to_be_updated.clear()) {
          const multiset_iterator& begin_new_elements(first_elements.begin());
          const Range& r(*begin_new_elements);
          assert(r.first != r.second);
          const multiset_iterator& end_new_elements = first_elements.upper_bound(r);
          if (end_new_elements == first_elements.end()) {
            *(out++) = *(r.first);
            std::copy(begin_new_elements, end_new_elements, std::back_inserter(to_be_updated));
            first_elements.clear();
          }
          else {
            std::copy(begin_new_elements, end_new_elements, std::back_inserter(to_be_updated));
            first_elements.erase(begin_new_elements, end_new_elements);
          }
          const vector_iterator& update_end(to_be_updated.end());
          for (vector_iterator begin(to_be_updated.begin()); begin != update_end; ++begin) {
            Range& r(*begin);
            if (++r.first != r.second)
              first_elements.insert(r);
            else
              return out;
            // ToDo: Using STL
          }
        }
        return out;
      }
    };

    template <typename InputIterator_sets, typename OutputIterator>
    inline OutputIterator intersection_sets(const InputIterator_sets begin_sets, const InputIterator_sets end_sets, const OutputIterator out) {
      return Intersection<InputIterator_sets, OutputIterator>()(begin_sets, end_sets, out);
    }
    
  }

}

#endif

// Oliver Kullmann, 10.11.2002 (Swansea)
/* Copyright 2002 - 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file General/IteratorHandling.hpp
  \brief Deprecated (shall be moved to Programming/Iterators). Tools for handling of iterators
*/

#ifndef ITERATORHANDLINGWAECHTER

#define ITERATORHANDLINGWAECHTER

#include <algorithm>
#include <iterator>
#include <cstddef>

#include <boost/iterator/transform_iterator.hpp>
#include <boost/range/functions.hpp>
#include <boost/range/metafunctions.hpp>
#include <boost/range/iterator_range.hpp>

#include <Transitional/Programming/Utilities/OrderRelations/DerivedRelations.hpp>

#include <Transitional/General/FunctionHandling.hpp>

namespace IteratorHandling {

  // ----------------------------------------
  // Counting
  // ----------------------------------------

  // Output iterator, which only counts how many time it has been advanced
  template <typename T>
  class Advance_Count : public std::iterator<std::output_iterator_tag, T> {
  public :
    Advance_Count() : c(0) {}
    Advance_Count(const Advance_Count& ac) : c(ac.c) {}
    Advance_Count& operator = (const Advance_Count& ac) { c = ac.c; return *this; }
    Advance_Count& operator = (const T&) { return *this; }
    Advance_Count& operator * () { return *this; }
    Advance_Count& operator ++ () { ++c; return *this; }
    Advance_Count operator ++ (int) {
      Advance_Count ac(*this);
      ++c;
      return ac;
    }

    std::ptrdiff_t count() const { return c; }

  private :
    std::ptrdiff_t c;
  };

  // Random access iterator for counting
  template <typename Int = std::ptrdiff_t>
  class Count_iterator : public std::iterator<std::random_access_iterator_tag, Int, Int> {
    Int c;
  public :
    Count_iterator() : c(0) {}
    Count_iterator(Int x) : c(x) {}

    friend inline bool operator ==(const Count_iterator lhs, const Count_iterator rhs) { return lhs.c == rhs.c; }
    friend inline bool operator <(Count_iterator lhs, Count_iterator rhs) { return lhs.c < rhs.c; }

    Int& operator *() { return c; }

    Count_iterator& operator ++() { ++c; return *this; }
    Count_iterator& operator --() { --c; return *this; }
    Count_iterator operator ++(int) { Count_iterator ci(*this); ++c; return ci; }
    Count_iterator operator --(int) { Count_iterator ci(*this); --c; return ci; }
    Count_iterator& operator +=(Int n) { c += n; return *this; }
    Count_iterator& operator -=(Int n) { c -= n; return *this; }
    friend inline Count_iterator operator + (Count_iterator ci, Int n) { ci += n; return ci; }
    friend inline Int operator - (Count_iterator lhs, Count_iterator rhs) { return lhs.c - rhs.c; }
  };
  
  OKLIB_DERIVED_UNEQUAL_TEMPLATE1(Count_iterator)
  OKLIB_DERIVED_ORDERRELATIONS_TEMPLATE1(Count_iterator)
  
  template <typename Int>
  Count_iterator<Int> count_iterator(Int x) { return Count_iterator<Int>(x); }

  // ----------------------------------------
  // Arithmetical progressions
  // ----------------------------------------

  // Arithmetical progressions (as a collection)
  // Collection a, a + d, a + 2d, ..., a + m * d
  // a, d numerical, m integral

  // ToDo: Integration with class in Hypergraphs/Colourings/Van_der_Waerden_hypergraph.hpp

  template <typename Num, typename Int>
  class Arithmetical_progression {
    const Num a, d;
    const Int m;

  public :
    Arithmetical_progression(const Num a, const Num d, const Int m) : a(a), d(d), m(m) {}
    Int size() const { return m+1; }
    
    class iterator : public Count_iterator<Int> {
      const Arithmetical_progression* ap;
      iterator(const Arithmetical_progression* ap) : ap(ap) {}
      iterator(const Arithmetical_progression* ap, Int i) : Count_iterator<Int>(i), ap(ap) {}
      friend class Arithmetical_progression;

    public :
      typedef Num value_type;
      typedef const Num* pointer;
      typedef Num reference;
      iterator() : ap(0) {}
      reference operator *() {
	assert(ap);
	return ap -> eval(Count_iterator<Int>::operator *());
      }
      reference operator *() const {
	assert(ap);
	return ap -> eval(Count_iterator<Int>::operator *());
      }

      friend inline bool operator ==(const iterator lhs, const iterator rhs) {
	return static_cast<Count_iterator<Int> >(lhs) == static_cast<Count_iterator<Int> >(rhs) and lhs.ap == rhs.ap;
      }
      friend OKLIB_DERIVED_UNEQUAL(iterator); // needed inside this class for type deduction

      iterator& operator ++() {
	Count_iterator<Int>::operator ++();
	return *this;
      }
      iterator& operator --() {
	Count_iterator<Int>::operator --();
	return *this;
      }
      iterator operator ++(int) {
	iterator i(*this); operator ++();
	return i;
      }
      iterator operator --(int) {
	iterator i(*this); operator --();
	return i;
      }
      iterator& operator +=(Int n) {
	Count_iterator<Int>::operator +=(n);
	return *this;
      }
      iterator& operator -=(Int n) {
	Count_iterator<Int>::operator -=(n);
	return *this;
      }
      friend inline iterator operator + (iterator i, Int n) {
	i += n; return i;
      }
      friend inline Int operator - (iterator lhs, iterator rhs) {
	return static_cast<Count_iterator<Int> >(lhs) - static_cast<Count_iterator<Int> >(rhs);
      }
    };
    
    typedef iterator const_iterator;
    iterator begin() { return iterator(this); }
    iterator end() {
      assert(m >= 0);
      return iterator(this, m + 1);
    }
    
  private :
    Num eval(Int i) const { return a + i * d; }
  };


  // ----------------------------------------
  // Iterator adaptors for sequences
  // ----------------------------------------

  /*!
    \class IteratorFirst
    \brief Metafunction for converting iterators over pairs into iterators over the first component.
  */

  template <class Iterator>
  struct IteratorFirst {
    typedef typename Iterator::value_type value_type;
    typedef FunctionHandling::First<value_type> first;
    typedef boost::transform_iterator<first, Iterator> type;
  };
  template <class Iterator>
  struct IteratorFirstMutable {
    typedef typename Iterator::value_type value_type;
    typedef FunctionHandling::FirstMutable<value_type> first;
    typedef boost::transform_iterator<first, Iterator> type;
  };


  template <class Iterator>
  typename IteratorFirst<Iterator>::type iterator_first(const Iterator& it) {
    typedef typename IteratorFirst<Iterator>::type iterator;
    return iterator(it);
  }
  template <class Iterator>
  typename IteratorFirstMutable<Iterator>::type iterator_first(Iterator& it) {
    typedef typename IteratorFirst<Iterator>::type iterator;
    return iterator(it);
  }


  /*!
    \class IteratorSecond
    \brief Metafunction for converting iterators over pairs into iterators over the second component.
  */

  template <class Iterator>
  struct IteratorSecond {
    typedef typename Iterator::value_type value_type;
    typedef FunctionHandling::Second<value_type> second;
    typedef boost::transform_iterator<second, Iterator> type;
  };
  template <class Iterator>
  struct IteratorSecondMutable {
    typedef typename Iterator::value_type value_type;
    typedef FunctionHandling::SecondMutable<value_type> second;
    typedef boost::transform_iterator<second, Iterator> type;
  };


  template <class Iterator>
  typename IteratorSecond<Iterator>::type iterator_second(const Iterator& it) {
    typedef typename IteratorSecond<Iterator>::type iterator;
    return iterator(it);
  }
  template <class Iterator>
  typename IteratorSecondMutable<Iterator>::type iterator_second(Iterator& it) {
    typedef typename IteratorSecond<Iterator>::type iterator;
    return iterator(it);
  }


  template <class Range>
  struct RangeFirstConst {
    typedef typename boost::range_const_iterator<Range>::type iterator;
    typedef typename IteratorFirst<iterator>::type iterator_first;
    typedef boost::iterator_range<iterator_first> type;
  };
  template <class Range>
  struct RangeFirstMutable {
    typedef typename boost::range_iterator<Range>::type iterator;
    typedef typename IteratorFirstMutable<iterator>::type iterator_first;
    typedef boost::iterator_range<iterator_first> type;
  };


  template <class Range>
  typename RangeFirstConst<Range>::type range_first(const Range& r) {
    typedef RangeFirstConst<Range> range_first_const;
    typedef typename range_first_const::type range;
    typedef typename range_first_const::iterator_first iterator;
    using boost::begin;
    using boost::end;
    return range(iterator(begin(r)), iterator(end(r)));
  }
  template <class Range>
  typename RangeFirstMutable<Range>::type range_first(Range& r) {
    typedef RangeFirstMutable<Range> range_first_mutable;
    typedef typename range_first_mutable::type range;
    typedef typename range_first_mutable::iterator_first iterator;
    using boost::begin;
    using boost::end;
    return range(iterator(begin(r)), iterator(end(r)));
  }


  template <class Range>
  struct RangeSecondConst {
    typedef typename boost::range_const_iterator<Range>::type iterator;
    typedef typename IteratorSecond<iterator>::type iterator_second;
    typedef boost::iterator_range<iterator_second> type;
  };
  template <class Range>
  struct RangeSecondMutable {
    typedef typename boost::range_iterator<Range>::type iterator;
    typedef typename IteratorSecondMutable<iterator>::type iterator_second;
    typedef boost::iterator_range<iterator_second> type;
  };


  template <class Range>
  typename RangeSecondConst<Range>::type range_second(const Range& r) {
    typedef RangeSecondConst<Range> range_second_const;
    typedef typename range_second_const::type range;
    typedef typename range_second_const::iterator_second iterator;
    using boost::begin;
    using boost::end;
    return range(iterator(begin(r)), iterator(end(r)));
  }
  template <class Range>
  typename RangeSecondConst<Range>::type range_second(Range& r) {
    typedef RangeSecondMutable<Range> range_second_mutable;
    typedef typename range_second_mutable::type range;
    typedef typename range_second_mutable::iterator_second iterator;
    using boost::begin;
    using boost::end;
    return range(iterator(begin(r)), iterator(end(r)));
  }


}

#endif

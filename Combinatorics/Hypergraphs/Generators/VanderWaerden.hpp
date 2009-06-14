// Oliver Kullmann, 15.5.2005 (Swansea)
/* Copyright 2005 - 2007, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKlib/Combinatorics/Hypergraphs/Generators/plans/VanderWaerden.hpp
  \brief %Tools for generating van der Waerden hypergraphs.
*/

#ifndef VANDERWAERDENHYPERGRAPH_77bQMQM_5993KKl
#define VANDERWAERDENHYPERGRAPH_77bQMQM_5993KKl

#include <iterator>
#include <cassert>

#include <OKlib/Concepts/Iterators.hpp>

namespace OKlib {
  
  /*!
    \class Iterator_arithmetic_progression
    \brief Iterator to iterate through the elements of an arithmetical progression.
    \todo Make a const random iterator out of it.
  */
  
  template <typename Integer>
  class Iterator_arithmetic_progression : public std::iterator<std::input_iterator_tag, Integer> {
    Integer current_value;
    Integer slope;
  public :
    typedef ::OKlib::Concepts::MultiPassInputIterator_tag concept_tag;
    Iterator_arithmetic_progression(const Integer start_value, const Integer slope) : current_value(start_value), slope(slope) {}
    Integer operator* () const { return current_value; }
    Iterator_arithmetic_progression& operator ++() {
      current_value += slope;
      return *this;
    }
    Iterator_arithmetic_progression operator ++(int) {
      Iterator_arithmetic_progression i(*this);
      operator ++();
      return i;
    }
  };
  
  template <typename Integer>
  inline bool operator ==(const Iterator_arithmetic_progression<Integer>& lhs, const Iterator_arithmetic_progression<Integer>& rhs) {
    return *lhs == *rhs;
  }
  template <typename Integer>
  inline bool operator !=(const Iterator_arithmetic_progression<Integer>& lhs, const Iterator_arithmetic_progression<Integer>& rhs) {
    return not (lhs == rhs);
  }
  
  // -----------------------------------------------------------------------------------------------------------------------------
  
  /*!
    \class Arithmetic_progression
    \brief An arithmetic progressions as a container.

    \todo Add the concept tag (a special case of a "const collection with begin, end and size; not default-constructible, but copyable, assignable, equality comparable").

    \todo See class IteratorHandling::Arithmetical_progression in
   General/IteratorHandling.hpp

  */
  
  template <typename Integer>
  class Arithmetic_progression {
    Integer start_, length, slope_;
    Integer finish;
  public :
    Arithmetic_progression(const Integer start, const Integer length, const Integer slope) : start_(start), length(length), slope_(slope), finish(start_ + length * slope_) {
      assert(length >= 0);
    }
    typedef Integer value_type;
    typedef Integer size_type;
    size_type size() const { return length; }
    typedef Iterator_arithmetic_progression<Integer> iterator;
    typedef iterator const_iterator;
    iterator begin() const { return iterator(start_, slope_); }
    iterator end() const { return iterator(finish, 0); }
    friend bool operator ==(const Arithmetic_progression& lhs, const Arithmetic_progression& rhs) {
      return lhs.start == rhs.start and lhs.length == rhs.length and lhs.slope == rhs.slope;
    }
    value_type start() const { return start_; }
    value_type slope() const { return slope_; }
  };
  
  // -----------------------------------------------------------------------------------------------------------------------------

}


#endif

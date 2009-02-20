// Oliver Kullmann, 2.2.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKlib/Programming/Utilities/TimeStamps/plans/general.hpp
  \brief Plans for the timestamp module


  \todo Update namespaces.


  \todo Write milestones.

  
  \todo First prototype
  <ul>
   <li>
   \code
#include <stdint.h> // later to be replaced by <cstdint>
template <typename UInt = uint_fast64_t, class OverflowPolicy = NoCheck> // later std::uint_fast64_t
class Timestamp {
  Uint counter;
public :
  typedef UInt size_type;
  typedef OverflowPolicy<Uint> overflow_policy_type;  
public :
  bool null() const { return counter == 0; }
  void reset() { counter = 0; }
  Timestamp& operator ++() {
    overflow_policy_type::check(++counter);
  }
  Timestamp& operator ++(int) {
    Timestamp tmp(*this);
    operator ++();
    return tmp;
  }

  operators <, ==, and the other four;
}
 
struct NoCheck {
  template <typename UInt>
  static void check(const UInt) {}
};
struct ThrowPolicy {
template <typename UInt>
  static void check(const UInt after_increment) {
    if (after_increment == 0)
      throw suitable exception;
  }
}
   \endcode
   </li>
   <li> The problem with the above approach is, that normally only
   one central timestamp-object with the ability for increment is needed,
   wile with the other objects just comparisons take place. </li>
   <li> One could use a non-polymorphic base class, which contains
   the counter and only provides the linear order, and the above class
   is derived from that class and adds the change-functionality. </li>
   <li> Inheritance then is better "protected" (this still allows
   for implicit conversion of the derived class into the base class), to
   indicate the special nature of inheritance here. </li>
   <li> so
   \code
template <typename UInt = uint_fast64_t, class OverflowPolicy = NoCheck>
class Timestamp {
  Uint counter;
public :
  Timestamp() : counter(0) {}
  friend bool operator ==(const Timestamp lhs, const Timestamp rhs) {
    return lhs == rhs;
  }
  friend bool operator <(const Timestamp lhs, const Timestamp rhs) {
    return lhs < rhs;
  }
}

// < != and <=, >, >=

template <typename UInt = uint_fast64_t, class OverflowPolicy = NoCheck>
class TimestampProvider : protected Timestamp<UInt, OverflowPolicy> {
public :
  typedef UInt size_type;
  typedef OverflowPolicy<Uint> overflow_policy_type;  
  bool null() const { return counter == 0; }
  void reset() { counter = 0; }
  TimestampProvider& operator ++() {
    overflow_policy_type::check(++counter);
  }
  TimestampProvider& operator ++(int) {
    TimestampProvider tmp(*this);
    operator ++();
    return tmp;
  }
}
   \endcode
  </ul>

*/

/*!
  \namespace OKlib::Utilities
  \brief Various utilities
*/

namespace OKlib {
  namespace Utilities {
  }
}  


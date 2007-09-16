// Oliver Kullmann, 6.11.2004 (Swansea)
/* Copyright 2004 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#ifndef ALGEBRATRAITS_ggznn159Y

#define ALGEBRATRAITS_ggznn159Y

#include <cassert>

namespace Algebra_Traits {

  template <typename Operator, typename Value>
  struct BinaryLinearCombination {

    typedef Operator operator_type;
    typedef Value value_type;
    const Operator x, y;
    const Value a, b, c;

    // TO IMPROVE: intelligent parameter passing
    BinaryLinearCombination(
			    const operator_type& x,
			    const operator_type& y,
			    const value_type& a,
			    const value_type& b) :
      x(x), y(y), a(a), b(b), c(x * a + y * b) {}

    BinaryLinearCombination(
			    const operator_type& x,
			    const operator_type& y,
			    const value_type& a,
			    const value_type& b,
			    const value_type& c) :
      x(x), y(y), a(a), b(b), c(c) {
      assert(c == x * a + y * b);
    }
  };

  template <typename Operator, typename Value>
  inline bool operator ==(const BinaryLinearCombination<Operator, Value>& lhs, const BinaryLinearCombination<Operator, Value>& rhs) {
    return lhs.x == rhs.x and lhs.y == rhs.y and lhs.a == rhs.a and lhs.b == rhs.b and lhs.c == rhs.c;
  }
  template <typename Operator, typename Value>
  inline bool operator !=(const BinaryLinearCombination<Operator, Value>& lhs, const BinaryLinearCombination<Operator, Value>& rhs) {
    return not (lhs == rhs);
  }

}

namespace Algebra_Traits {

  template <typename MultiplicativeGroupoid>
  struct MultiplicativeGroupoid_traits {
    typedef MultiplicativeGroupoid value_type;
    static value_type identity();
    static bool invertible(value_type);
    static value_type inverse(value_type);
  };

  template <>
  struct MultiplicativeGroupoid_traits<int> {
    typedef int value_type;
    static value_type identity() { return value_type(1); }
    static bool invertible(const value_type x) { return x == 1 or x == -1; }
    static value_type inverse (const value_type x) {
      assert(invertible(x));
      return x;
    }
  };

}


#endif

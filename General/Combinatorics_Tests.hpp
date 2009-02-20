// Oliver Kullmann, 22.8.2004 (Swansea)
/* Copyright 2004 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file General/Combinatorics_Tests.hpp
  \brief Tests for the elementary counting and enumeration tools.

  \todo When transferring to OKlib:
   - Use the new test system.
   - Complete the tests.

  \todo With C++09 stdint.h likely is to be replaced by cstdint.
*/

#ifndef COMBINATORICS_TESTS_JSJD6gac2v
#define COMBINATORICS_TESTS_JSJD6gac2v

#include <cassert>
#include <numeric>
#include <functional>

#include <stdint.h>

namespace Combinatorics_Tests {

  template <template <typename Int> class Factorial>
  struct test_factorial {
    test_factorial() {
#ifndef NDEBUG
      Factorial<unsigned int> f;
#endif
      for (unsigned int i = 0; i <= 12; ++i) {
        unsigned int prod = 1;
        for (unsigned int j = 2; j <= i; ++j)
          prod *= j;
        assert(f(i) == prod);
      } 
    }
  };
  
  template <template <typename Num, typename Int> class Descending_power>
  struct test_descending_power {
    test_descending_power() {
      // XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
    }
  };

  template <template <typename Int> class Binom_integer_over_integer>
  struct test_binom_integer_over_integer {
    test_binom_integer_over_integer() {
      {
        typedef int int_type;
#ifndef NDEBUG
        Binom_integer_over_integer<int_type> b;
#endif
        for (int_type i = -10; i <= 10; ++i) {
          for (int_type j = -10; j <= -1; ++j)
            assert(b(i, j) == 0);
          assert(b(i, 0) == 1);
          assert(b(i, 1) == i);
        }
        for (int_type i = 1; i <= 10; ++i) {
          assert(b(0, i) == 0);
          assert(b(i,i) == 1);
        }
        assert(b(3,2) == 3);
        assert(b(4,2) == 6);
        assert(b(43,10) == 1917334783);
        // XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
      }
      {
        typedef uint_fast64_t int_type;
#ifndef NDEBUG
        Binom_integer_over_integer<int_type> b;
        const int_type r(b(80,20));
#endif
        // b(80,20) = 3535316142212174320 =  2^4  5 7 11 13 23 31 37 61 67 71 73 79
        // In C++98 this number cannot be represented as a literal.
#ifndef NDEBUG
        unsigned int prime_factors[] = {16, 5, 7, 11, 13, 23, 31, 37, 61, 67, 71, 73, 79};
        const int number_factors = sizeof(prime_factors) / sizeof(unsigned int);
        const int_type ref(std::accumulate(prime_factors, prime_factors + number_factors, int_type(1), std::multiplies<int_type>()));
#endif
        assert(r == ref);
      }
    }
  };
    
}

#endif

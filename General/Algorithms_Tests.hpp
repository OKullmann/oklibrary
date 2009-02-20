// Oliver Kullmann, 11.9.2004 (Swansea)
/* Copyright 2004 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#ifndef ALGORITHMS_TESTS_hh47bbs44Rd

#define ALGORITHMS_TESTS_hh47bbs44Rd

#include <cassert>
#include <vector>

#include <OKlib/General/IteratorHandling.hpp>
#include <OKlib/General/Algebra_Models.hpp>

namespace Algorithms_Tests {

  template <template <typename Out, typename It> class median>
  struct test_median {
    test_median() {
      // XXXXXXXXXXXXXXXXXXXXXXXXXXX
    }
  };

  template <template <typename Out, typename It> class median>
  struct test_median_with_sorting {
    test_median_with_sorting() {
      {
        typedef double Out;
        typedef std::vector<Out> container;
        typedef container::iterator It;

        // sequence a + 0 * d, a + 1 * d, ..., a + m * d has median
        // a + m/2 * d for real a, d and m in {0,1,2,...}.

        const Out a_min = -10, a_max = +10;
        const Out d_min = -5, d_max = +5;
        const int m_min = 0, m_max = 100;

        for (Out a = a_min; a <= a_max; ++a)
          for (Out d = d_min; d <= d_max; ++d)
            for (int m = m_min; m <= m_max; ++m) {
              IteratorHandling::Arithmetical_progression<Out, int> ap(a, d, m);
              std::vector<Out> seq(ap.begin(), ap.end());
#ifndef NDEBUG
              median<Out, It> med;
#endif
              assert(med(seq.begin(), seq.end()) == a + (m * d) / 2);
            }
      }
      {
        typedef double Out;
        typedef double* It;
        typedef std::vector<double>::iterator It2;
        double a[] = {
          641,517,653,877,385,605,521,627,599,777,695,615,713,465,773,729,749,601,653,931,411,455,701,493,545,963,469,271,775,633,697,619,925,533,595,629,577,725,539,651,469,1119,733,783,549,579,885,389,525,683,707,557,483,413,951,589,1275,617,989,879,943,721,473,623,697,639,735,569,863,441,751,505,593,459,657,655,725,443,653,877,347,589,943,521,555,571,863,653,797,975,661,439,725,375,513,333,693,525,627,769,693,739,571,729,287,741,569,509,479,495,813,745,563,715,631,777,357,745,805,647,841,597,613,871,469,435,751,477
        };
        std::vector<double> a2(a, a+128);
#ifndef NDEBUG
        median<Out, It> med;
#endif
        assert(med(a, a+128) == 632);
#ifndef NDEBUG
        median<Out, It2> med2;
#endif
        assert(med2(a2.begin(), a2.end()) == 632);
      }
    }
  };

}

#include <OKlib/General/Numerics.hpp>

namespace Algorithms_Tests {

  // OK, 6.11.2004
  template <template <typename MultiplicativeMonoid, typename Int> class Power_natural>
  struct Test_power_natural {
    Test_power_natural() {
      {
        typedef int MultiplicativeMonoid;
        typedef int Int;
#ifndef NDEBUG
        Power_natural<MultiplicativeMonoid, Int> power;
#endif
	
        assert(power(0,0) == 1);
        assert(power(0,1) == 0);
        assert(power(1,0) == 1);

        const int max_basis = 7;
        const int max_exponent = 10;
        for (int b = 0; b <= max_basis; ++b)
          for (int e = 0; e <= max_exponent; ++e)
            assert(power(b,e) == Numerics::integer_exponentiation(b,e));
      }
      {
        typedef int Int;
        const Int modulus = 122;
        typedef Algebra::Z_mod_n<modulus, Int> Z_mod;
#ifndef NDEBUG
        Power_natural<Z_mod, Int> power;
#endif
        const Int max_exponent = 20;
        for (Int x = 0; x < modulus; ++x)
          for (Int e = 0; e <= max_exponent; ++e)
            assert(power(Z_mod(x),e) == Numerics::integer_exponentiation(Z_mod(x),e));
      }
      {
        typedef int Int;
        const Int modulus = 5 * 11;
        typedef Algebra::Z_mod_n<modulus, Int> Z_mod;
#ifndef NDEBUG
        Power_natural<Z_mod, Int> power;
        const Int e = 13, d = 37;
#endif
        for (Int x = 0; x < modulus; ++x)
          assert(power(power(Z_mod(x),e),d) == Z_mod(x));
      }
    }
  };

}

#endif

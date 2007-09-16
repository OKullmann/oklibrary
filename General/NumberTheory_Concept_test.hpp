// Oliver Kullmann, 6.11.2004 (Swansea)
/* Copyright 2004 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#ifndef NUMBERTHEORYCONCEPTTEST_kdk44acPo

#define NUMBERTHEORYCONCEPTTEST_kdk44acPo

#include <cassert>

namespace NumberTheory_Concept_test {

  // OK, 6.11.2004
  template <template <typename Int> class Gcd>
  struct Test_gcd {
    Test_gcd() {
      {
        // TO IMPROVE: The concept must specify, that if the inputs are integerrs >= 0, then the output also is non-negative.
        typedef int Int;
        Gcd<Int> gcd;
	
        assert(gcd(0,0) == 0);
        assert(gcd(1,0) == 1);
        assert(gcd(0,1) == 1);
	
        // TO IMPROVE: test levels
	
        const Int max_trivial_tests = 100;
        for (Int i = 1; i <= max_trivial_tests; ++i) {
          assert(gcd(i,i) == i);
          assert(gcd(i,0) == i);
          assert(gcd(0,i) == i);
          assert(gcd(i,1) == 1);
          assert(gcd(1,i) == 1);
        }
	
        const Int max_symmetry_tests = 100;
        for (Int i = 1; i <= max_symmetry_tests; ++i)
          for (Int j = i+1; j <= max_symmetry_tests; ++j)
            assert(gcd(i,j) == gcd(j,i));
	
        assert(gcd(10,3) == 1);
        assert(gcd(19,10) == 1);
        assert(gcd(55,10) == 5);
        assert(gcd(112,36) == 4);
        assert(gcd(312,262) == 2);
        assert(gcd(1200,360) == 120);
      }
    }
  };

  // OK, 6.11.2004
  template <typename Int, template <typename> class Gcd_extended, template <typename> class Gcd>
  struct Test_wrapper_gcd_extended {
    void operator() (const Int a, const Int b) const {
      typedef Gcd_extended<Int> Gcd_i_e;
      typedef Gcd<Int> Gcd_i;
      typedef typename Gcd_i_e::result_type result_type;

      Gcd_i_e gcd_e;
      Gcd_i gcd;

      const result_type r = gcd_e(a, b);
      assert(r.a == a);
      assert(r.b == b);
      assert(r.c == gcd(a,b));
      assert(r.x * a + r.y * b == r.c);
    }
  };

  // OK, 6.11.2004
  template <template <typename Int> class Gcd_extended, template <typename Int> class Gcd>
  struct Test_gcd_extended {
    Test_gcd_extended() {
      {
        typedef int Int;
        Test_wrapper_gcd_extended<Int, Gcd_extended, Gcd> gcd_test;
	
        gcd_test(0,0);
        gcd_test(0,1);
        gcd_test(1,0);

        const Int max_trivial_tests = 100;
        for (Int i = 1; i <= max_trivial_tests; ++i) {
          gcd_test(i,i);
          gcd_test(i,0);
          gcd_test(0,i);
          gcd_test(i,1);
          gcd_test(1,i);
        }

        const Int max_tests = 100;
        for (Int i = 1; i <= max_tests; ++i)
          for (Int j = i+1; j <= max_tests; ++j)
            gcd_test(i,j);
	
        gcd_test(10,3);
        gcd_test(19,10);
        gcd_test(55,10);
        gcd_test(112,36);
        gcd_test(312,262);
        gcd_test(1200,360);

      }
    }
  };

 // OK, 6.11.2004
  template <template <typename Int> class Euler_phi>
  struct Test_Euler_phi {
    Test_Euler_phi() {
      {
        typedef int Int;
        typedef Euler_phi<Int> Euler_phi_i;
#ifndef NDEBUG
        Euler_phi_i phi;
#endif
	
        assert(phi(0) == 0);
        assert(phi(1) == 1);
        assert(phi(2) == 1);
        assert(phi(3) == 2);
        assert(phi(4) == 2);
        assert(phi(5) == 4);
        assert(phi(6) == 2);
        assert(phi(7) == 6);
        assert(phi(8) == 4);
        assert(phi(9) == 6);
        assert(phi(10) == 4);
        assert(phi(32) == 16);
        assert(phi(40) == 16);
        assert(phi(55) == 40);
      }
    }
  };

}

#endif

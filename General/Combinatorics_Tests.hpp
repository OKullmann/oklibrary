// Oliver Kullmann, 22.8.2004

#ifndef COMBINATORICS_TESTS_JSJD6gac2v

#define COMBINATORICS_TESTS_JSJD6gac2v

#include <cassert>

namespace Combinatorics_Tests {

  template <template <typename Int> class Factorial>
  struct test_factorial {
    test_factorial() {
      Factorial<int> f;
      for (int i = 0; i <= 12; ++i) {
	int prod = 1;
	for (int j = 2; j <= i; ++j)
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
      Binom_integer_over_integer<int> b;
      for (int i = -10; i <= 10; ++i) {
	for (int j = -10; j <= -1; ++j)
	  assert(b(i, j) == 0);
	assert(b(i, 0) == 1);
	assert(b(i, 1) == i);
      }
      for (int i = 1; i <= 10; ++i) {
	assert(b(0, i) == 0);
	assert(b(i,i) == 1);
      }
      assert(b(3,2) == 3);
      assert(b(4,2) == 6);
      assert(b(43,10) == 1917334783);
      // XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
    }
  };
  
}

#endif

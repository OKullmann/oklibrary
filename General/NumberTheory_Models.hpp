// Oliver Kullmann, 6.11.2004 (Swansea)

#ifndef NUMBERTHEORYMODELS_66hdhsb2

#define NUMBERTHEORYMODELS_66hdhsb2

#include <functional>
#include <algorithm>

#include "Algebra_Traits.hpp"

namespace NumberTheory {

  // Euclidian algorithm

  // OK, 6.11.2004
  template <typename Int>
  struct Gcd : std::binary_function<Int, Int, Int> {
    Int operator() (Int a, Int b) const {
      while (b != Int(0)) {
	a %= b;
	using std::swap;
	swap(a,b);
      }
      return a;
    }
  };

  template <typename Int>
  inline Int gcd(const Int a, const Int b) {
    return Gcd<Int>()(a, b);
  }

  // Extended Euclidian algorithm

  // OK, 6.11.2004
  template <typename Int>
  struct Gcd_extended :
    std::binary_function<Int, Int,
    Algebra_Traits::BinaryLinearCombination<Int, Int> > {

    typedef typename std::binary_function<Int,Int,Algebra_Traits::BinaryLinearCombination<Int, Int> >::result_type result_type;

    result_type operator() (const Int a, const Int b) const {
      // TO IMPROVE: Using a fixed size vector class
      Int a0(a), a1(b);
      Int x0(Int(1)), y0(Int(0)), x1(Int(0)), y1(Int(1));
      while (a1 != Int(0)) {
	const Int div = a0 / a1;
	using std::swap;
	x0 = x0 - div * x1; y0 = y0 - div * y1;
	swap(x0, x1); swap(y0, y1);
	a0 -= div * a1;
	swap(a0, a1);
      }
      return result_type(x0, y0, a, b, a0);
    }
  };

  template <typename Int>
  inline Algebra_Traits::BinaryLinearCombination<Int, Int> gcd_extended(const Int a, const Int b) {
    return Gcd_extended<Int>()(a, b);
  }

  // Euler's phi function
  template <typename Int>
  struct Euler_phi_brute_force : std::unary_function<Int, Int> {
    Int operator() (const Int n) const {
      assert(n >= 0);
      if (n == Int(0)) return Int(0);
      Int counter(Int(0));
      for (Int i = 2; i < n; ++i)
	if (gcd(i, n) == Int(1)) ++counter;
      return counter + Int(1);
    }
  };
  template <typename Int>
  inline Int euler_phi_brute_force(const Int n) {
    return Euler_phi_brute_force<Int>()(n);
  }
}

#endif

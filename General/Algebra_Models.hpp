// Oliver Kullmann, 6.11.2004 (Swansea)

#ifndef ALGEBRAWAECHTER_jbbZ3y7

#define ALGEBRAWAECHTER_jbbZ3y7

#include <functional>
#include <cassert>
#include <ostream>

#include "NumberTheory_Models.hpp"

namespace Algebra {

  // OK, 7.11.2004
  template <typename Int>
  struct Normed_remainder : std::unary_function<Int, Int> {
    const Int n;
    Normed_remainder(const Int n) : n(n) {
      assert(n > 0);
    }
    Int operator() (Int x) const {
      if (x < 0) {
	x = -x; x %= n;
	if (x != 0)
	  x = n - x;
      }
      else
	x %= n;
      assert(x >= 0);
      assert(x < n);
      return x;
    }
  };
  template <typename Int>
  inline Int normed_remainder(const Int x, const Int n) {
    return (Normed_remainder<Int>(n))(x);
  }
  
  // OK, 6.11.2004
  template <unsigned long int modulus, typename Int>
  class Z_mod_n {
    // TO IMPROVE: Recognising moduli which are too large (at compile time).
    Int x;
    typedef NumberTheory::Gcd_extended<Int> Gcd_e;
  public :
    Z_mod_n() : x(Int(0)) {}
    explicit Z_mod_n(Int x) : x(normed_remainder(x, Int(modulus))) {}

    Z_mod_n& operator +=(Z_mod_n y) {
      assert(x >= 0);
      assert(x < Int(modulus));
      x += y.x;
      if (x >= Int(modulus))
	x -= Int(modulus);
      return *this;
    }
    Z_mod_n& operator *=(Z_mod_n y) {
      assert(x >= 0);
      assert(x < Int(modulus));
      x *= y.x;
      x %= Int(modulus);
      return *this;
    }
    Z_mod_n& operator -=(Z_mod_n y) {
      assert(x >= 0);
      assert(x < Int(modulus));
      x -= y.x;
      if (x < Int(0))
	x += Int(modulus);
      return *this;
    }
    bool invert() {
      assert(x >= 0);
      assert(x < Int(modulus));
      const typename Gcd_e::result_type gcd_extended = Gcd_e()(x, Int(modulus));
      if (gcd_extended.c != Int(1))
	return false;
      x = gcd_extended.x;
      if (x < Int(0))
	x += Int(modulus);
      return true;
    }
    Z_mod_n& operator /=(Z_mod_n y) {
      assert(x >= 0);
      assert(x < Int(modulus));
      const bool invertible = y.invert();
      assert(invertible);
      x *= y.x;
      x %= Int(modulus);
      return *this;
    }
    static const Z_mod_n null() {
      return Z_mod_n(Int(0));
    }
    static const Z_mod_n unit() {
      return Z_mod_n(Int(1));
    }

    friend inline bool operator ==(const Z_mod_n lhs, const Z_mod_n rhs) {
      return lhs.x == rhs.x;
    }
    
    friend inline std::ostream& operator <<(std::ostream& out, Z_mod_n<modulus, Int> x) {
      return out << x.x;
    }

  };

  template <unsigned long int modulus, typename Int>
  inline Z_mod_n<modulus, Int> operator +(const Z_mod_n<modulus, Int> a, const Z_mod_n<modulus, Int> b) {
    Z_mod_n<modulus, Int> r(a);
    r += b;
    return r;
  }
  template <unsigned long int modulus, typename Int>
  inline Z_mod_n<modulus, Int> operator *(const Z_mod_n<modulus, Int> a, const Z_mod_n<modulus, Int> b) {
    Z_mod_n<modulus, Int> r(a);
    r *= b;
    return r;
  }
  template <unsigned long int modulus, typename Int>
  inline Z_mod_n<modulus, Int> operator -(const Z_mod_n<modulus, Int> a, const Z_mod_n<modulus, Int> b) {
    Z_mod_n<modulus, Int> r(a);
    r -= b;
    return r;
  }
  template <unsigned long int modulus, typename Int>
  inline Z_mod_n<modulus, Int> operator -(const Z_mod_n<modulus, Int> a) {
    Z_mod_n<modulus, Int> r(Int(0));
    r -= a;
    return r;
  }
  template <unsigned long int modulus, typename Int>
  inline Z_mod_n<modulus, Int> operator /(const Z_mod_n<modulus, Int> a, const Z_mod_n<modulus, Int> b) {
    assert((Z_mod_n<modulus, Int>(b).invert()));
    Z_mod_n<modulus, Int> r(a);
    r /= b;
    return r;
  }
  
  template <unsigned long int modulus, typename Int>
  inline bool operator !=(const Z_mod_n<modulus, Int> a, const Z_mod_n<modulus, Int> b) {
    return not(a == b);
  }


}

namespace Algebra_Traits {

  template <unsigned long int modulus, typename Int>
  struct MultiplicativeGroupoid_traits<Algebra::Z_mod_n<modulus, Int> > {
    typedef Algebra::Z_mod_n<modulus, Int> value_type;
    static value_type identity() { return value_type(1); }
    static bool invertible(value_type x) { return x.invert(); }
    static value_type inverse (value_type x) {
      const bool invertible = x.invert();
      assert(invertible);
      return x;
    }
  };
}

#endif

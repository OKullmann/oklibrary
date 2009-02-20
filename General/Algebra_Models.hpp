// Oliver Kullmann, 6.11.2004 (Swansea)
/* Copyright 2004 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Algebra_Models.hpp
  \brief Deprecated (that is, to be "upgraded" and replaced).
  Contains Z_mod_n<modules, Int> and Zmodn<Int> for modular arithmetic
  (in the first case the modulus is fixed at compile time), and RSA<Int>
  for RSA-encryption and decryption (given the private key).
  \todo These classes should go into a number theory module.
  \todo Using a static member for the modulus in Zmodn is very crude.
*/

#ifndef ALGEBRAWAECHTER_jbbZ3y7

#define ALGEBRAWAECHTER_jbbZ3y7

#include <functional>
#include <cassert>
#include <ostream>

#include <OKlib/General/NumberTheory_Models.hpp>
#include <OKlib/General/Algorithms.hpp>

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
  
  // #####################################################

  template <unsigned long int modulus, typename Int>
  class Z_mod_n {
    // TO IMPROVE: Generalising "unsigned long int".
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

// #####################################################

namespace Algebra {
  
  template <typename Int = long int>
  class Zmodn {
    // TO IMPROVE: Needs proper design.
    Int x;
    typedef NumberTheory::Gcd_extended<Int> Gcd_e;
  public :
    static Int modulus;
    Zmodn() : x(Int(0)) {}
    explicit Zmodn(Int x) : x(normed_remainder(x, Int(modulus))) {}
    Int rem() const { return x; }
    
    Zmodn& operator +=(Zmodn y) {
      assert(x >= 0);
      assert(x < Int(modulus));
      x += y.x;
      if (x >= Int(modulus))
        x -= Int(modulus);
      return *this;
    }
    Zmodn& operator *=(Zmodn y) {
      assert(x >= 0);
      assert(x < Int(modulus));
      x *= y.x;
      x %= Int(modulus);
      return *this;
    }
    Zmodn& operator -=(Zmodn y) {
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
    Zmodn& operator /=(Zmodn y) {
      assert(x >= 0);
      assert(x < Int(modulus));
      assert(y.invert());
      x *= y.x;
      x %= Int(modulus);
      return *this;
    }
    static const Zmodn null() {
      return Zmodn(Int(0));
    }
    static const Zmodn unit() {
      return Zmodn(Int(1));
    }

    friend inline bool operator ==(const Zmodn lhs, const Zmodn rhs) {
      return lhs.x == rhs.x;
    }
    
    friend inline std::ostream& operator <<(std::ostream& out, Zmodn<Int> x) {
      return out << x.x;
    }

  };

   template <typename Int>
   Int Zmodn<Int>::modulus(1);

  template <typename Int>
  inline Zmodn<Int> operator +(const Zmodn<Int> a, const Zmodn<Int> b) {
    Zmodn<Int> r(a);
    r += b;
    return r;
  }
  template <typename Int>
  inline Zmodn<Int> operator *(const Zmodn<Int> a, const Zmodn<Int> b) {
    Zmodn<Int> r(a);
    r *= b;
    return r;
  }
  template <typename Int>
  inline Zmodn<Int> operator -(const Zmodn<Int> a, const Zmodn<Int> b) {
    Zmodn<Int> r(a);
    r -= b;
    return r;
  }
  template <typename Int>
  inline Zmodn<Int> operator -(const Zmodn<Int> a) {
    Zmodn<Int> r(Int(0));
    r -= a;
    return r;
  }
  template <typename Int>
  inline Zmodn<Int> operator /(const Zmodn<Int> a, const Zmodn<Int> b) {
    assert((Zmodn<Int>(b).invert()));
    Zmodn<Int> r(a);
    r /= b;
    return r;
  }
  
  template <typename Int>
  inline bool operator !=(const Zmodn<Int> a, const Zmodn<Int> b) {
    return not(a == b);
  }


}

namespace Algebra_Traits {

  template <typename Int>
  struct MultiplicativeGroupoid_traits<Algebra::Zmodn<Int> > {
    typedef Algebra::Zmodn<Int> value_type;
    static value_type identity() { return value_type(1); }
    static bool invertible(value_type x) { return x.invert(); }
    static value_type inverse (value_type x) {
      const bool invertible = x.invert();
      assert(invertible);
      return x;
    }
  };
}

// #####################################################

namespace Algebra {

  template <typename Int = long int>
  struct RSA {

    typedef Int int_type;
    const int_type p, q;
    const int_type n, N;

    typedef Zmodn<Int> mod_type; // We need *two* different types or objects here (for the 2 moduli) !
    bool dummy;

    const mod_type e, d;

    RSA(const int_type p_, const int_type q_, const int_type e_) : p(p_), q(q_), n(p * q), N((p-1) * (q-1)), dummy(set_temp_modulus()), e(e_), d(mod_type(1) / e) {
      // assert p, q are prime numbers
      mod_type::modulus = n;
    }

    mod_type encrypt(const int_type plaintext) const {
      return Algorithms::power_natural(mod_type(plaintext), e.rem());
    }
    mod_type decrypt(const int_type plaintext) const {
      return Algorithms::power_natural(mod_type(plaintext), d.rem());
    }

  private :

    bool set_temp_modulus() {
      mod_type::modulus = N;
      return true;
    }
  };

}

#endif

// Oliver Kullmann, 6.11.2004 (Swansea)
/* Copyright 2004 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file General/NumberTheory_Models.hpp
  \brief Deprecated (to be replaced). Contains functors performing the Euclidian algorithm,
  and a brute-force computation of Euler's phi-function.
*/

#ifndef NUMBERTHEORYMODELS_66hdhsb2
#define NUMBERTHEORYMODELS_66hdhsb2

#include <functional>
#include <algorithm>
#include <ostream>
#include <string>
#include <cassert>

#include <OKlib/General/Algebra_Traits.hpp>

namespace NumberTheory {

  // ######################################################

  // Euclidian algorithm

  template <typename Int>
  struct GcdVisitor_empty {
    void operator() (const Int&) {}
    void finished() {}
  };
  template <typename Int>
  struct GcdVisitor_output {
    std::ostream& out;
    const std::string sep;
    GcdVisitor_output(std::ostream& out, const std::string& sep = ",") : out(out), sep(sep) {}
    void operator() (const Int& x) {
      out << x << sep;
    }
    void finished() {
      out << 0;
    }
  };

  template <typename Int, class Visitor = GcdVisitor_empty<Int> >
  struct Gcd : std::binary_function<Int, Int, Int> {
    typedef Int int_type;
    Visitor vis;
    Gcd() {}
    Gcd(Visitor vis) : vis(vis) {}
    int_type operator() (int_type a, int_type b) {
      vis(a);
      while (b != int_type(0)) {
        vis(b);
        a %= b;
        using std::swap;
        swap(a,b);
      }
      vis.finished();
      return a;
    }
  };

  template <typename Int>
  inline Int gcd(const Int a, const Int b) {
    return Gcd<Int>()(a, b);
  }

  // ######################

  // Extended Euclidian algorithm

  template <typename Int>
  struct GcdExtVisitor_empty {
    void operator() (const Int&) {}
    void coefficients(const Int&, const Int&) {}
    void last_coefficients(const Int&, const Int&) {}
    void finished() {}
  };
  template <typename Int>
  struct GcdExtVisitor_output {
    std::ostream& out_euc_seq;
    std::ostream& out_euc_ext_seq;
    const std::string sep;
    GcdExtVisitor_output(std::ostream& out1, std::ostream& out2, const std::string& sep = ",") : out_euc_seq(out1), out_euc_ext_seq(out2), sep(sep) {}
    void operator() (const Int& a) {
      out_euc_seq << a << sep;
    }
    void coefficients(const Int& x, const Int& y) {
      out_euc_ext_seq << "(" << x << "," << y << ")" << sep;
    }
    void last_coefficients(const Int& x, const Int& y) {
      out_euc_ext_seq << "(" << x << "," << y << ")";
    }
    void finished() {
      out_euc_seq << 0;
    }
  };

  template <typename Int, class Visitor = GcdExtVisitor_empty<Int> >
  struct Gcd_extended : std::binary_function<Int, Int, Algebra_Traits::BinaryLinearCombination<Int, Int> > {

    typedef Int int_type;
    typedef typename std::binary_function<int_type,int_type,Algebra_Traits::BinaryLinearCombination<int_type, int_type> >::result_type result_type;

    Visitor vis;

    Gcd_extended() {}
    Gcd_extended(Visitor vis) : vis(vis) {}

    result_type operator() (const int_type a, const int_type b) {
      // TO IMPROVE: Using a fixed size vector class
      int_type a0(a), a1(b);
      vis(a0);
      int_type x0(int_type(1)), y0(int_type(0)), x1(int_type(0)), y1(int_type(1));
      vis.coefficients(x0, y0);
      assert(a0 == x0 * a + y0 * b);
      assert(a1 == x1 * a + y1 * b);
      while (a1 != int_type(0)) {
        vis(a1);
        const int_type div = a0 / a1;
        using std::swap;
        x0 = x0 - div * x1; y0 = y0 - div * y1;
        swap(x0, x1); swap(y0, y1);
        vis.coefficients(x0, y0);
        a0 -= div * a1;
        swap(a0, a1);
        assert(a0 == x0 * a + y0 * b);
        assert(a1 == x1 * a + y1 * b);
      }
      vis.last_coefficients(x1, y1);
      vis.finished();
      return result_type(x0, y0, a, b, a0);
    }
  };

  template <typename Int>
  inline Algebra_Traits::BinaryLinearCombination<Int, Int> gcd_extended(const Int a, const Int b) {
    return Gcd_extended<Int>()(a, b);
  }

  // ######################################################

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

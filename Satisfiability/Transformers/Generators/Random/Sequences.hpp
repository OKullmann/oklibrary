// Oliver Kullmann, 8.11.2020 (Swansea)
/* Copyright 2020, 2023 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Random sequences

   struct ExpSeq :

     The idea is to create equally spread random points for each order of
     magnitude, e.g., 10 points in 1-10, 10 points in 10-100, 10 points
     in 100-1000 etc. Each point is close to the center, starting with
     a radius 0.01, which grows by a factor of 10 by each order.
     E is the number of orders. By parameter N we get N variations of the
     random points around the center (above is N=1).
     Via S we get a fine subdivision of each order (above S=1; increasing
     S by 1 means basically one order finer).
     Finally by the boolean parameter ge1, when set to false (above is
     true), we obtain the reciprocal values.
     In more details:

     ExpSeq(E,S,N,ge1) is a range of values, which via the static function
     ExpSeq::transform<FloatingPoint> is translated to floating-point numbers:

      - E >= 1 is the number of order of magnitudes -- for each order
        9 points are created around 2, ..., 10 in this magnitude:

     For example, using Environment::out_line_T:
     out_line_T(std::cout, ExpSeq(1,1,1,true), ExpSeq::translate<double>);
     yields
2.00653 3.00095 4.00554 5.00397 5.99944 6.99846 8.00064 8.99009 9.9936
     while
     out_line_T(std::cout, ExpSeq(3,1,1,true), ExpSeq::translate<double>);
     yields (with line-breaks for better readability):
1.99729 2.9903 4.0051 4.9997 6.0084 7.00473 7.99501 9.00803 9.99089
19.9632 29.963 40.0521 50.0746 59.9637 70.0041 79.9809 90.0321 100.05
200.415 299.013 400.026 500.159 600.17 699.714 799.823 899.051 999.102

     "Around" here means with the radius 0.01 for the first order, 0.1 for
     the second order, 1 for the third, and so on.

     The (pseudo-random) sequences are determined by the four parameters.

     The third parameter N creates N random variations instead of one, around
     the (imaginary) central point, e.g.
     out_line_T(std::cout, ExpSeq(1,1,2,true), ExpSeq::translate<double>);
     yields
2.00476 1.99479 2.99816 3.00743 3.99464 3.99643 5.00278 5.00846
5.99912 5.99182 7.00731 7.00288 8.00943 8.00198 9.00211 8.99021 9.9994 10.0001

     With the second parameter S the order is more finely subdivided, so that
     for example for S=2 we get 99 points instead of 9, namely (the centres)
       1.1, 1.2, ..., 10.9
     (so we cover now also 1.X and 10.X),while for S=3 we get
     999 points
       1.01, ..., 10.99
     and so on. With each increase of S the radius shrinks by a factor of 10.

     Setting eq1 to false yields reciprocal values:
     out_line_T(std::cout, ExpSeq(1,1,1,false), ExpSeq::translate<double>);
     yields
0.499499 0.333022 0.249866 0.200113 0.166686 0.143046 0.125067 0.111178
0.100044
     We note that by using S >= 2 we start closer to 1, and end below 0.1,
     e.g.
     out_line_T(std::cout, ExpSeq(1,2,1,false), ExpSeq::translate<double>);
     yields (99 values)
0.908686 0.833398 0.769321 0.713893 0.667088 0.625171 0.588468 0.555735
...
0.0970951 0.0961462 0.0952381 0.0943427 0.0934519 0.0925859 0.0917494

*/

#ifndef SEQUENCES_dZakTZB00i
#define SEQUENCES_dZakTZB00i

#include <ostream>
#include <iterator>
#include <algorithm>
#include <vector>

#include <cassert>
#include <cstdint>
#include <cmath>
#include <cstddef>

// Guaranteed to be included:
#include "Numbers.hpp"

#include "Distributions.hpp"

namespace RandGen {

  /*
    Convenience wrappers

      - UniformVectors V(a,b, seeds) :
          calls to V(N) create a vector of length N with "random" elements
          from the closed interval [a,b], where if a>b, then these values are
          swapped.
  */

  class UniformVectors {
  public :
    const gen_uint_t a, b, n; // a, b here mean the closed interval, except
                              // when a=0, b=randgen_max.

    UniformVectors(const gen_uint_t a0, const gen_uint_t b0,
                   const vec_eseed_t& v) noexcept :
      a(std::min(a0,b0)), b(std::max(a0,b0)), n((b-a)+1),
      U(n==0?1:n, n==0?vec_seed_t{}:transform(v), n==0?0:a) {}


    std::vector<gen_uint_t> operator ()(const gen_uint_t N) {
      if (N == 0) return {};
      if (a == b) return std::vector<gen_uint_t>(N,a);
      std::vector<gen_uint_t> res; res.reserve(N);
      if (n == 0)
        for (gen_uint_t i = 0; i < N; ++i) res.push_back(U.g());
      else
        for (gen_uint_t i = 0; i < N; ++i) res.push_back(U());
      return res;
    }

  private :
    UniformRangeS U;
  };


  /*
    ExpSeq(E,S,N,true/false)

    provides a sequence (as a range) of random values, which can be
    transformed into a sequence of
      E * (10^S - 1) * N
    many floating-point numbers, ranging roughly from 1 to 10^E.
  */

  struct ExpSeq {
    typedef gen_uint_t size_t;
    typedef std::uint8_t small_size_t;

    const size_t E;
    const small_size_t S;
    const size_t s = std::pow(size_t(10), S);
    constexpr static small_size_t max_S = 19;
    STATIC_ASSERT(std::pow(size_t(10), max_S) < randgen_max);
    const size_t N;
    const bool ge1;

    constexpr ExpSeq() = delete;
    constexpr ExpSeq(const size_t E, const size_t S0, const size_t N,
                     const bool ge1) noexcept :
    E(E), S(S0), N(N), ge1(ge1) {
      assert(E >= 1);
      assert(S0 >= 1);
      assert(S0 <= max_S);
      assert(N >= 1);
    }

    constexpr size_t main_size() const noexcept { return E * (s-1); }
    constexpr size_t size() const noexcept { return main_size() * N; }
    constexpr bool empty() const noexcept { return false; }

    struct iterator_value {
      const size_t i, s, j, val;
      const bool ge1;
    };
    friend constexpr bool operator ==(const iterator_value& l,
                                      const iterator_value& r) {
      return l.i==r.i && l.s==r.s && l.j==r.j && l.val==r.val && l.ge1==r.ge1;
    }
    friend std::ostream& operator <<(std::ostream& out,
                                     const iterator_value& v) {
      out << "(" << v.i << "," << v.s << "," << v.j << "," << v.val
          << "," << v.ge1 << ")";
      return out;
    }
    template <typename FP>
    static FP translate(const iterator_value& v) {
      const FP p = FP(v.val) / FP(randgen_max);
      const FP base = std::pow(FP(10), v.i);
      const FP quot = base / FP(v.s);
      const FP factor = 10 * quot;
      const FP main_point = base + v.j * factor;
      const FP radius = quot / 10;
      const FP point = main_point + radius * (2 * p - 1);
      return v.ge1 ? point : 1 / point;
    }

  private :

    struct iterator_t {

      typedef std::input_iterator_tag iterator_concept; // for explicitness
      typedef std::ptrdiff_t difference_type; // formally required
      typedef iterator_value value_type;

      constexpr iterator_t() noexcept {} // not "default", since that would
      // yield an incomplete type

      iterator_t(const size_t E,
                 const small_size_t S, const size_t s,
                 const size_t N, const bool ge1) noexcept :
      E(E), S(S), s(s), N(N), ge1(ge1),
      g(new RandGen_t(transform({E,S,N,ge1}))), val((*g)()) {}

      iterator_t(const iterator_t& it) noexcept :
      E(it.E), S(it.S), s(it.s), N(it.N), ge1(it.ge1), i(it.i), j(it.j),
      g(it.g ? new RandGen_t(*it.g):nullptr), val(it.val) {}

      iterator_t& operator =(const iterator_t& it) noexcept {
        E = it.E; S = it.S; s = it.s; N = it.N; ge1 = it.ge1;
        i = it.i; j = it.j;
        delete g;
        g = it.g ? new RandGen_t(*it.g) : nullptr;
        val = it.val;
        return *this;
      }

      ~iterator_t() noexcept { delete g; }

      iterator_value operator *() const noexcept {
        return {i,s,j,val,ge1};
      }
      iterator_t& operator ++() noexcept {
        assert(g);
        if (++k == N) {
          k = 0;
          if (++j == s) {
            j=1;
            if (++i == E) {
              delete g;
              g = nullptr;
              return *this;
            }
          }
        }
        val = (*g)();
        return *this;
      }
      iterator_t operator ++(int) noexcept {
        iterator_t temp(*this); operator ++(); return temp;
      }

      friend bool operator ==(const iterator_t& lhs,
                              const iterator_t& rhs) noexcept {
        return (lhs.g == nullptr and rhs.g == nullptr) or
          (lhs.E==rhs.E&&lhs.S==rhs.S&&lhs.N==rhs.N&&lhs.ge1==rhs.ge1 and
           lhs.i == rhs.i and lhs.j == rhs.j and lhs.g == rhs.g);
      }
      friend bool operator !=(const iterator_t& lhs,
                              const iterator_t& rhs) noexcept {
        return not (lhs == rhs);
      }

    private :

      size_t E = 0;
      small_size_t S = 0;
      size_t s = 0;
      size_t N = 0;
      bool ge1 = 0;

      size_t i = 0; // i < E
      size_t j = 1; // j < s
      size_t k = 0; // k < N

      RandGen_t* g = nullptr;
      gen_uint_t val = 0;

    };
    static_assert(std::input_iterator<iterator_t>);

  public :

    iterator_t begin() const noexcept { return iterator_t(E,S,s,N,ge1); }
    iterator_t end() const noexcept { return iterator_t(); }

  };

}

#endif

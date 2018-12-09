// Oliver Kullmann, 19.8.2018 (Swansea)
/* Copyright 2018 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* An attempt to find (simple) recursions which simulate the node-counts
   of backtracking-algorithms.
*/

/* TODOS

1. Add parallelisation

    - Similar to Backtracking.hpp.
    - The optimisation of the compiler reduces the computation of
        CountLeaves<BaseS>()()
      to a non-branching very fast computation, exploiting that lc = rc.

2. Implement ln(tau(a,b))    DONE -- but numeric details need to be tested
                             carefully

   Let ltau(a,b) := ln(tau(a,b)), for a, b > 0. (If a or b is infinite, then we
   set ltau(a,b) := 0.)
   The characteristic equation of ltau(a,b) is

     ltau(a,b) is the x > 0 such that
       exp(-x*a) + exp(-x*b) = 1.

   To apply Newton-Raphson, for parameters a,b the map
     x -> f(x) := exp(-x*a) + exp(-x*b) - 1
   needs to be differentiated:
     f'(x) = -a*exp(-x*a) - b*exp(-x*b).

   The iteration is
     x_{n+1} = x_n - f(x_n) / f'(x_n).

   The initial guess, as for function Branching_tau::tau(a,b) in
     Solvers/TawSolver/tawSolver.cpp,
   can use x_0 := ln(4^(1/(a+b))) = ln(4) / (a+b).

   For the stopping-criterion, since we start above the solution, and
   monotonically decrease, we might just try x_n == x_{n+1}.

3. Implement ltau_1eq(a,b), computing the x with   DONE -- but numerics need
                                                   careful checking
     ltau(a,b) = ltau(1,x).

       ltau(1,x) = y > 0 iff
         exp(-y) + exp(-y*x) = 1 <=>
         exp(-y*x) = 1 - exp(-y) <=>
         -y * x = ln(1 - exp(-y)) <=>
         x = - ln(1 - exp(-y)) / y.

    Thus ltau_1eq(a,b) = - ln(1 - exp(-tau(a,b))) / tau(a,b).

*/

#ifndef RECURSION_ZESDN9fd4B
#define RECURSION_ZESDN9fd4B

#include <array>
#include <algorithm>
#include <limits>

#include <cassert>
#include <cmath>

#include "ChessBoard.hpp"

namespace Recursion {

  typedef long double floating_t;
  using limitfloat = std::numeric_limits<floating_t>;

  constexpr floating_t log(const floating_t x)noexcept{ return std::log(x); }
  constexpr floating_t exp(const floating_t x)noexcept{ return std::exp(x); }
  constexpr floating_t expm1(const floating_t x)noexcept{return std::expm1(x);}
  constexpr floating_t sqrt(const floating_t x)noexcept{return std::sqrt(x);}

  constexpr bool equal(const floating_t a, const floating_t b) noexcept {
    assert(a >= b);
    return a == b;
  }
  constexpr floating_t ltau(floating_t a, floating_t b) noexcept {
    assert(a > 0);
    assert(b > 0);
    if (a > b) {const auto t=a; a=b; b=t;}
    if (std::isinf(b)) return 0;
    floating_t x0 = log(4) / (a+b);
    while (true) {
      const floating_t Am1 = expm1(-a*x0), B = exp(-b*x0);
      const floating_t fx0 = Am1 + B;
      assert(fx0 >= 0);
      if (fx0 == 0) return x0;
      const floating_t x1 = x0 + fx0/(a*(Am1+1) + b*B);
      if (equal(x1,x0)) return x0;
      x0 = x1;
    }
  }
  static_assert(ltau(1,1) == log(2));
  static_assert(ltau(2,2) == log(2)/2);
  static_assert(ltau(3,3) == log(2)/3); // ltau(a,a) = log(2)/a
  static_assert(exp(ltau(1,2)) == (1+sqrt(5))/2);
  static_assert(exp(ltau(2,4)) == sqrt((1+sqrt(5))/2));
  static_assert(ltau(3,7) > ltau(3,7+10*limitfloat::epsilon()));
  static_assert(ltau(3*5,7*5) == ltau(3,7)/5);

  static_assert(limitfloat::has_infinity);
  constexpr floating_t pinfinity = limitfloat::infinity();
  static_assert(pinfinity > 0);
  static_assert(std::isinf(pinfinity));
  static_assert(ltau(pinfinity, 1) == 0);

  constexpr floating_t ltau_1eq(const floating_t a, const floating_t b) noexcept {
    if (a == 1) return b;
    if (b == 1) return a;
    const floating_t lt = ltau(a,b);
    if (lt == 0) return pinfinity;
    else return - log(-expm1(-lt)) / lt;
  }
  static_assert(ltau_1eq(1,1) == 1);
  static_assert(ltau(1, ltau_1eq(2,2)) == ltau(2,2));
  static_assert(ltau(ltau_1eq(7,3), 1) == ltau(3,7));
  static_assert(ltau_1eq(1,pinfinity) == pinfinity);


  // The known exact values for N-Queens counting:
  constexpr ChessBoard::coord_t max_N_exact = 27;
  constexpr std::array<ChessBoard::Var_uint, max_N_exact+1> exact_values{1,
    1ULL,0ULL,0ULL,2ULL,10ULL,4ULL,40ULL,92ULL,352ULL,724ULL,
    2680ULL,14200ULL,73712ULL,365596ULL,2279184ULL,14772512ULL,95815104ULL,666090624ULL,4968057848ULL,39029188884ULL,
    314666222712ULL,2691008701644ULL,24233937684440ULL,227514171973736ULL,2207893435808352ULL,22317699616364044ULL,234907967154122528ULL};
  constexpr auto exact_value(const ChessBoard::coord_t N) noexcept {
    return (N > max_N_exact) ? 0 : exact_values[N];
  }

  // The "strong conjecture", according to https://oeis.org/A000170 :
  constexpr floating_t base_strong_conjecture = 2.444638;
  constexpr auto strong_conjecture(const ChessBoard::coord_t N) noexcept {
    floating_t res = 1;
    for (ChessBoard::coord_t i = 0; i < N;)
      res *= ++i / base_strong_conjecture;
    return res;
  }

  // Simulating the branching tree:
  template <class Branching>
  struct CountLeaves {
    typedef Branching branching_t;
    using Var_uint = ChessBoard::Var_uint;
    using Count_t = ChessBoard::Count_t;

    const Var_uint N;
    const branching_t B;
    const Var_uint n0 = N*N;
    CountLeaves(const ChessBoard::coord_t N) noexcept : N(N), B{N} {}
    Count_t operator()() const noexcept { return operator()(n0); }
    Count_t operator()(const Var_uint n) const noexcept {
      if (n == 0) return 1;
      const Var_uint l = std::min(n,B.left(n));
      assert(l >= 1);
      const Count_t lc = operator()(n-l);
      const Var_uint r = std::min(n,B.right(n));
      assert(r >= 1);
      const Count_t rc = operator()(n-r);
      return lc + rc;
    }
  };


  // Class for Branching:
  struct Base {
    using Var_uint = ChessBoard::Var_uint;
    const Var_uint N;
    Base(const ChessBoard::coord_t N) noexcept : N(N) {}
  };
  // For symmetric branching:
  struct BaseS : Base {
    using Base::Base;
    virtual Var_uint right(const Var_uint n) const noexcept final {
      return left(n);
    }
    virtual ~BaseS() = default;
  private :
    virtual Var_uint left(const Var_uint) const noexcept = 0;
  };


  // Result too big: N^(1/2) left, N right:
  struct NTwo : Base {
    using Base::Base;
    const Var_uint dl = std::round(std::sqrt(N));
    const Var_uint dr = N;
    Var_uint left(const Var_uint) const noexcept { return dl; }
    Var_uint right(const Var_uint) const noexcept { return dr; }
  };

  // Result too small: N left, N right, yields 2^N:
  struct NN : BaseS {
    using BaseS::BaseS;
    const Var_uint d = N;
    Var_uint left(const Var_uint) const noexcept override { return d; }
  };

  constexpr floating_t factorial(const ChessBoard::coord_t N) noexcept {
    return (N == 0) ? 1 : N * factorial(N-1);
  }
  constexpr floating_t pi = std::acos(floating_t(-1));
  // Aproximating N!, using n! ~ n^(n+1/2)/e^n * (2*pi)^(1/2):
  constexpr floating_t Stirling_factor = log(2*pi)/2;
  struct Nfact : BaseS {
    using BaseS::BaseS;
    const floating_t d0 =
      N*N*log(2) / (log(N)*(N+0.5) - N + Stirling_factor);
    const Var_uint d = std::round(d0);
    Var_uint left(const Var_uint) const noexcept override { return d; }
  };
  // Approximating strong_conjecture, symmetrically, and via Stirling:
  struct Nstrconj : BaseS {
    using BaseS::BaseS;
    const floating_t d0 =
      N*N*log(2) / (log(N)*(N+0.5) - N * (1 + log(base_strong_conjecture))
         + Stirling_factor);
    const Var_uint d = std::round(d0);
    Var_uint left(const Var_uint) const noexcept override { return d; }
  };
  // Now asymmetrically:
  struct NAstrconj : Base {
    using Base::Base;
    const floating_t ds = Nstrconj(N).d0;
    const floating_t d0 = ltau_1eq(ds,ds);
    constexpr static Var_uint dl = 1;
    const Var_uint dr = std::round(d0);
    Var_uint left(const Var_uint) const noexcept { return dl; }
    Var_uint right(const Var_uint) const noexcept { return dr; }
  };

}

#endif

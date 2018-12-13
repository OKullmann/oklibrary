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

  /* The floating-point type: */

  typedef long double floating_t;
  using limitfloat = std::numeric_limits<floating_t>;
  static_assert(limitfloat::is_iec559);

  constexpr floating_t pinfinity = limitfloat::infinity();
  static_assert(pinfinity > 0);
  static_assert(std::isinf(pinfinity));

  static_assert(1 - limitfloat::epsilon() < 1);
  static_assert(1 + limitfloat::epsilon() > 1);

  inline constexpr floating_t log(const floating_t x) noexcept {
    return std::log(x);
  }
  static_assert(log(1) == 0);

  inline constexpr floating_t exp(const floating_t x) noexcept {
    return std::exp(x);
  }
  static_assert(exp(0) == 1);
  static_assert(log(exp(1)) == 1);
  constexpr floating_t euler = exp(1);
  static_assert(log(euler) == 1);

  inline constexpr floating_t expm1(const floating_t x) noexcept {
    return std::expm1(x);
  }
  static_assert(expm1(0) == 0);
  static_assert(abs(expm1(1) - (euler - 1)) < limitfloat::epsilon());

  inline constexpr floating_t sqrt(const floating_t x) noexcept {
    return std::sqrt(x);
  }
  static_assert(sqrt(1) == 1);

  inline constexpr floating_t abs(const floating_t x) noexcept {
    return std::abs(x);
  }
  static_assert(abs(log(sqrt(2)) - log(2)/2) < limitfloat::epsilon());

  inline constexpr floating_t pow(const floating_t x, const floating_t y) noexcept {
    return std::pow(x,y);
  }
  static_assert(pow(0,0) == 1);
  static_assert(pow(2,16) == 65536);

  inline constexpr floating_t round(const floating_t x) noexcept {
    return std::round(x);
  }
  static_assert(round(0.5) == 1);
  static_assert(round(1.5) == 2);
  static_assert(round(2.5) == 3);

  // floating_t fully includes Var_uint:
  constexpr ChessBoard::Var_uint P264m1 = std::numeric_limits<ChessBoard::Var_uint>::max();
  static_assert(P264m1 + 1 == 0);
  static_assert(ChessBoard::Var_uint(floating_t(P264m1)) == P264m1);
  constexpr floating_t P264 = pow(2,64);
  static_assert(sqrt(P264) == pow(2,32));
  static_assert(sqrt(sqrt(P264)) == pow(2,16));
  // Exactly the integers in the interval [-P264, +P264] are exactly represented by floating_t:
  static_assert(P264-1 == floating_t(P264m1));
  static_assert(-P264 == -floating_t(P264m1) - 1);
  static_assert(P264+1 == P264);
  static_assert(-P264-1 == -P264);


  /* Computations related to the factorial function: */

  inline constexpr floating_t factorial(const ChessBoard::coord_t N) noexcept {
    floating_t prod = 1;
    for (ChessBoard::coord_t i = 1; i < N; ++i) prod *= i+1;
    return prod;
  }
  static_assert(factorial(20) == 2432902008176640000ULL);
  inline constexpr floating_t lfactorial(const ChessBoard::Var_uint N) noexcept {
    floating_t sum = 0;
    for (ChessBoard::Var_uint i = 1; i < N; ++i) sum += log(i+1);
    return sum;
  }
  static_assert(lfactorial(0) == 0);
  static_assert(lfactorial(1) == 0);
  static_assert(lfactorial(2) == log(2));
  static_assert(exp(lfactorial(10)) == factorial(10));
  static_assert(round(exp(lfactorial(19))) == factorial(19));

  // The Stirling approximation:
  constexpr floating_t pi = std::acos(floating_t(-1));
  static_assert(std::cos(pi) == -1);
  static_assert(abs(std::sin(pi)) < limitfloat::epsilon());
  inline constexpr floating_t Sfactorial(const ChessBoard::coord_t N) noexcept {
    return sqrt(2*pi*N) * pow(N/euler,N);
  }
  static_assert(Sfactorial(0) == 0);
  static_assert(Sfactorial(1) == sqrt(2*pi)/euler);
  static_assert(Sfactorial(1754) < factorial(1754));
  static_assert(factorial(1754) / Sfactorial(1754) < 1.00005);
  constexpr floating_t lStirling_factor = log(2*pi)/2;
  inline constexpr floating_t lSfactorial(const ChessBoard::Var_uint N) noexcept {
    assert(N != 0);
    return log(N) * (N + 0.5) - N + lStirling_factor;
  }
  static_assert(lSfactorial(1) == lStirling_factor - 1);
  static_assert(abs(lSfactorial(10) - log(Sfactorial(10))) < limitfloat::epsilon());


  /* Tau computations */

  inline constexpr bool equal(const floating_t a, const floating_t b) noexcept {
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
  static_assert(ltau(3,3) == log(2)/3);
  static_assert(ltau(1000,1000) == log(2)/1000); // ltau(a,a) = log(2)/a
  static_assert(exp(ltau(1,2)) == (1+sqrt(5))/2);
  static_assert(exp(ltau(2,4)) == sqrt((1+sqrt(5))/2));
  static_assert(ltau(3,7) > ltau(3,7+10*limitfloat::epsilon()));
  static_assert(ltau(3*5,7*5) == ltau(3,7)/5);
  static_assert(ltau(23,57) == 0.018551927277904456577L);
  static_assert(ltau(0.1,0.23) == 4.451086045963786618L);
  static_assert(ltau(0.1,123) == 0.044112256194439923384L);
  static_assert(ltau(0.123,54321) == 0.00019576547107916477533L);
  static_assert(ltau(0.02345,0.00543) == 56.65900358501618499L);
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
  static_assert(ltau(1, ltau_1eq(23,57)) == ltau(23,57));
  static_assert(ltau_1eq(1,pinfinity) == pinfinity);


  // The known exact values for N-Queens counting:
  constexpr ChessBoard::coord_t max_N_exact = 27;
  constexpr auto exact_value(const ChessBoard::coord_t N) noexcept {
    constexpr std::array<ChessBoard::Var_uint, max_N_exact+1> exact_values{1,
      1ULL,0ULL,0ULL,2ULL,10ULL,4ULL,40ULL,92ULL,352ULL,724ULL,
      2680ULL,14200ULL,73712ULL,365596ULL,2279184ULL,14772512ULL,95815104ULL,666090624ULL,4968057848ULL,39029188884ULL,
      314666222712ULL,2691008701644ULL,24233937684440ULL,227514171973736ULL,2207893435808352ULL,22317699616364044ULL,234907967154122528ULL};
    return (N > max_N_exact) ? 0 : exact_values[N];
  }

  // The "strong conjecture", according to https://oeis.org/A000170 :
  constexpr floating_t base_strong_conjecture = 2.444638;
  constexpr floating_t lbase_strong_conjecture = log(base_strong_conjecture);
  inline constexpr floating_t strong_conjecture(const ChessBoard::coord_t N) noexcept {
    floating_t res = 1;
    for (ChessBoard::coord_t i = 0; i < N;)
      res *= ++i / base_strong_conjecture;
    return res;
  }
  static_assert(strong_conjecture(0) == 1);
  static_assert(strong_conjecture(1) == 1/base_strong_conjecture);
  inline constexpr floating_t lstrong_conjecture(const ChessBoard::Var_uint N) noexcept {
    return lfactorial(N) - N*lbase_strong_conjecture;
  }
  static_assert(exp(lstrong_conjecture(2)) == strong_conjecture(2));


  // Simulating the branching tree:
  class BaseS;
  template <class Branching>
  struct CountLeaves {
    typedef Branching branching_t;
    using Var_uint = ChessBoard::Var_uint;

    const Var_uint N;
    const branching_t B;
    const Var_uint n0 = N*N;
    CountLeaves(const ChessBoard::coord_t N) noexcept : N(N), B{N} {}
    floating_t operator()() const noexcept { return operator()(n0); }
    floating_t operator()(const floating_t n) const noexcept {
      if (n <= 0) return 1;
      const auto l = B.left(n);
      assert(l > 0);
      const auto lc = operator()(n-l);
      if constexpr (std::is_base_of_v<BaseS, Branching>) return 2 * lc;
      const auto r = B.right(n);
      assert(r > 0);
      const auto rc = operator()(n-r);
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
    virtual floating_t right(const floating_t n) const noexcept final {
      return left(n);
    }
    virtual ~BaseS() = default;
  private :
    virtual floating_t left(floating_t) const noexcept = 0;
  };


  // Result too big: N^(1/2) left, N right:
  struct NTwo : Base {
    using Base::Base;
    const floating_t dl = sqrt(N);
    const floating_t dr = N;
    floating_t left(floating_t) const noexcept { return dl; }
    floating_t right(floating_t) const noexcept { return dr; }
  };

  // Result too small: N left, N right, yields 2^N:
  struct NN : BaseS {
    using BaseS::BaseS;
    const floating_t d = N;
    floating_t left(floating_t) const noexcept override { return d; }
  };

  // Aproximating N!, using Stirling's approximation:
  struct Nfact : BaseS {
    using BaseS::BaseS;
    const floating_t d0 = N*N*log(2) / lSfactorial(N);
    floating_t left(floating_t) const noexcept override { return d0; }
  };
  // Approximating strong_conjecture, symmetrically, and via Stirling:
  struct Nstrconj : BaseS {
    using BaseS::BaseS;
    const floating_t d0 =
      N*N*log(2) / (lSfactorial(N) - N * lbase_strong_conjecture);
    floating_t left(floating_t) const noexcept override { return d0; }
  };
  // Now asymmetrically:
  struct NAstrconj : Base {
    using Base::Base;
    const floating_t ds = Nstrconj(N).d0;
    const floating_t d0 = ltau_1eq(ds,ds);
    constexpr static floating_t dl = 1;
    const floating_t dr = d0;
    floating_t left(floating_t) const noexcept { return dl; }
    floating_t right(floating_t) const noexcept { return dr; }
  };

}

#endif

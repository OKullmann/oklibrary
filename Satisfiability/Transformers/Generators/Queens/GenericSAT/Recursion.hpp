// Oliver Kullmann, 19.8.2018 (Swansea)
/* Copyright 2018, 2019 Oliver Kullmann
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

2. Analyse ltau(a,b) = ln(tau(a,b))

   Let ltau(a,b) := ln(tau(a,b)), for a, b > 0. (If a or b is infinite, then we
   set ltau(a,b) := 0.)
   The characteristic equation of ltau(a,b) is

     ltau(a,b) is the x > 0 such that
       exp(-x*a) + exp(-x*b) = 1.

   To apply Newton-Raphson, for parameters a,b the map
     x -> f(x) := exp(-x*a) + exp(-x*b) - 1
   is to be differentiated:
     f'(x) = -a*exp(-x*a) - b*exp(-x*b).

   The iteration is
     x_{n+1} = x_n - f(x_n) / f'(x_n).

   The initial guess, as for function Branching_tau::tau(a,b) in
     Solvers/TawSolver/tawSolver.cpp,
   uses x_0 := ln(4^(1/(a+b))) = ln(4) / (a+b).

   (a) Are there cases where the current implementation does not terminate?
   (b) Are there cases where one of the asserts triggers (of course, not
       considering the two asserts for the arguments)?
   (c) The computation of fpx0 = a*Am1 + a + b*B could also be formulated as
       fpx0 = a*(Am1 + 1) + b*B, however the current form seems numerically
       better -- is this true? One would assume that the form with "+1"
       is slightly faster.
   (d) How many interations are used? Where is the maximum reached, and
       how big is it?

3. Analyse ltau_1eq(a,b), computing the x with
     ltau(a,b) = ltau(1,x).

       ltau(1,x) = y > 0 iff
         exp(-y) + exp(-y*x) = 1 <=>
         exp(-y*x) = 1 - exp(-y) <=>
         -y * x = ln(1 - exp(-y)) <=>
         x = - ln(1 - exp(-y)) / y.

    Thus ltau_1eq(a,b) = - ln(1 - exp(-tau(a,b))) / tau(a,b).

    (a) At least the above deduction should be transferred somewhere.
    (b) Example for big numerical errors?

4. Improve organisation

   (a) Perhaps there should be a dedicated module (file) for general
       floating-point tools.
   (b) It seems that long double is indeed a fundamental type, since it fully
       contains 64-bit integer arithmetic. We should have helper classes
       to use this as a fundamental counting type (for solutions).
   (c) Also tau-functionality better goes to its own module.
   (d) Factorial functions and relatives likely go to the module with fp-tools.

*/

#ifndef RECURSION_ZESDN9fd4B
#define RECURSION_ZESDN9fd4B

#include <array>
#include <algorithm>
#include <limits>
#include <utility>

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

  inline constexpr floating_t ltau(floating_t a, floating_t b) noexcept {
    assert(a > 0);
    assert(b > 0);
    if (a > b) {const auto t=a; a=b; b=t;}
    if (std::isinf(b)) return 0;
    floating_t x0 = log(4) / (a+b);
    while (true) {
      const floating_t Am1 = expm1(-a*x0), B = exp(-b*x0);
      const floating_t fx0 = Am1 + B;
      if (fx0 <= 0) return x0;
      const floating_t fpx0 = a*Am1 + a + b*B;
      assert(fpx0 > 0);
      const floating_t x1 = x0 + fx0/fpx0;
      assert(x1 >= x0);
      if (x1 == x0) return x0;
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
  static_assert(ltau(21,23) == 0.031529279361734392134L);
  static_assert(ltau(1,limitfloat::max()) > 0);
  static_assert(ltau(pinfinity, 1) == 0);
  static_assert(ltau(pinfinity,pinfinity) == 0);

  typedef std::pair<floating_t,floating_t> floatpair_t;
  // The induced probability distribution of length 2, via their logarithms
  // and as std::pair:
  inline constexpr floatpair_t lprobtau(const floating_t a, const floating_t b) noexcept {
    assert(a > 0);
    assert(b > 0);
    assert(not std::isinf(a) and not std::isinf(b));
    const auto t = ltau(a,b);
    assert(t > 0);
    return {-a * t, -b * t};
  }
  static_assert(lprobtau(1,1) == floatpair_t(-log(2),-log(2)));
  static_assert(lprobtau(1000000000L,1000000000L) == floatpair_t(-log(2),-log(2)));
  static_assert(lprobtau(10e-9L,10e-9L) == floatpair_t(-log(2),-log(2)));
  static_assert(exp(lprobtau(22,24.7).first) + exp(lprobtau(22,24.7).second) == 1);

  // Solving ltau(1,a) = lt:
  inline constexpr floating_t ltau21a(const floating_t lt) noexcept {
    assert(lt >= 0);
    if (lt == 0) return pinfinity;
    else return - log(-expm1(-lt)) / lt;
  }
  static_assert(ltau21a(log(2)) == 1);
  // Solving ltau(1, ltau_1eq(a,b)) = ltau(a,b):
  inline constexpr floating_t ltau_1eq(const floating_t a, const floating_t b) noexcept {
    if (a == 1) return b;
    if (b == 1) return a;
    return ltau21a(ltau(a,b));
  }
  static_assert(ltau_1eq(1,1) == 1);
  static_assert(ltau(1, ltau_1eq(2,2)) == ltau(2,2));
  static_assert(ltau(ltau_1eq(7,3), 1) == ltau(3,7));
  static_assert(ltau(1, ltau_1eq(23,57)) == ltau(23,57));
  static_assert(ltau_1eq(1,pinfinity) == pinfinity);
  // Solving ltau(a,a) = lt:
  inline constexpr floating_t ltau2aa(const floating_t lt) noexcept {
    assert(lt > 0);
    return log(2) / lt;
  }
  static_assert(ltau2aa(log(2)) == 1);
  // Solving ltau(tau_mean(a,b), tau_mean(a,b)) = ltau(a,b); this acts also
  // as a generalised mean (see Handbook article):
  inline constexpr floating_t tau_mean(const floating_t a, const floating_t b) noexcept {
    if (a == b) return a;
    return ltau2aa(ltau(a,b));
  }
  static_assert(tau_mean(1,1) == 1);
  static_assert(tau_mean(21,23) > 21);
  static_assert(tau_mean(21,23) < 23);
  static_assert(ltau(tau_mean(21,23), tau_mean(21,23)) == ltau(21,23));
  static_assert(ltau(tau_mean(0.01,0.002), tau_mean(0.01,0.002)) == ltau(0.01,0.002));


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
    const Var_uint N2 = N*N;
    constexpr Base(const ChessBoard::coord_t N) noexcept : N(N) {}
  };
  static_assert(Base(10).N == 10);
  static_assert(Base(10).N2 == 100);

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
  // For asymmetric branching:
  struct BaseA : Base {
    using Base::Base;
    static constexpr floating_t dl = 1;
    virtual floating_t left(const floating_t) const noexcept final {
      return dl;
    }
    virtual ~BaseA() = default;
  };


  // Result too big: N^(1/2) left, N right:
  struct NTwo : Base {
    using Base::Base;
    const floating_t dl = sqrt(N);
    const floating_t dr = N;
    floating_t left(floating_t) const noexcept { return dl; }
    floating_t right(floating_t) const noexcept { return dr; }
  };
  static_assert(NTwo(100).dl == 10);

  // Result too small: N left, N right, yields 2^N:
  struct NN : BaseS {
    using BaseS::BaseS;
    const floating_t d = N;
    floating_t left(floating_t) const noexcept override { return d; }
  };


  // Given the measure at the root and the log of the number of leaves, computing ltau:
  inline constexpr floating_t ltau_for_tree(const floating_t measure, const floating_t lnlvs) noexcept {
    assert(lnlvs >= 0);
    return lnlvs / measure;
  }
  // Computing symmetric d:
  inline constexpr floating_t sd_for_tree(const floating_t measure, const floating_t lnlvs) noexcept {
    return ltau2aa(ltau_for_tree(measure, lnlvs));
  }
  // Computing asymmetric d:
  inline constexpr floating_t ad_for_tree(const floating_t measure, const floating_t lnlvs) noexcept {
    return ltau21a(ltau_for_tree(measure, lnlvs));
  }


  // Aproximating N!, using Stirling's approximation:
  struct Sfact : BaseS {
    using BaseS::BaseS;
    const floating_t lt = ltau_for_tree(N2, lSfactorial(N));
    const floating_t d0 = ltau2aa(lt);
    floating_t left(floating_t) const noexcept override { return d0; }
  };

  // Approximating strong_conjecture, symmetrically, and via Stirling:
  struct Nstrconj : BaseS {
    using BaseS::BaseS;
    const floating_t lt = ltau_for_tree(N2, lSfactorial(N) - N * lbase_strong_conjecture);
    const floating_t d0 = ltau2aa(lt);
    floating_t left(floating_t) const noexcept override { return d0; }
  };
  // Now asymmetrically:
  struct NAstrconj : BaseA {
    using BaseA::BaseA;
    const floating_t lt = Nstrconj(N).lt;
    const floating_t d0 = ltau21a(lt);
    const floating_t dr = d0;
    floating_t right(floating_t) const noexcept { return dr; }
  };

}

#endif

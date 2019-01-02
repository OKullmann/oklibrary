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
       floating-point tools. DONE
   (b) It seems that long double is indeed a fundamental type, since it fully
       contains 64-bit integer arithmetic. We should have helper classes
       to use this as a fundamental counting type (for solutions).
   (c) Also tau-functionality better goes to its own module. DONE
   (d) Factorial functions and relatives likely go to the module with fp-tools.
       DONE
*/

#ifndef RECURSION_ZESDN9fd4B
#define RECURSION_ZESDN9fd4B

#include <array>
#include <algorithm>
#include <limits>
#include <utility>

#include <cassert>

#include "ChessBoard.hpp"
#include "FloatingPoint.hpp"
#include "BranchingTuples.hpp"

namespace Recursion {

  namespace FP = FloatingPoint;
  namespace BT = BranchingTuples;

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
  constexpr FP::floating_t base_strong_conjecture = 2.444638;
  constexpr FP::floating_t lbase_strong_conjecture = FP::log(base_strong_conjecture);
  inline constexpr FP::floating_t strong_conjecture(const ChessBoard::coord_t N) noexcept {
    FP::floating_t res = 1;
    for (ChessBoard::coord_t i = 0; i < N;)
      res *= ++i / base_strong_conjecture;
    return res;
  }
  static_assert(strong_conjecture(0) == 1);
  static_assert(strong_conjecture(1) == 1/base_strong_conjecture);
  inline constexpr FP::floating_t lstrong_conjecture(const ChessBoard::Var_uint N) noexcept {
    return FP::lfactorial(N) - N*lbase_strong_conjecture;
  }
  static_assert(FP::exp(lstrong_conjecture(2)) == strong_conjecture(2));


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
    FP::floating_t operator()() const noexcept { return operator()(n0); }
    FP::floating_t operator()(const FP::floating_t n) const noexcept {
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
    virtual FP::floating_t right(const FP::floating_t n) const noexcept final {
      return left(n);
    }
    virtual ~BaseS() = default;
  private :
    virtual FP::floating_t left(FP::floating_t) const noexcept = 0;
  };
  // For asymmetric branching:
  struct BaseA : Base {
    using Base::Base;
    static constexpr FP::floating_t dl = 1;
    virtual FP::floating_t left(const FP::floating_t) const noexcept final {
      return dl;
    }
    virtual ~BaseA() = default;
  };


  // Result too big: N^(1/2) left, N right:
  struct NTwo : Base {
    using Base::Base;
    const FP::floating_t dl = FP::sqrt(N);
    const FP::floating_t dr = N;
    FP::floating_t left(FP::floating_t) const noexcept { return dl; }
    FP::floating_t right(FP::floating_t) const noexcept { return dr; }
  };
  static_assert(NTwo(100).dl == 10);

  // Result too small: N left, N right, yields 2^N:
  struct NN : BaseS {
    using BaseS::BaseS;
    const FP::floating_t d = N;
    FP::floating_t left(FP::floating_t) const noexcept override { return d; }
  };


  // Given the measure at the root and the log of the number of leaves, computing ltau:
  inline constexpr FP::floating_t ltau_for_tree(const FP::floating_t measure, const FP::floating_t lnlvs) noexcept {
    assert(lnlvs >= 0);
    return lnlvs / measure;
  }
  // Computing symmetric d:
  inline constexpr FP::floating_t sd_for_tree(const FP::floating_t measure, const FP::floating_t lnlvs) noexcept {
    return BT::ltau2aa(ltau_for_tree(measure, lnlvs));
  }
  // Computing asymmetric d:
  inline constexpr FP::floating_t ad_for_tree(const FP::floating_t measure, const FP::floating_t lnlvs) noexcept {
    return BT::ltau21a(ltau_for_tree(measure, lnlvs));
  }


  // Aproximating N!, using Stirling's approximation:
  struct Sfact : BaseS {
    using BaseS::BaseS;
    const FP::floating_t lt = ltau_for_tree(N2, FP::lSfactorial(N));
    const FP::floating_t d0 = BT::ltau2aa(lt);
    FP::floating_t left(FP::floating_t) const noexcept override { return d0; }
  };

  // Approximating strong_conjecture, symmetrically, and via Stirling:
  struct Nstrconj : BaseS {
    using BaseS::BaseS;
    const FP::floating_t lt = ltau_for_tree(N2, FP::lSfactorial(N) - N * lbase_strong_conjecture);
    const FP::floating_t d0 = BT::ltau2aa(lt);
    FP::floating_t left(FP::floating_t) const noexcept override { return d0; }
  };
  // Now asymmetrically:
  struct NAstrconj : BaseA {
    using BaseA::BaseA;
    const FP::floating_t lt = Nstrconj(N).lt;
    const FP::floating_t d0 = BT::ltau21a(lt);
    const FP::floating_t dr = d0;
    FP::floating_t right(FP::floating_t) const noexcept { return dr; }
  };

}

#endif

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

2. Implement ln(tau(a,b))

3. Implement tau_1equaliser(a,b), computing the x with
     ln(tau(a,b)) = ln(tau(1,x)).

*/

#ifndef RECURSION_ZESDN9fd4B
#define RECURSION_ZESDN9fd4B

#include <array>
#include <algorithm>

#include <cassert>
#include <cmath>

#include "ChessBoard.hpp"

namespace Recursion {

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
  typedef long double floating_t;
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


  // Result too big: N left, N^(1/2) right:
  struct NTwo : Base {
    using Base::Base;
    const Var_uint dl = N;
    const Var_uint dr = std::round(std::sqrt(N));
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
  constexpr floating_t log(const floating_t x) noexcept { return std::log(x); }
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
  // Approximating strong_conjecture:
  struct Nstrconj : BaseS {
    using BaseS::BaseS;
    const floating_t d0 =
      N*N*log(2) / (log(N)*(N+0.5) - N * (1 + log(base_strong_conjecture))
         + Stirling_factor);
    const Var_uint d = std::round(d0);
    Var_uint left(const Var_uint) const noexcept override { return d; }
  };

}

#endif

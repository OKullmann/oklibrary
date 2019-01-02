// Oliver Kullmann, 2.1.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* The tau-function and relatives

TODOS:

1. Analyse ltau(a,b) = ln(tau(a,b))

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

2. Analyse ltau_1eq(a,b), computing the x with
     ltau(a,b) = ltau(1,x).

       ltau(1,x) = y > 0 iff
         exp(-y) + exp(-y*x) = 1 <=>
         exp(-y*x) = 1 - exp(-y) <=>
         -y * x = ln(1 - exp(-y)) <=>
         x = - ln(1 - exp(-y)) / y.

    Thus ltau_1eq(a,b) = - ln(1 - exp(-tau(a,b))) / tau(a,b).

    (a) At least the above deduction should be transferred somewhere.
    (b) Example for big numerical errors?

*/

#ifndef BRANCHINGTUPLES_tb6aPElFBN
#define BRANCHINGTUPLES_tb6aPElFBN

#include <cassert>

#include "FloatingPoint.hpp"

namespace BranchingTuples {

  namespace FP = FloatingPoint;

  inline constexpr FP::floating_t ltau(FP::floating_t a, FP::floating_t b) noexcept {
    assert(a > 0);
    assert(b > 0);
    if (a > b) {const auto t=a; a=b; b=t;}
    if (FP::isinf(b)) return 0;
    FP::floating_t x0 = FP::log(4) / (a+b);
    while (true) {
      const FP::floating_t Am1 = FP::expm1(-a*x0), B = FP::exp(-b*x0);
      const FP::floating_t fx0 = Am1 + B;
      if (fx0 <= 0) return x0;
      const FP::floating_t fpx0 = a*Am1 + a + b*B;
      assert(fpx0 > 0);
      const FP::floating_t x1 = x0 + fx0/fpx0;
      assert(x1 >= x0);
      if (x1 == x0) return x0;
      x0 = x1;
    }
  }
  static_assert(ltau(1,1) == FP::log(2));
  static_assert(ltau(2,2) == FP::log(2)/2);
  static_assert(ltau(3,3) == FP::log(2)/3);
  static_assert(ltau(1000,1000) == FP::log(2)/1000); // ltau(a,a) = FP::log(2)/a
  static_assert(FP::exp(ltau(1,2)) == (1+FP::sqrt(5))/2);
  static_assert(FP::exp(ltau(2,4)) == FP::sqrt((1+FP::sqrt(5))/2));
  static_assert(ltau(3,7) > ltau(3,7+10*FP::epsilon));
  static_assert(ltau(3*5,7*5) == ltau(3,7)/5);
  static_assert(ltau(23,57) == 0.018551927277904456577L);
  static_assert(ltau(0.1,0.23) == 4.451086045963786618L);
  static_assert(ltau(0.1,123) == 0.044112256194439923384L);
  static_assert(ltau(0.123,54321) == 0.00019576547107916477533L);
  static_assert(ltau(0.02345,0.00543) == 56.65900358501618499L);
  static_assert(ltau(21,23) == 0.031529279361734392134L);
  static_assert(ltau(1,FP::limitfloat::max()) > 0);
  static_assert(ltau(FP::pinfinity, 1) == 0);
  static_assert(ltau(FP::pinfinity,FP::pinfinity) == 0);

  typedef std::pair<FP::floating_t,FP::floating_t> floatpair_t;
  // The induced probability distribution of length 2, via their logarithms
  // and as std::pair:
  inline constexpr floatpair_t lprobtau(const FP::floating_t a, const FP::floating_t b) noexcept {
    assert(a > 0);
    assert(b > 0);
    assert(not FP::isinf(a) and not FP::isinf(b));
    const auto t = ltau(a,b);
    assert(t > 0);
    return {-a * t, -b * t};
  }
  static_assert(lprobtau(1,1) == floatpair_t(-FP::log(2),-FP::log(2)));
  static_assert(lprobtau(1000000000L,1000000000L) == floatpair_t(-FP::log(2),-FP::log(2)));
  static_assert(lprobtau(10e-9L,10e-9L) == floatpair_t(-FP::log(2),-FP::log(2)));
  static_assert(FP::exp(lprobtau(22,24.7).first) + FP::exp(lprobtau(22,24.7).second) == 1);

  // Solving ltau(1,a) = lt:
  inline constexpr FP::floating_t ltau21a(const FP::floating_t lt) noexcept {
    assert(lt >= 0);
    if (lt == 0) return FP::pinfinity;
    else return - FP::log(-FP::expm1(-lt)) / lt;
  }
  static_assert(ltau21a(FP::log(2)) == 1);
  // Solving ltau(1, ltau_1eq(a,b)) = ltau(a,b):
  inline constexpr FP::floating_t ltau_1eq(const FP::floating_t a, const FP::floating_t b) noexcept {
    if (a == 1) return b;
    if (b == 1) return a;
    return ltau21a(ltau(a,b));
  }
  static_assert(ltau_1eq(1,1) == 1);
  static_assert(ltau(1, ltau_1eq(2,2)) == ltau(2,2));
  static_assert(ltau(ltau_1eq(7,3), 1) == ltau(3,7));
  static_assert(ltau(1, ltau_1eq(23,57)) == ltau(23,57));
  static_assert(ltau_1eq(1,FP::pinfinity) == FP::pinfinity);
  // Solving ltau(a,a) = lt:
  inline constexpr FP::floating_t ltau2aa(const FP::floating_t lt) noexcept {
    assert(lt > 0);
    return FP::log(2) / lt;
  }
  static_assert(ltau2aa(FP::log(2)) == 1);
  // Solving ltau(tau_mean(a,b), tau_mean(a,b)) = ltau(a,b); this acts also
  // as a generalised mean (see Handbook article):
  inline constexpr FP::floating_t tau_mean(const FP::floating_t a, const FP::floating_t b) noexcept {
    if (a == b) return a;
    return ltau2aa(ltau(a,b));
  }
  static_assert(tau_mean(1,1) == 1);
  static_assert(tau_mean(21,23) > 21);
  static_assert(tau_mean(21,23) < 23);
  static_assert(ltau(tau_mean(21,23), tau_mean(21,23)) == ltau(21,23));
  static_assert(ltau(tau_mean(0.01,0.002), tau_mean(0.01,0.002)) == ltau(0.01,0.002));

}

#endif

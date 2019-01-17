// Oliver Kullmann, 2.1.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* The tau-function and relatives

   Tools for handling ln(tau(a,b)):

    - lower bound ltau_Wlb(a,b) for ltau(a,b)
    - upper bound ltau_Wub(a,b) for ltau(a,b)
        also in double-version, called ltau_Wub_d
      (in both cases "W" reminds of the Lambert-W-function)
    - improved lower bound ltau_Wublb (and internal version ltau_Wublbu)
        the internal version also in double, called ltau_Wublbu_d
    - ltau(a,b)
    - lprobtau(a,b)
    - ltau21a, ltau1eq
    - ltau2aa, ltau_mean

   Compare with

    - the Maxima implementations at
      OKlib/ComputerAlgebra/Satisfiability/Lisp/BranchingTuples/Basic.mac
    - the implementation in the tawSolver at
      OKlib/Satisfiability/Solvers/TawSolver/tawSolver.cpp
    - the general plans at
      OKlib/Satisfiability/Heuristics/Projections/plans/TauMethod.hpp
    - the investigations in
      OKlib/Satisfiability/Transformers/Generators/Queens/GenericSAT/ExploreBts.cpp

TODOS:

1. Analyse ltau_1eq(a,b), computing the x with
     ltau(a,b) = ltau(1,x).

       ltau(1,x) = y > 0 iff
         exp(-y) + exp(-y*x) = 1 <=>
         exp(-y*x) = 1 - exp(-y) <=>
         -y * x = ln(1 - exp(-y)) <=>
         x = - ln(1 - exp(-y)) / y.

    Thus ltau_1eq(a,b) = - ln(1 - exp(-tau(a,b))) / tau(a,b).

    (a) At least the above deduction should be transferred somewhere.
    (b) Example for big numerical errors?

2. Lower bounds

   Prove that ltau_Wublb(a,b) > ltau_Wlb(a,b) for a # b.

*/

#ifndef BRANCHINGTUPLES_tb6aPElFBN
#define BRANCHINGTUPLES_tb6aPElFBN

#include <cassert>

#include "FloatingPoint.hpp"

namespace BranchingTuples {

  namespace FP = FloatingPoint;

  /* The lower bound
       ltau(a,b) >= W(b/a) / b,
     using FP::lambertW0_lb (set adapted=false for uncorrected computation).

     This was obtained at
     https://mathoverflow.net/questions/320584/a-certain-generalisation-of-the-golden-ratio

     Derivation:
     W(x) for x >= 0 is the unique y >= 0 such that y*exp(y) = x,
     i.e., W(x) * exp(W(X)) = x. W(0) = 0.
     So we have exp(-W(x)) = W(x)/x. If follows:
       exp(-W(a)/a) + exp(-a*W(x)/a) = exp(-W(a)/a) + exp(-W(x)) =
       exp(-W(a)/a) + W(a)/a >= -W(a)/a+1 + W(a)/a = 1
     (using exp(x) >= x+1 for all x), and thus
       W(a)/a <= ltau(1,a) for all a > 0.
     We obtain
       ltau(a,b) = 1/a ltau(1,b/a) >= 1/a (W(b/a)/(b/a)) = W(b/a) / b.
  */
  constexpr FP::float80 tauWlb_eq = 2.89146460534737291831L;
  constexpr FP::float80 l_tauWlb_eq = FP::log(tauWlb_eq);
  inline constexpr FP::float80 ltau_Wlb(const FP::float80 a, const FP::float80 b, const bool adapted=true) noexcept {
    assert(a > 0);
    assert(b > a);
    const FP::float80 l = FP::log(b) - FP::log(a);
    if (adapted and l < l_tauWlb_eq) return FP::log(4) / (a+b);
    assert(l >= 1);
    return FP::lambertW0l_lb(l) / b;
  }
  static_assert(ltau_Wlb(1, FP::euler, false) == 1 / FP::euler);
  static_assert(ltau_Wlb(1, tauWlb_eq) == FP::log(4)/(1+tauWlb_eq));

  /* The upper bound
       ltau(1,x) <= 1/x * ln(x / W(x) + 1) = 1/x * ln(exp(W(x))+1),
     yielding
       ltau(a,b) = 1/a ltau(1,b/a) <= ln(b/a / W(b/a) + 1) / b
                                    = ln(exp(W(b/a)) + 1) / b,
     using then either (again) the lower bound for Lambert-W,
     or the upper bound (but the second choice yields a worse approximation).

     Derivation (using exp(-x) <= 1/(1+x)):
       exp(- 1/a * ln(exp(W(a))+1)) <= 1 / (1 + 1/a ln(exp(W(a))))
       = 1 / (1 + W(a)/a)
       exp(-a * 1/a * ln(exp(W(a))+1)) = exp(ln(exp(W(a))+1))^-1
       = 1/ (exp(W(a)) + 1) = 1 / (a/W(a) + 1)
       1/(1+x) + 1/(1+1/x) = 1
     Thus
       ltau(1,a) <= 1/a * ln(exp(W(a)) + 1).

       The quotient (ln(exp(W(b/a)) + 1) / b) / (W(b/a) / b) =
                    ln(exp(W(b/a))+1) / W(b/a)
       goes to 1 with b/a -> infinity. This also holds when the elementary upper and lower
       bounds are used. So the bounds ltau_W(l|u)b are asymptotically optimal.
  */
  constexpr FP::float80 tauWub_eq = 5.71344571324574840133839L;
  constexpr FP::float80 l_tauWub_eq = FP::log(tauWub_eq);
  inline constexpr FP::float80 ltau_Wub(const FP::float80 a, const FP::float80 b, const bool adapted=true) noexcept {
    assert(a > 0);
    assert(b > a);
    const FP::float80 l = FP::log(b) - FP::log(a);
    if (adapted and l < l_tauWub_eq) return FP::log(2) / (FP::sqrt(a)*FP::sqrt(b));
    assert(l >= 1);
    return FP::log1p(b/a/FP::lambertW0l_lb(l)) / b;
  }
  static_assert(ltau_Wub(1, FP::euler, false) == FP::log1p(FP::euler) / FP::euler);
  static_assert(FP::abs(ltau_Wub(1, tauWub_eq) - FP::log(2) / FP::sqrt(tauWub_eq)) < 1e-19L);

  inline constexpr double ltau_Wub_d(const double a, const double b) noexcept {
    constexpr double l_tauWub = l_tauWub_eq;
    assert(a > 0);
    assert(b > a);
    const double l = std::log(b) - std::log(a);
    if (l < l_tauWub) return std::log(2) / (std::sqrt(a)*std::sqrt(b));
    assert(l >= 1);
    return std::log1p(b/a/FP::lambertW0l_lb_d(l)) / b;
  }
  static_assert(std::abs(ltau_Wub_d(1, tauWub_eq) - std::log(2) / std::sqrt(tauWub_eq)) < 1e-16);


  /* The lower bound derived from the upper bound by one Newton-step: */
  /* First the unchecked version: */
  inline constexpr FP::float80 ltau_Wublbu(const FP::float80 a, const FP::float80 b, const FP::float80 na, const FP::float80 nb) noexcept {
    assert(a > 0);
    assert(a < b);
    const FP::float80 x0 = ltau_Wub(a,b);
    const FP::float80 Am1 = FP::expm1(na*x0), B = FP::exp(nb*x0);
    const FP::float80 fx0 = Am1 + B;
    const FP::float80 fpx0 = FP::fma(b,B,FP::fma(a,Am1,a));
    assert(fpx0 > 0);
    const FP::float80 x1 = x0 + fx0/fpx0;
    assert(x1 <= x0);
    return x1;
  }
  inline constexpr FP::float80 ltau_Wublbu(const FP::float80 a, const FP::float80 b) noexcept {
    return ltau_Wublbu(a,b,-a,-b);
  }
  inline constexpr FP::float80 ltau_Wublb(const FP::float80 a0, const FP::float80 b0) noexcept {
    assert(a0 > 0);
    assert(b0 > 0);
    if (a0 == b0) return FP::log(2)/a0;
    const FP::float80 a = (a0 <= b0) ? a0 : b0, b = (a0 <= b0) ? b0 : a0;
    assert(a < b);
    return ltau_Wublbu(a,b);
  }
  static_assert(ltau_Wublb(FP::log(2), FP::log(2)) == 1);
  static_assert(ltau_Wublb(1,2) > ltau_Wlb(1,2));
  static_assert(ltau_Wublb(1,2) < FP::log_golden_ratio);
  static_assert(ltau_Wublb(1,1e1000L) > ltau_Wlb(1,1e1000L));

  inline constexpr double ltau_Wublbu_d(const double a, const double b, const double na, const double nb) noexcept {
    assert(a > 0);
    assert(a < b);
    const double x0 = ltau_Wub_d(a,b);
    const double Am1 = std::expm1(na*x0), B = std::exp(nb*x0);
    const double fx0 = Am1 + B;
    const double fpx0 = std::fma(b,B,std::fma(a,Am1,a));
    assert(fpx0 > 0);
    const double x1 = x0 + fx0/fpx0;
    assert(x1 <= x0);
    return x1;
  }


  /* ltau(a,b) = ln(tau(a,b))
   Let ltau(a,b) := ln(tau(a,b)), for a, b > 0.
   (If a or b is infinite, then ltau(a,b) := 0.)
   The characteristic equation of ltau(a,b) is
     ltau(a,b) is the x > 0 such that
       exp(-x*a) + exp(-x*b) = 1.
   To apply Newton-Raphson, for parameters a,b the map
     x -> f(x) := exp(-x*a) + exp(-x*b) - 1
   is to be differentiated:
     f'(x) = -a*exp(-x*a) - b*exp(-x*b).
   The iteration is
     x_{n+1} = x_n - f(x_n) / f'(x_n).
   The initial guess uses the above lower bound.
*/
  inline constexpr FP::float80 ltau(const FP::float80 a0, const FP::float80 b0) noexcept {
    assert(a0 > 0);
    assert(b0 > 0);
    if (a0 == b0) return FP::log(2) / a0;
    const FP::float80 a = (a0 <= b0) ? a0 : b0, b = (a0 <= b0) ? b0 : a0,
      na = -a, nb = -b;
    assert(a < b);
    if (FP::isinf(b)) return 0;
    FP::float80 x0 = ltau_Wublbu(a,b,na,nb);
    while (true) {
      const FP::float80 Am1 = FP::expm1(na*x0), B = FP::exp(nb*x0);
      const FP::float80 fx0 = Am1 + B;
      if (fx0 <= 0) return x0;
      const FP::float80 rfpx0 = 1 / FP::fma(b,B, FP::fma(a,Am1,a));
      assert(rfpx0 > 0);
      const FP::float80 x1 = FP::fma(fx0, rfpx0, x0);
      assert(x1 >= x0);
      if (x1 == x0) return x0;
      x0 = x1;
    }
  }
  static_assert(ltau(1,1) == FP::log(2));
  static_assert(ltau(2,2) == FP::log(2)/2);
  static_assert(ltau(3,3) == FP::log(2)/3);
  static_assert(ltau(1000,1000) == FP::log(2)/1000);
  static_assert(ltau(1e+1000L, 1e+1000L) == FP::log(2) * 1e-1000L);
  static_assert(ltau(1e-1000L, 1e-1000L) == FP::log(2) * 1e1000L);
  static_assert(ltau(FP::max_value,FP::max_value) > 0);
  static_assert(ltau(FP::max_value,FP::max_value) < FP::min_value);
  static_assert(ltau(FP::max_value,FP::max_value) == FP::log(2)/FP::max_value);
  static_assert(ltau(FP::min_value,FP::min_value) < FP::max_value);
  static_assert(ltau(FP::min_value,FP::min_value) == FP::log(2)/FP::min_value);
  static_assert(ltau(1,2) == FP::log_golden_ratio);
  static_assert(ltau(2,4) == FP::log_golden_ratio/2);
  static_assert(FP::abs(ltau(1e10,2e10) - FP::log_golden_ratio * 1e-10L) < 1e-29L);
  static_assert(ltau(1e1000L,2e1000L) == FP::log_golden_ratio * 1e-1000L);
  static_assert(ltau(1e-1000L,2e-1000L) == FP::log_golden_ratio * 1e1000L);
  static_assert(ltau(3,7) > ltau(3,7+6*FP::epsilon));
  static_assert(ltau(3*5,7*5) == ltau(3,7)/5);
  static_assert(FP::abs(ltau(23,57) - 1.855192727790445657682267e-2L) < 1e-20L);
  static_assert(ltau(0.1,0.23) == 4.451086045963786618L);
  static_assert(ltau(0.1,123) == 0.044112256194439923384L);
  static_assert(abs(ltau(0.123,54321) - 1.95765471079164775334702e-4L) < FP::min_value);
  static_assert(ltau(0.02345,0.00543) == 56.65900358501618499L);
  static_assert(ltau(21,23) == 0.031529279361734392134L);
  static_assert(ltau(1,FP::max_value) > FP::min_value);
  static_assert(ltau(FP::pinfinity, 1) == 0);
  static_assert(ltau(1, FP::pinfinity) == 0);
  static_assert(ltau(FP::pinfinity,FP::pinfinity) == 0);
  static_assert(ltau_Wublb(1,1e1000L) < ltau(1,1e1000L));


  typedef std::pair<FP::float80,FP::float80> floatpair_t;
  // The induced probability distribution of length 2, via their logarithms
  // and as std::pair:
  inline constexpr floatpair_t lprobtau(const FP::float80 a, const FP::float80 b) noexcept {
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
  inline constexpr FP::float80 ltau21a(const FP::float80 lt) noexcept {
    assert(lt >= 0);
    if (lt == 0) return FP::pinfinity;
    else return - FP::log(-FP::expm1(-lt)) / lt;
  }
  static_assert(ltau21a(FP::log(2)) == 1);
  // Solving ltau(1, ltau_1eq(a,b)) = ltau(a,b):
  inline constexpr FP::float80 ltau_1eq(const FP::float80 a, const FP::float80 b) noexcept {
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
  inline constexpr FP::float80 ltau2aa(const FP::float80 lt) noexcept {
    assert(lt > 0);
    return FP::log(2) / lt;
  }
  static_assert(ltau2aa(FP::log(2)) == 1);
  // Solving ltau(tau_mean(a,b), tau_mean(a,b)) = ltau(a,b); this acts also
  // as a generalised mean (see Handbook article):
  inline constexpr FP::float80 tau_mean(const FP::float80 a, const FP::float80 b) noexcept {
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

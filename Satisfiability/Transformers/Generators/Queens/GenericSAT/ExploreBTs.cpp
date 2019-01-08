// Oliver Kullmann, 2.1.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Analysis of the ltau-computation

USAGE:

> ExploreBts

gives information on input and output.

> ExploreBTs x
a b ltau N rp eam1 eb sum ldiff

(where "x" can be any string) prints the header line for the output:
 - a, b the inputs, sorted in ascending order
 - ltau = ln(tau(a,b))
 - N is the number of iterations
 - rp is the return-place
 - eam1 = exp(-a*ltau)-1
 - eb = exp(-b*ltau)
 - sum = eam1 + eb
 - ldff = ln(b) - ln(a)

> ExploreBTs a b

prints the results of the computation for ln(tau(a,b)).

TODOS:

1. Analyse ltau:

   (a) Are there cases where the current implementation does not terminate?
       Apparently not.
   (b) Are there cases where one of the asserts triggers (of course, not
       considering the two asserts for the arguments)?
       Apparently not.
   (c) The computation of fpx0 = a*Am1 + a + b*B could also be formulated as
       fpx0 = a*(Am1 + 1) + b*B, however the current form seems numerically
       better -- is this true? One would assume that the form with "+1"
       is slightly faster.
        (1) Addition +1 introduces an error, and then we multiplied with that
            accumulated error.
        (2) The current form not only avoids this, but is then also able to
            use fma.
   (d) How many interations are used? Where is the maximum reached, and
       how big is it?
        (1) A surprisingly good approximation of the number of iterations is
            ln(b/a) (assuming b >= a), that is, ln(b)-ln(a).
        (2) Apparently due to the capping of the precision, this is shortcut
            at 11400 iterations.
        (3) How is the relation for float (32 bits) and double (64 bits)?
   (e) Some approximations of the error, perhaps in dependency of ln(b/a),
       are needed.
        (1) We should check whether we have quadratic convergence (and
            what that means), as predicted by the general results for
            Newton's method. Perhaps the convergence speed should be
            measured.
            https://en.wikipedia.org/wiki/Rate_of_convergence

(2) Alternative methods
   (a) When we start below the tau-value, we have strictly monotonically
       increasing convergence. And above -- is it now strictly monotonically
       decreasing?
   (b) The Newton-Fourier method
       https://en.wikipedia.org/wiki/Newton%27s_method#Newton–Fourier_method
       yields lower and upper bounds at the same time, using a somewhat
       changed second strand -- how does this fit?
   (c) Via Halley's method
       https://en.wikipedia.org/wiki/Halley%27s_method
       we should obtain cubic convergence.

(3) Better exproximation for ltau(1,a) for a >= 1.
   (a) In principle, handling of this case is enough, via
       ltau(a,b) = 1/a ltau(1,b/a) for a <= b.
   (b) While having good lower bounds (for some a) would enable faster general
       computations, which currently just uses the start value ltau(1,1)
       (scaled).
       On should start curve-fitting, to see the general shape (growth rate)
       of f(x) = ltau(1,x), x >= 1.
   (b) ltau(1,1) = ln(2), ltau(1,2) = ln(1+sqrt(5)) - ln(2).
       At Maxima we see formulas via radicals for tau(1,3), tau(1,4), while
       the method fails for integers a >= 5 -- is this provably?
       tau(1,5): x^-1 + x^-5 = 1, <=> x^4 + 1 = x^5 <=> x^5 - x^4 - 1 = 0 ---
       is it provable that this polynomial has no solutions via radicals?
       Algorithms (implemented) should exist to decide this question.
   (c) The general polynomial is x^a - x^(a-1) - 1 = 0. Having more information
       on this for natural numbers a would be helpful.
       The related reccurrences are
       f(n) = f(n-1) + f(n-a). In Maxima, for a=5:
       f5(n) := if n<5 then n else f5(n-1) + f5(n-5)
       for n >= 0. It seems that this sequence is not in the OEIS.
         create_list(f5(n),n,0,20);
         [0,1,2,3,4,4,5,7,10,14,18,23,30,40,54,72,95,125,165,219,291]
   (d) One would assume that considering e.g.
         exp(-x) + exp(-5*x) = 1
       wouldn't have analytical advantages (only numerical)?
       All that should be raised at MathOverflow.
   (e) The general bounds we have are,
       using the general bounds
         mtau(a,b) := ln(2) / ltau(a,b)
         sqrt(a*b) <= mtau(a,b) <= (a+b)/2,
       which yields
         ltau(1,a) >= 2ln(2) / (a+1)
         ltau(1,a) <= ln(2) / sqrt(a).
       One would guess that the upper bound is closer to the truth?
       Considering the quotients, that's only true for parameter a not too big.
       For the curve-fitting as above, one would guess an approach
         f(x) ~ alpha / (x+beta)^gamma
       for 1/2 < gamma < 1, should yield a reasonable fit.

*/

#include <iostream>
#include <string>

#include "FloatingPoint.hpp"
#include "BranchingTuples.hpp"

namespace {

  const std::string version = "0.2.3";
  const std::string date = "4.1.2019";
  const std::string program = "ExploreBTs"
#ifndef NDEBUG
  "_debug"
#endif
;
  const std::string error = "ERROR[" + program + "]: ";


  namespace FP = FloatingPoint;

  struct Result_t {
    FP::floating_t t;
    FP::uint_t c;
    int place;
  };
  std::ostream& operator <<(std::ostream& out, const Result_t r) {
    return out << FP::Wrap(r.t) << " " << r.c << " " << r.place;
  }

  // Version with counting iterations:
  inline constexpr Result_t ltau(FP::floating_t a, FP::floating_t b) noexcept {
    assert(a > 0);
    assert(b > 0);
    if (a == b) return {FP::log(2)/a, 0, 1};
    if (a > b) {const auto t=a; a=b; b=t;}
    assert(a < b);
    if (FP::isinf(b)) return {0, 0, 2};
    FP::floating_t x0 = FP::log(4) / (a+b);
    FP::uint_t rounds = 0;
    while (true) {
      ++rounds;
      const FP::floating_t Am1 = FP::expm1(-a*x0), B = FP::exp(-b*x0);
      const FP::floating_t fx0 = Am1 + B;
      if (fx0 < 0) return {x0, rounds, 3};
      if (fx0 == 0) return {x0, rounds, 4};
      const FP::floating_t fpx0 = FP::fma(b,B,FP::fma(a,Am1,a));
      assert(fpx0 > 0);
      const FP::floating_t x1 = x0 + fx0/fpx0;
      assert(x1 >= x0);
      if (x1 == x0) return {x0, rounds, 5};
      x0 = x1;
    }
  }

}

int main(const int argc, const char* const argv[]) {
  if (argc == 1) {
    std::cout << error << "Two arguments a, b > 0 are needed.\n";
    std::cout << " Output: a b ln(tau(a,b)) N ret-p exp(-a*t)-1 exp(-b*t) sum ln(b)-ln(a).\n";
    return 0;
  }
  if (argc == 2) {
    std::cout << "a b ltau N rp eam1 eb sum ldiff\n";
    return 0;
  }
  using namespace FP;
  const floating_t a0 = std::stold(argv[1]), b0 = std::stold(argv[2]);
  const floating_t a = min(a0,b0), b = max(a0,b0);
  const auto res = ltau(a,b);
  const floating_t lt = res.t, Am1 = expm1(-a*lt), B = exp(-b*lt),
    sum = Am1 + B, pred_num_its = log(b) - log(a);
  std::cout << Wrap(a) << " " << Wrap(b) << " " << res << " " << Wrap(Am1) << " " << Wrap(B) << " " << Wrap(sum) << " " << Wrap(pred_num_its) << "\n";
}

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

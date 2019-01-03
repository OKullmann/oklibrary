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
a b ltau N eam1 eb sum ldiff

(where "x" can be any string) prints the header line for the output:
 - a, b the inputs, sorted in ascending order
 - ltau = ln(tau(a,b))
 - N is the number of iterations
 - eam1 = exp(-a*ltau)-1
 - eb = exp(-b*ltau)
 - sum = eam1 + eb
 - ldff = ln(b) - ln(a)

> ExploreBTs a b

prints the results of the computation for ln(tau(a,b)).

*/

#include <iostream>
#include <string>
#include <utility>

#include "FloatingPoint.hpp"
#include "BranchingTuples.hpp"

namespace {

  const std::string version = "0.2";
  const std::string date = "3.1.2019";
  const std::string program = "ExploreBTs"
#ifndef NDEBUG
  "_debug"
#endif
;
  const std::string error = "ERROR[" + program + "]: ";


  namespace FP = FloatingPoint;

  typedef std::pair<FP::floating_t, FP::UInt_t> Result_t;
  std::ostream& operator <<(std::ostream& out, const Result_t r) {
    return out << FP::Wrap(r.first) << " " << r.second;
  }

  // Version with counting iterations:
  inline constexpr Result_t ltau(FP::floating_t a, FP::floating_t b) noexcept {
    assert(a > 0);
    assert(b > 0);
    if (a > b) {const auto t=a; a=b; b=t;}
    if (FP::isinf(b)) return {0,0};
    FP::floating_t x0 = FP::log(4) / (a+b);
    FP::UInt_t rounds = 0;
    while (true) {
      ++rounds;
      const FP::floating_t Am1 = FP::expm1(-a*x0), B = FP::exp(-b*x0);
      const FP::floating_t fx0 = Am1 + B;
      if (fx0 <= 0) return {x0,rounds};
      const FP::floating_t fpx0 = a*Am1 + a + b*B;
      assert(fpx0 > 0);
      const FP::floating_t x1 = x0 + fx0/fpx0;
      assert(x1 >= x0);
      if (x1 == x0) return {x0,rounds};
      x0 = x1;
    }
  }

}

int main(const int argc, const char* const argv[]) {
  if (argc == 1) {
    std::cout << error << "Two arguments a, b > 0 are needed.\n";
    std::cout << " Output: a b ln(tau(a,b)) N exp(-a*t)-1 exp(-b*t) sum ln(b)-ln(a).\n";
    return 0;
  }
  if (argc == 2) {
    std::cout << "a b ltau N eam1 eb sum ldiff\n";
    return 0;
  }
  using namespace FP;
  const floating_t a0 = std::stold(argv[1]), b0 = std::stold(argv[2]);
  const floating_t a = min(a0,b0), b = max(a0,b0);
  const auto res = ltau(a,b);
  const floating_t lt = res.first, Am1 = expm1(-a*lt), B = exp(-b*lt),
    sum = Am1 + B, pred_num_its = log(b) - log(a);
  std::cout << Wrap(a) << " " << Wrap(b) << " " << res << " " << Wrap(Am1) << " " << Wrap(B) << " " << Wrap(sum) << " " << Wrap(pred_num_its) << "\n";
}

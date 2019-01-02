// Oliver Kullmann, 2.1.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <iostream>
#include <string>
#include <utility>

#include "FloatingPoint.hpp"
#include "BranchingTuples.hpp"

namespace {

  const std::string version = "0.1.3";
  const std::string date = "2.1.2019";
  const std::string program = "ExploreBTs"
#ifndef NDEBUG
  "_debug"
#endif
;
  const std::string error = "ERROR[" + program + "]: ";


  namespace FP = FloatingPoint;

  typedef std::pair<FP::floating_t, FP::UInt_t> Result_t;
  std::ostream& operator <<(std::ostream& out, const Result_t r) {
    const auto prec = out.precision();
    out.precision(FP::limitfloat::digits10 + 2);
    out << r.first << " " << r.second;
    out.precision(prec);
    return out;
  }

  // Version with counting iterations:
  inline constexpr Result_t ltau(FP::floating_t a, FP::floating_t b) noexcept {
    assert(a > 0);
    assert(b > 0);
    if (a > b) {const auto t=a; a=b; b=t;}
    if (std::isinf(b)) return {0,0};
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
  if (argc < 3) {
    std::cerr << error << "two arguments a, b > 0 are needed.\n";
    return 1;
  }
  const FP::floating_t a = std::stold(argv[1]);
  const FP::floating_t b = std::stold(argv[2]);
  std::cout << ltau(a,b) << "\n";
}

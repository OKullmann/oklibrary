// Oliver Kullmann, 19.1.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Simple measurement of tau-runtimes



TODOS:

1. Basic measurements using parameters
     64/80 1e1 1e1 1e7 1e10

csltok:
T> time ./MeasureTau 64 1e1 1e1 1e7 1e10
75750.154509474588849
real    170m59.247s
user    170m10.173s
sys     0m18.904s

80 XXX

cs-wsok:

csverify:
$ time ./MeasureTau 64 1e1 1e1 1e7 1e10
75750.154509474588849
real    32m42.582s
user    32m42.582s
sys     0m0.004s

80 XXX

cs2-irfan536:


*/

#include <iostream>
#include <string>

#include <cassert>

#include "FloatingPoint.hpp"
#include "BranchingTuples.hpp"

namespace {

  const std::string version = "0.0.1";
  const std::string date = "19.1.2019";
  const std::string program = "MeasureTau"
#ifndef NDEBUG
  "_debug"
#endif
;
  const std::string error = "ERROR[" + program + "]: ";

  namespace FP = FloatingPoint;

  void show_usage() noexcept {
    std::cout << "Arguments:" << " digits a b_begin b_end b_steps\n";
  }

}

int main(const int argc, const char* const argv[]) {
  if (argc == 1) { show_usage(); return 0; }
  if (argc != 6) {
    std::cerr << error << "0 or five arguments needed.\n";
    return 1;
  }
  const std::string form = argv[1];
  if (form != "64" and form != "80") {
    std::cerr << error << "form must be \"64\" or \"80\".\n";
    return 1;
  }
  const bool use_double = form == "64";
  const FP::float80 a = std::stold(argv[2]),
    b_begin = std::stold(argv[3]), b_end = std::stold(argv[4]), b_steps_0 = std::stold(argv[5]);
  const FP::UInt_t b_steps = b_steps_0;
  assert(b_steps < FP::P264m1);
  const FP::float80 delta = (b_end - b_begin) / b_steps;
  if (delta == 0) {
    std::cerr << error << "delta=0.\n";
    return 1;
  }
  FP::float80 sum = 0;
  for (FP::UInt_t i = 0; i <= b_steps; ++i) {
    const FP::float80 b = b_begin + i * delta;
    sum += (use_double) ? BranchingTuples::ltau_d(a,b) : BranchingTuples::ltau(a,b);
  }
  std::cout << FP::Wrap(sum) << "\n";
}

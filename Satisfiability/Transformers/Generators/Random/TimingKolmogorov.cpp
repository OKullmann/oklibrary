// Oliver Kullmann, 5.5.2019 (London)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Timing and counting of class p-value computation from the Kolmogorov D-value

Examples (annotations on following lines):

Version information:


*/

#include <iostream>
#include <vector>

#include <cassert>

#include <Numerics/FloatingPoint.hpp>
#include <ProgramOptions/Environment.hpp>

#include "Tests.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.0",
        "5.5.2019",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/TimingKolmogorov.cpp",
        "GPL v3"};

  using namespace RandGen;
  using FloatingPoint::Wrap;
  using FloatingPoint::float80;

  constexpr gen_uint_t N_default = 16'000;
  constexpr float80 D_default = 0.016L;

}

int main(const int argc0, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc0, argv)) return 0;
  const bool profiling = Environment::profiling(argc0, argv);
  const int argc = profiling ? 1 : argc0;

  Environment::Index index;
  const gen_uint_t N = (argc <= index) ? N_default : FloatingPoint::toUInt(argv[index++]);
  assert(N >= 1);
  const float80 D = (argc <= index) ? D_default : FloatingPoint::stold(argv[index++]);
  assert(D >= 0 and D <= 1);
  index.deactivate();

  std::cout << N << " " << Wrap(D) << " " << Wrap(ks_P(N, D)) << "\n";

}

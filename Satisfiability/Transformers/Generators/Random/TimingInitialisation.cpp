// Oliver Kullmann, 15.3.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Timing of initialisation of Mersenne-Twister

Example (annotation on next line)

Random> ./TimingInitialisation 10 1e6
# size of seed-sequence and number N of initialisations of the generator (default values)
0.1.0 15.3.2019 TimingInitialisation 454a2364afa29b2724aa077818af7d9ed155a782
# version, last change-date, name, git ID
csltok.swansea.ac.uk g++ 8.3.0 Mar 15 2019 04:20:59
# machine, compiler-version, compilation-date
--std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static -fno-finite-math-only
# optimisation options
10 1000000 16124971662046032120
# the arguments plus the sum of all generated numbers (as unsigned 64bit)
10 1e+06
# arguments again, in float80-precision.


Results:

On csltok:


On cs-wsok:


On csverify:


*/

#include <iostream>
#include <algorithm>

#include <cassert>

#include <Numerics/FloatingPoint.hpp>

#include "Numbers.hpp"
#include "Environment.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.1",
        "15.3.2019",
        __FILE__};

  using namespace RandGen;

  constexpr gen_uint_t size_default = 10;
  constexpr gen_uint_t N_default = 1e6L;

}

int main(const int argc, const char* const argv[]) {

  const gen_uint_t size = (argc == 1) ? size_default : std::max(gen_uint_t(1),FloatingPoint::toUInt(argv[1]));
  const gen_uint_t N = (argc <= 2) ? N_default : FloatingPoint::toUInt(argv[2]);

  assert(size >= 1);
  vec_seed_t seeds; seeds.resize(size);

  gen_uint_t sum = 0;
  for (; seeds[0] < N; ++seeds[0]) {
    RandGen_t g(seeds);
    sum += g();
  }


  std::cout << proginfo;

  std::cout << size << " " << N << " " << sum << "\n";

  using FloatingPoint::float80;
  std::cout << float80(size) << " " << float80(N) << "\n";

}

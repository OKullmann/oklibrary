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
10 1000000 16124971662046032120
# the arguments plus the sum of all generated numbers (as unsigned 64bit)
10 1e+06
# arguments again, in float80-precision.


Results:

On csltok:

Random> ./RunTime ./TimingInitialisation
program name:       TimingInitialisation
 version:           0.1.4
 last change:       19.3.2019
 git-id:            ab7c0e5bddce380d43b683649f90df701d1a3a64
machine name:       csltok.swansea.ac.uk
 bogomips:          4788.21
compiler version:   g++ 8.3.0
 date:              Mar_19_2019 21:01:59
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only
Output program:
10 1000000 16124971662046032120
10 1e+06
MIN + MAX user times:
15.21 15.39

Roughly 1e6 / 15.21 ~ 65.7e3 initialisatons per sec.


On cs-wsok:

Random> ./RunTime ./TimingInitialisation
program name:       TimingInitialisation
 version:           0.1.4
 last change:       19.3.2019
 git-id:            ab7c0e5bddce380d43b683649f90df701d1a3a64
machine name:       cs-wsok
 bogomips:          5986.74
compiler version:   g++ 8.3.0
 date:              Mar_19_2019 21:04:54
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only
Output program:
10 1000000 16124971662046032120
10 1e+06
MIN + MAX user times:
9.89 9.94

Roughly 1e6 / 9.89 ~ 101e3 initialisatons per sec.


On csverify:

Random$ ./RunTime ./TimingInitialisation
program name:       TimingInitialisation
 version:           0.1.4
 last change:       19.3.2019
 git-id:            ab7c0e5bddce380d43b683649f90df701d1a3a64
machine name:       csverify
 bogomips:          7183.87
compiler version:   g++ 8.2.0
 date:              Mar_19_2019 21:05:21
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only
Output program:
10 1000000 16124971662046032120
10 1e+06
MIN + MAX user times:
5.65 5.72

Roughly 1e6 / 5.65 ~ 177e3 initialisatons per sec.


TODO: analyse the dependency on the length of the sequence.
Hypothesis: this is a linear relation.

*/

#include <iostream>
#include <algorithm>

#include <cassert>

#include <Numerics/FloatingPoint.hpp>
#include <ProgramOptions/Environment.hpp>

#include "Distributions.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.4",
        "19.3.2019",
        __FILE__};

  using namespace RandGen;

  constexpr gen_uint_t size_default = 10;
  constexpr gen_uint_t N_default = 1e6L;

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  const gen_uint_t size = (argc == 1) ? size_default : std::max(gen_uint_t(1),FloatingPoint::toUInt(argv[1]));
  const gen_uint_t N = (argc <= 2) ? N_default : FloatingPoint::toUInt(argv[2]);

  assert(size >= 1);
  vec_seed_t seeds; seeds.resize(size);

  gen_uint_t sum = 0;
  for (; seeds[0] < N; ++seeds[0]) {
    RandGen_t g(seeds);
    sum += g();
  }


  std::cout << size << " " << N << " " << sum << "\n";

  using FloatingPoint::float80;
  std::cout << float80(size) << " " << float80(N) << "\n";

}

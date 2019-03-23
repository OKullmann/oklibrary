// Oliver Kullmann, 4.3.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Timing of generation-function for Mersenne-Twister

Example (annotation on next line)

Random> ./TimingGeneration 3e9
# number N of calls of the generator (this is the default value)
3000000000 12297525670096617830
# the argument plus the sum of all generated numbers (as unsigned 64bit)
3e+09
# N again, in float80-precision.


Results:

On csltok:

Random> ./RunTime ./TimingGeneration
program name:       TimingGeneration
 version:           0.2.4
 last change:       19.3.2019
 git-id:            8faf8629885596dfc2342df79985d6e30898160c
machine name:       csltok.swansea.ac.uk
 bogomips:          4788.21
compiler version:   g++ 8.3.0
 date:              Mar_19_2019 19:05:31
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only
Output program:
3000000000 12297525670096617830
3e+09
MIN + MAX user times:
13.33 13.65

So roughly 3e9 / 13.33 ~ 225e6 generations per sec.


On cs-wsok:

Random> ./RunTime ./TimingGeneration
program name:       TimingGeneration
 version:           0.2.4
 last change:       19.3.2019
 git-id:            8faf8629885596dfc2342df79985d6e30898160c
machine name:       cs-wsok
 bogomips:          5986.74
compiler version:   g++ 8.3.0
 date:              Mar_19_2019 19:13:20
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only
Output program:
3000000000 12297525670096617830
3e+09
MIN + MAX user times:
9.47 9.57

So roughly 3e9 / 9.47 ~ 317e6 generations per sec.


On csverify:

Random$ ./RunTime ./TimingGeneration
program name:       TimingGeneration
 version:           0.2.4
 last change:       19.3.2019
 git-id:            8faf8629885596dfc2342df79985d6e30898160c
machine name:       csverify
 bogomips:          7183.87
compiler version:   g++ 8.2.0
 date:              Mar_19_2019 19:21:10
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only
Output program:
3000000000 12297525670096617830
3e+09
MIN + MAX user times:
5.28 5.39

So roughly 3e9 / 5.28 ~ 568e6 generations per sec.

*/

#include <iostream>

#include <Numerics/FloatingPoint.hpp>
#include <ProgramOptions/Environment.hpp>

#include "Numbers.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.2.5",
        "23.3.2019",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/TimingGeneration.cpp",
        "GPL v3"};

  using namespace RandGen;

  constexpr gen_uint_t N_default = 3e9L;

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  const gen_uint_t N = (argc == 1) ? N_default : FloatingPoint::toUInt(argv[1]);


  gen_uint_t sum = 0;
  RandGen_t g;
  for (gen_uint_t i = 0; i < N; ++i) sum += g();


  std::cout << N << " " << sum << "\n";

  std::cout << FloatingPoint::float80(N) << "\n";

}

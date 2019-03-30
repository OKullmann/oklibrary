// Oliver Kullmann, 7.3.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Timing of class Bernoulli2

Example (annotation on next line):

Random> ./TimingBernoulli2 -v
program name:       TimingBernoulli2
 version:           0.3.0
 last change:       21.3.2019
 git-id:            efd3a73bdaecf88852ec2e5cbda2dfc5c225d460
machine name:       csltok.swansea.ac.uk
 bogomips:          4788.21
compiler version:   g++ 8.3.0
 date:              Mar_21_2019 21:30:08
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only

Random> ./TimingBernoulli2 1e9 3 1
# number N of calls of the generator, exponent e of denominator, nominator x (default values)
# plus sequence of 64-bit seed values (empty by default)
1000000000 3 1
# the arguments (in integer-format)
()
# the list of 32-bit seeds (initialising the generator)
124997746 0.124997746 0.82935909643158182823
# the count of results "true", their relative frequency, and the p-value
1e+09 0.125
# N again, in float80-precision, and x/2^e.


Results:

On csltok:

Random> ./RunTime ./TimingBernoulli2
program name:       TimingBernoulli2
 version:           0.2.3
 last change:       19.3.2019
 git-id:            ab7c0e5bddce380d43b683649f90df701d1a3a64
machine name:       csltok.swansea.ac.uk
 bogomips:          4788.21
compiler version:   g++ 8.3.0
 date:              Mar_19_2019 21:01:57
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only
Output program:
1000000000 3 1
()
124997746 0.124997746
1e+09 0.125
MIN + MAX user times:
5.38 5.56

Roughly 1e9 / 5.38 ~  186e6 generations per sec, 186 / 188 ~ 99%
of the bernoulli-speed.
This is 186 / 189 ~ 98% of the speed of the previous version.

New version:
andom> ./RunTime ./TimingBernoulli2
program name:       TimingBernoulli2
 version:           0.3.0
 last change:       21.3.2019
 git-id:            efd3a73bdaecf88852ec2e5cbda2dfc5c225d460
machine name:       csltok.swansea.ac.uk
 bogomips:          4788.21
compiler version:   g++ 8.3.0
 date:              Mar_21_2019 21:30:08
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only
Output program:
1000000000 3 1
()
124997746 0.124997746 0.82935909643158182823
1e+09 0.125
MIN + MAX user times:
5.42 5.54


On cs-wsok:

Random> ./RunTime ./TimingBernoulli2
program name:       TimingBernoulli2
 version:           0.2.3
 last change:       19.3.2019
 git-id:            ab7c0e5bddce380d43b683649f90df701d1a3a64
machine name:       cs-wsok
 bogomips:          5986.74
compiler version:   g++ 8.3.0
 date:              Mar_19_2019 21:04:53
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only
Output program:
1000000000 3 1
()
124997746 0.124997746
1e+09 0.125
MIN + MAX user times:
4.14 4.18

Roughly 1e9 / 4.14 ~  242e6 generations per sec, 242 / 252 ~ 96%
of the bernoulli-speed.
This is 242 / 253 ~ 96% of the speed of the previous version.

New version:
Random> ./RunTime ./TimingBernoulli2
program name:       TimingBernoulli2
 version:           0.3.0
 last change:       21.3.2019
 git-id:            efd3a73bdaecf88852ec2e5cbda2dfc5c225d460
machine name:       cs-wsok
 bogomips:          5986.74
compiler version:   g++ 8.3.0
 date:              Mar_21_2019 21:33:49
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only
Output program:
1000000000 3 1
()
124997746 0.124997746 0.82935909643158182823
1e+09 0.125
MIN + MAX user times:
4.15 4.19


On csverify:

$ ./RunTime ./TimingBernoulli2
program name:       TimingBernoulli2
 version:           0.2.3
 last change:       19.3.2019
 git-id:            ab7c0e5bddce380d43b683649f90df701d1a3a64
machine name:       csverify
 bogomips:          7183.87
compiler version:   g++ 8.2.0
 date:              Mar_19_2019 21:05:20
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only
Output program:
1000000000 3 1
()
124997746 0.124997746
1e+09 0.125
MIN + MAX user times:
2.39 2.44

Roughly 1e9 / 2.39 ~  418e6 generations per sec, 418 / 492 ~ 85%
of the bernoulli-speed.
A surprising drop.

New version:
Random$ ./RunTime ./TimingBernoulli2
program name:       TimingBernoulli2
 version:           0.3.0
 last change:       21.3.2019
 git-id:            efd3a73bdaecf88852ec2e5cbda2dfc5c225d460
machine name:       csverify
 bogomips:          7183.87
compiler version:   g++ 8.2.0
 date:              Mar_21_2019 21:35:46
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only
Output program:
1000000000 3 1
()
124997746 0.124997746 0.82935909643158182823
1e+09 0.125
MIN + MAX user times:
2.39 2.45

*/

#include <iostream>

#include <Numerics/FloatingPoint.hpp>
#include <ProgramOptions/Environment.hpp>

#include <cassert>

#include "Distributions.hpp"
#include "Tests.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.3.3",
        "30.3.2019",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/TimingBernoulli2.cpp",
        "GPL v3"};

  using namespace RandGen;

  constexpr gen_uint_t N_default = 1e9L;
  constexpr gen_uint_t e_default = 3;

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  const gen_uint_t N = (argc == 1) ? N_default : FloatingPoint::toUInt(argv[1]);
  const gen_uint_t e = (argc <= 2) ? e_default : FloatingPoint::toUInt(argv[2]);
  const gen_uint_t size = FloatingPoint::exp2(e);
  const gen_uint_t x = (argc <= 3) ? 1 : FloatingPoint::toUInt(argv[3]);
  assert(x <= size);
  vec_eseed_t seeds64;
  if (argc >= 5) {
    seeds64.reserve(argc-4);
    for (int i = 4; i < argc; ++i)
      seeds64.push_back(FloatingPoint::toUInt(argv[i]));
  }


  Count_true ct;
  const vec_seed_t seeds = transform(seeds64);
  randgen_t g{init(seeds)};
  Bernoulli2 b(g,x,e);
  for (gen_uint_t i = 0; i < N; ++i) ct(b());


  using FloatingPoint::float80;
  using FloatingPoint::Wrap;
  std::cout << N << " " << e << " " << x;
  for (const auto x : seeds64) std::cout << " " << x;
  std::cout << "\n" << RandGen::SW{seeds};
  const float80 p = float80(x) / size;
  std::cout << "\n" << *ct << " " << Wrap(float80(*ct) / N) << " " << Wrap(monobit(*ct, N, p)) << "\n";
  std::cout << float80(N) << " " << Wrap(p) << "\n";

}

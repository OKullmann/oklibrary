// Oliver Kullmann, 5.3.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Timing of function bernoulli(g)

Example (annotation on next line):

Random> ./TimingBernoulli12 -v
program name:       TimingBernoulli12
 version:           0.4.0
 last change:       21.3.2019
 git-id:            e130527c6a50f5016461088eeffdee7e29961895
machine name:       csltok.swansea.ac.uk
 bogomips:          4788.21
compiler version:   g++ 8.3.0
 date:              Mar_21_2019 19:27:32
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only

Random> ./TimingBernoulli12 3e9 0
# number N of calls of the generator and number of discards (default values),
# plus sequence of 64-bit seed values (empty by default)
3000000000 0
# the arguments (in integer-format)
()
# the list of 32-bit seeds (initialising the generator)
1499919941 0.49997331366666666666 0.0034629664979143074932
# the count of results "true", their relative frequency, and the p-value
3e+09 0
# N and discards again, in float80-precision.

Remark: The p-value is very low: the probability that with 3e9 fair coin-flips
we obtain a deviation of the measured frequency f to the true probability of
at least the given value, i.e.,
  |f-0.5| >= 0.5 - 0.49997331366666666666
holds, is 0.35%, which is a low probability, and one would reject that the
given sequence is "random", due to having too few "true"-results.
This 0/1-sequence is further evaluated in Distributions.hpp.

With one seed-value (a 64-bit 0, yielding two 32-bit 0s):
Random> ./TimingBernoulli12 3e9 0 0
3000000000 0 0
(0,0)
1500008065 0.50000268833333333336 0.76838185202788818196
3e+09 0


Results:

On csltok:

Random> ./RunTime ./TimingBernoulli12
program name:       TimingBernoulli12
 version:           0.3.2
 last change:       19.3.2019
 git-id:            0e33bd127e3ef9e78912f8c8a900413d59382c25
machine name:       csltok.swansea.ac.uk
 bogomips:          4788.21
compiler version:   g++ 8.3.0
 date:              Mar_19_2019 20:37:29
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only
Output program:
3000000000 0
()
1499919941 0.49997331366666666666
3e+09 0
MIN + MAX user times:
15.97 16.25

Roughly 3e9 / 15.97 ~ 188e6 generations per sec, which is
188 / 225 ~ 84% of pure generation speed.
And this is 188 / 207 ~ 91% of the speed of the previous version,
where just trivial changes took place -- the compilation is "all over
the place".

New version:
andom> ./RunTime ./TimingBernoulli12
program name:       TimingBernoulli12
 version:           0.4.0
 last change:       21.3.2019
 git-id:            e130527c6a50f5016461088eeffdee7e29961895
machine name:       csltok.swansea.ac.uk
 bogomips:          4788.21
compiler version:   g++ 8.3.0
 date:              Mar_21_2019 19:27:32
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only
Output program:
3000000000 0
()
1499919941 0.49997331366666666666 0.0034629664979143074932
3e+09 0
MIN + MAX user times:
16.08 16.37

It seems that csltok has quite a high runtime-variability.


On cs-wsok:

Random> ./RunTime ./TimingBernoulli12
program name:       TimingBernoulli12
 version:           0.3.2
 last change:       19.3.2019
 git-id:            0e33bd127e3ef9e78912f8c8a900413d59382c25
machine name:       cs-wsok
 bogomips:          5986.74
compiler version:   g++ 8.3.0
 date:              Mar_19_2019 20:41:28
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only
Output program:
3000000000 0
()
1499919941 0.49997331366666666666
3e+09 0
MIN + MAX user times:
11.90 12.00

Roughly 3e9 / 11.9 ~ 252e6 generations per sec, which is
252 / 317 ~ 79.5% of pure generation-speed.
And this is 252 / 291 ~ 87% of the speed of the previous version,

New version:
Random> ./RunTime ./TimingBernoulli12
program name:       TimingBernoulli12
 version:           0.4.0
 last change:       21.3.2019
 git-id:            e130527c6a50f5016461088eeffdee7e29961895
machine name:       cs-wsok
 bogomips:          5986.74
compiler version:   g++ 8.3.0
 date:              Mar_21_2019 19:42:15
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only
Output program:
3000000000 0
()
1499919941 0.49997331366666666666 0.0034629664979143074932
3e+09 0
MIN + MAX user times:
11.87 12.02

Basically the same.


On csverify:

Random$ ./RunTime ./TimingBernoulli12
program name:       TimingBernoulli12
 version:           0.3.2
 last change:       19.3.2019
 git-id:            0e33bd127e3ef9e78912f8c8a900413d59382c25
machine name:       csverify
 bogomips:          7183.87
compiler version:   g++ 8.2.0
 date:              Mar_19_2019 20:42:57
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only
Output program:
3000000000 0
()
1499919941 0.49997331366666666666
3e+09 0
MIN + MAX user times:
6.10 6.39

Roughly 3e9 / 6.1 ~ 492e6 generations per sec, which is
492 / 568 ~ 86.6% of pure generation-speed.
And this is 492 / 522 ~ 94% of the speed of the previous version,

New version:
Random$ ./RunTime ./TimingBernoulli12
program name:       TimingBernoulli12
 version:           0.4.0
 last change:       21.3.2019
 git-id:            e130527c6a50f5016461088eeffdee7e29961895
machine name:       csverify
 bogomips:          7183.87
compiler version:   g++ 8.2.0
 date:              Mar_21_2019 19:45:42
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only
Output program:
3000000000 0
()
1499919941 0.49997331366666666666 0.0034629664979143074932
3e+09 0
MIN + MAX user times:
6.13 6.42

Basically the same.


It is actually surprising that adding just one small test to the generation
slows it down by ~ 10%: the generation should be a much more involved
computation, and so the effect of adding this test should be smaller?
Apparently the computation of g() is very fast, so that such a small addition
matters. Or it is the compilation.

*/

#include <iostream>

#include <cassert>

#include <Numerics/FloatingPoint.hpp>
#include <ProgramOptions/Environment.hpp>

#include "Distributions.hpp"
#include "Tests.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.4.1",
        "22.3.2019",
        __FILE__};

  using namespace RandGen;

  constexpr gen_uint_t N_default = 3e9L;
  constexpr gen_uint_t discard_default = 0;

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  const gen_uint_t N = (argc == 1) ? N_default : FloatingPoint::toUInt(argv[1]);
  assert(N >= 1);
  const gen_uint_t discard = (argc <= 2) ? discard_default : FloatingPoint::toUInt(argv[2]);
  vec_eseed_t seeds64;
  if (argc >= 4) {
    seeds64.reserve(argc-3);
    for (int i = 3; i < argc; ++i)
      seeds64.push_back(FloatingPoint::toUInt(argv[i]));
  }


  const vec_seed_t seeds = transform(seeds64);
  RandGen_t g(seeds);
  g.discard(discard);
  LongestRun count(bernoulli(g));;
  for (gen_uint_t i = 1; i < N; ++i) count(bernoulli(g));
  const auto [lr, cr, ct] = *count;


  using FloatingPoint::float80;
  using FloatingPoint::Wrap;
  std::cout << N << " " << discard;
  for (const auto x : seeds64) std::cout << " " << x;
  std::cout << "\n";
  out_seeds(std::cout, seeds);
  std::cout << "\n" << ct << " " << Wrap(float80(ct) / N) << " " << Wrap(monobit(ct, N)) << "\n";
  std::cout << cr << " " << Wrap(runstest(ct, N, cr)) << "\n";
  std::cout << lr << " " << expectedlongestrun(N) << "\n";
  std::cout << float80(N) << " " << float80(discard) << "\n";

}

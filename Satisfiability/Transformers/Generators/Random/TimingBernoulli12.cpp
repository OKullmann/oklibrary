// Oliver Kullmann, 5.3.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Timing of function bernoulli(g)

Example (annotation on next line):

Random> ./TimingBernoulli12 3e9
# number N of calls of the generator (this is the default value),
# plus sequence of 64-bit seed values (empty by default)
0.2.0 14.3.2019 TimingBernoulli12 0782ded0d27be1b7c9fdcdfb5797112a385bda04
# version, last change-date, name, git ID
g++ 8.3.0 Mar 14 2019 19:55:51
# compiler-version, compilation-date
-Ofast -DNDEBUG -march=native -fwhole-program -static -fno-finite-math-only
# optimisation options
3000000000
# the arguments repeated (in integer-format)
()
# the list of 32-bit seeds (initialising the generator)
1499919941 0.49997331366666666666
# the count of results "true" and their relative frequency
3e+09
# N again, in float80-precision.

With one seed-value:
Random> ./TimingBernoulli12 3e9 0
0.2.0 14.3.2019 TimingBernoulli12 0782ded0d27be1b7c9fdcdfb5797112a385bda04
g++ 8.3.0 Mar 14 2019 20:36:44
-Ofast -DNDEBUG -march=native -fwhole-program -static -fno-finite-math-only
3000000000 0
(0,0)
1500008065 0.50000268833333333336
3e+09


Results:

On csltok:

Random> time ./TimingBernoulli12
0.1.4 10.3.2019 TimingBernoulli12 55c8a21b600a40817a4b729b2948b86cee6b57e0
g++ 8.3.0 Mar 10 2019 04:21:59
-Ofast -DNDEBUG -march=native -fwhole-program -static -fno-finite-math-only -fno-unsafe-math-optimizations -fno-associative-math -fno-reciprocal-math  -fno-signed-zeros -fno-math-errno -fno-trapping-math
3000000000 1499961918 0.499987306
3e+09
real    0m14.500s
user    0m14.460s
sys     0m0.001s

So roughly 207e6 generations per sec, which is 91.6% of the pure
generation-speed.

Concerning optimisation options: "-fwhole-program" has been seen elsewhere
as possibly negative, while here is seems to have no effect. From the
numeric-options we only need "-fno-finite-math-only":

Random> rm TimingBernoulli12
Random> make Optimisation_options="-Ofast -DNDEBUG -march=native -static" numerics_options="-fno-finite-math-only" TimingBernoulli12
Random> time ./TimingBernoulli12
0.1.4 10.3.2019 TimingBernoulli12 eaca56ce83ef28df55e9326f2dc7c15e10c7e9bd
g++ 8.3.0 Mar 14 2019 19:22:18
-Ofast -DNDEBUG -march=native -static -fno-finite-math-only
3000000000 1499961918 0.499987306
3e+09
real    0m14.553s
user    0m14.444s
sys     0m0.067s


On cs-wsok:

Random> time ./TimingBernoulli12
0.1.4 10.3.2019 TimingBernoulli12 55c8a21b600a40817a4b729b2948b86cee6b57e0
g++ 8.3.0 Mar 10 2019 04:22:40
-Ofast -DNDEBUG -march=native -fwhole-program -static -fno-finite-math-only -fno-unsafe-math-optimizations -fno-associative-math -fno-reciprocal-math  -fno-signed-zeros -fno-math-errno -fno-trapping-math
3000000000 1499961918 0.499987306
3e+09
real    0m10.314s
user    0m10.305s
sys     0m0.001s

So roughly 291e6 generations per sec, which is 92.1% of pure
generation-speed.

Also here is seems that the simpler optimisation options given by

Random> make Optimisation_options="-Ofast -DNDEBUG -march=native -static" numerics_options="-fno-finite-math-only" TimingBernoulli12

have the same effect.
Update:
Random> time ./TimingBernoulli12
TimingBernoulli12 0.2.3 15.3.2019 93eb5050dfbacbe5b394337aba92175894220659
cs-wsok 5986.74
g++ 8.3.0 Mar_15_2019 12:16:54
--std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static -fno-finite-math-only
3000000000
()
1499919941 0.49997331366666666666
3e+09
real    0m10.264s
user    0m10.251s
sys     0m0.005s


On csverify:

Random$ time ./TimingBernoulli12
0.1.4 10.3.2019 TimingBernoulli12 55c8a21b600a40817a4b729b2948b86cee6b57e0
g++ 8.2.0 Mar 10 2019 04:24:12
-Ofast -DNDEBUG -march=native -fwhole-program -static -fno-finite-math-only -fno-unsafe-math-optimizations -fno-associative-math -fno-reciprocal-math  -fno-signed-zeros -fno-math-errno -fno-trapping-math
3000000000 1499961918 0.499987306
3e+09
real    0m5.675s
user    0m5.675s
sys     0m0.000

So roughly 529e6 generations per sec, which is 93.4% of pure
generation-speed.

Also here the simple options given by
Random$ make Optimisation_options="-Ofast -DNDEBUG -march=native -static" numerics_options="-fno-finite-math-only" TimingBernoulli12
seem not to change runtimes.
UPDATE:
Random$ time ./TimingBernoulli12
TimingBernoulli12 0.2.3 15.3.2019 e36bfa11535ab14a2bcf597751e46a3747ff5038
csverify 7183.87
g++ 8.2.0 Mar_15_2019 10:27:52
--std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static -fno-finite-math-only
3000000000
()
1499919941 0.49997331366666666666
3e+09
real    0m5.965s
user    0m5.964s
sys     0m0.001s
Here quite a considerable slowdown (as reported in Numbers.hpp), which needs
to be investigated:
 - Using the old, "full" compilation-options doesn't make a change.
 - Resetting the repository to 55c8a21b600a40817a4b729b2948b86cee6b57e0
   does make a change:
Random$ time ./TimingBernoulli12
0.1.4 10.3.2019 TimingBernoulli12 55c8a21b600a40817a4b729b2948b86cee6b57e0
g++ 8.2.0 Mar 15 2019 12:01:07
-Ofast -DNDEBUG -march=native -fwhole-program -static -fno-finite-math-only -fno-unsafe-math-optimizations -fno-associative-math -fno-reciprocal-math  -fno-signed-zeros -fno-math-errno -fno-trapping-math
3000000000 1499961918 0.499987306
3e+09
real    0m5.718s
user    0m5.718s
sys     0m0.000s
But using the newest version again yields something nearly similar:
Random$ time ./TimingBernoulli12
TimingBernoulli12 0.2.3 15.3.2019 93eb5050dfbacbe5b394337aba92175894220659
csverify 7183.87
g++ 8.2.0 Mar_15_2019 12:06:16
--std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static -fno-finite-math-only
3000000000
()
1499919941 0.49997331366666666666
3e+09
real    0m5.761s
user    0m5.757s
sys     0m0.004s

So perhaps one has to accept that at least on this platform the "random"
differences in run-time are as high as 10%.


It is actually surprising that adding just one small test to the generation
slows it down by 7%: the generation should be a much more involved computation,
and so the effect of adding this test should be smaller? Apparently the
computation of g() is very fast, so that such a small addition matters.

*/

#include <iostream>

#include <Numerics/FloatingPoint.hpp>

#include "Distributions.hpp"
#include "Environment.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.2.3",
        "15.3.2019",
        __FILE__};

  using namespace RandGen;

  constexpr gen_uint_t N_default = 3e9L;

}

int main(const int argc, const char* const argv[]) {

  const gen_uint_t N = (argc == 1) ? N_default : FloatingPoint::toUInt(argv[1]);
  vec_eseed_t seeds64;
  if (argc >= 3) {
    seeds64.reserve(argc-2);
    for (int i = 2; i < argc; ++i)
      seeds64.push_back(FloatingPoint::toUInt(argv[i]));
  }

  gen_uint_t count_true = 0;
  const vec_seed_t seeds = transform(seeds64);
  RandGen_t g(seeds);
  for (gen_uint_t i = 0; i < N; ++i) count_true += bernoulli(g);


  std::cout << proginfo;

  using FloatingPoint::float80;
  std::cout << N;
  for (const auto x : seeds64) std::cout << " " << x;
  std::cout << "\n(";
  {bool first = true;
   for (const auto x : seeds)
     if (first) {std::cout << x; first = false;}
     else std::cout << "," << x;
  }
  std::cout << ")\n" << count_true << " " << FloatingPoint::Wrap(float80(count_true) / N) << "\n";
  std::cout << float80(N) << "\n";

}

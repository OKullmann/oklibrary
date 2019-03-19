// Oliver Kullmann, 5.3.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Timing of function bernoulli(g)

Example (annotation on next line):

Random> ./TimingBernoulli12 3e9 0
# number N of calls of the generator and number of discards (default values),
# plus sequence of 64-bit seed values (empty by default)
TimingBernoulli12 0.3.0 17.3.2019 f611f6943f65c89076db4cf3ce929bd55108c340
csltok.swansea.ac.uk 4788.21
g++ 8.3.0 Mar_17_2019 17:24:19
--std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static -fno-finite-math-only
# program information
3000000000 0
# the arguments (in integer-format)
()
# the list of 32-bit seeds (initialising the generator)
1499919941 0.49997331366666666666
# the count of results "true" and their relative frequency
3e+09 0
# N and discards again, in float80-precision.

With one seed-value (a 64-bit 0, yielding two 32-bit 0s):
Random> ./TimingBernoulli12 3e9 0 0
TimingBernoulli12 0.3.0 17.3.2019 f611f6943f65c89076db4cf3ce929bd55108c340
csltok.swansea.ac.uk 4788.21
g++ 8.3.0 Mar_17_2019 17:36:11
--std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static -fno-finite-math-only
3000000000 0 0
(0,0)
1500008065 0.50000268833333333336
3e+09 0


Results:

On csltok:

Random> time ./TimingBernoulli12
TimingBernoulli12 0.2.3 15.3.2019 c3e4d5e6d6b055a2dbb378bc8479bae92d29b3bc
csltok.swansea.ac.uk 4788.21
g++ 8.3.0 Mar_15_2019 18:54:05
--std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static -fno-finite-math-only
3000000000
()
1499919941 0.49997331366666666666
3e+09
real    0m14.575s
user    0m14.504s
sys     0m0.033s

So roughly 207e6 generations per sec, which is 91.6% of the pure
generation-speed.


On cs-wsok:

Random> time ./TimingBernoulli12
TimingBernoulli12 0.2.3 15.3.2019 4c4213e5b304b64d8c3438be8c9da3d7d058ee34
cs-wsok 5986.74
g++ 8.3.0 Mar_15_2019 19:02:25
--std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static -fno-finite-math-only
3000000000
()
1499919941 0.49997331366666666666
3e+09
real    0m10.310s
user    0m10.304s
sys     0m0.000s

So roughly 291e6 generations per sec, which is 92.1% of pure
generation-speed.


On csverify:

Random$ time ./TimingBernoulli12
TimingBernoulli12 0.2.3 15.3.2019 4c4213e5b304b64d8c3438be8c9da3d7d058ee34
csverify 7183.87
g++ 8.2.0 Mar_15_2019 19:16:29
--std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static -fno-finite-math-only
3000000000
()
1499919941 0.49997331366666666666
3e+09
real    0m5.746s
user    0m5.746s
sys     0m0.000s

So roughly 522e6 generations per sec, which is 92% of pure
generation-speed.


It is actually surprising that adding just one small test to the generation
slows it down by 7%: the generation should be a much more involved computation,
and so the effect of adding this test should be smaller? Apparently the
computation of g() is very fast, so that such a small addition matters.

*/

#include <iostream>

#include <Numerics/FloatingPoint.hpp>
#include <ProgramOptions/Environment.hpp>

#include "Distributions.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.3.0",
        "17.3.2019",
        __FILE__};

  using namespace RandGen;

  constexpr gen_uint_t N_default = 3e9L;
  constexpr gen_uint_t discard_default = 0;

}

int main(const int argc, const char* const argv[]) {

  const gen_uint_t N = (argc == 1) ? N_default : FloatingPoint::toUInt(argv[1]);
  const gen_uint_t discard = (argc <= 2) ? discard_default : FloatingPoint::toUInt(argv[2]);
  vec_eseed_t seeds64;
  if (argc >= 4) {
    seeds64.reserve(argc-3);
    for (int i = 3; i < argc; ++i)
      seeds64.push_back(FloatingPoint::toUInt(argv[i]));
  }

  gen_uint_t count_true = 0;
  const vec_seed_t seeds = transform(seeds64);
  RandGen_t g(seeds);
  g.discard(discard);
  for (gen_uint_t i = 0; i < N; ++i) count_true += bernoulli(g);


  std::cout << proginfo;

  using FloatingPoint::float80;
  std::cout << N << " " << discard;
  for (const auto x : seeds64) std::cout << " " << x;
  std::cout << "\n";
  out_seeds(std::cout, seeds);
  std::cout << "\n" << count_true << " " << FloatingPoint::Wrap(float80(count_true) / N) << "\n";
  std::cout << float80(N) << " " << float80(discard) << "\n";

}

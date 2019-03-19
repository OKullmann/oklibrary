// Oliver Kullmann, 7.3.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Timing of class Bernoulli2

Example (annotation on next line):

Random> ./TimingBernoulli2 1e9 3 1
# number N of calls of the generator, exponent e of denominator, nominator x (default values)
# plus sequence of 64-bit seed values (empty by default)
TimingBernoulli2 0.2.0 16.3.2019 228464838cdbccae59a42248dd9b0036ecfe4a5d
csltok.swansea.ac.uk 4788.21
g++ 8.3.0 Mar_16_2019 21:09:12
--std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static -fno-finite-math-only
# information on the program, the machine and the compilation
1000000000 3 1
# the arguments (in integer-format)
()
# the list of 32-bit seeds (initialising the generator)
124997746 0.124997746
#  the count of results "true" and their relative frequency
1e+09 0.125
# N again, in float80-precision, and x/2^e.


Results:

On csltok:

Random> time ./TimingBernoulli2
TimingBernoulli2 0.2.0 16.3.2019 954cfb6d774b9f562424d99234bbd1df6e75bc52
csltok.swansea.ac.uk 4788.21
g++ 8.3.0 Mar_16_2019 21:22:58
--std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static -fno-finite-math-only
1000000000 3 1
()
124997746 0.124997746
1e+09 0.125
real    0m5.307s
user    0m5.292s
sys     0m0.001s

Roughly 189e6 generations per sec, 91% of the bernoulli-speed.


On cs-wsok:

Random> time ./TimingBernoulli2
TimingBernoulli2 0.2.0 16.3.2019 954cfb6d774b9f562424d99234bbd1df6e75bc52
cs-wsok 5986.74
g++ 8.3.0 Mar_16_2019 21:22:22
--std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static -fno-finite-math-only
1000000000 3 1
()
124997746 0.124997746
1e+09 0.125
real    0m3.951s
user    0m3.947s
sys     0m0.000s

Roughly 253e6 generations per sec, 87% of the bernoulli-speed.


On csverify:

csoliver@csverify:~/oklibrary/Satisfiability/Transformers/Generators/Random$ time ./TimingBernoulli2
TimingBernoulli2 0.2.0 16.3.2019 954cfb6d774b9f562424d99234bbd1df6e75bc52
csverify 7183.87
g++ 8.2.0 Mar_16_2019 21:27:17
--std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static -fno-finite-math-only
1000000000 3 1
()
124997746 0.124997746
1e+09 0.125
real    0m2.394s
user    0m2.390s
sys     0m0.004s

Roughly 418e6 generations per sec, which is 80% of the bernoulli-speed.
A surprising drop.

*/

#include <iostream>

#include <Numerics/FloatingPoint.hpp>
#include <ProgramOptions/Environment.hpp>

#include <cassert>

#include "Distributions.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.2.3",
        "19.3.2019",
        __FILE__};

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


  gen_uint_t count_true = 0;
  const vec_seed_t seeds = transform(seeds64);
  randgen_t g{init(seeds)};
  Bernoulli2 b(g,x,e);
  for (gen_uint_t i = 0; i < N; ++i) count_true += b();


  using FloatingPoint::float80;
  std::cout << N << " " << e << " " << x;
  for (const auto x : seeds64) std::cout << " " << x;
  std::cout << "\n";
  out_seeds(std::cout, seeds);
  std::cout << "\n" << count_true << " " << FloatingPoint::Wrap(float80(count_true) / N) << "\n";
  std::cout << float80(N) << " " << FloatingPoint::Wrap(float80(x) / size) << "\n";

}

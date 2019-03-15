// Oliver Kullmann, 7.3.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Timing of class Bernoulli

Example (annotation on next line):

Random> ./TimingBernoulli 1e9 3 1
# number N of calls of the generator, exponent e of denominator, nominator x (these are the default values)
0.1.1 8.3.2019 TimingBernoulli d395512d19fa6b0c6255336d91a7d9de2cf1a30f
# name, version, last change-date, git ID
g++ 8.3.0 Mar  8 2019 10:32:01
# compiler-version, compilation-date
-Ofast -DNDEBUG -march=native -fwhole-program -static -fno-finite-math-only -fno-unsafe-math-optimizations -fno-associative-math -fno-reciprocal-math  -fno-signed-zeros -fno-math-errno -fno-trapping-math
# optimisation options
1000000000 3 1: 124986269 0.124986269
# the arguments plus the count of results "true" and their relative frequency
1e+09 0.125
# N again, in float80-precision, and x/2^e.


Results:

On csltok:

Random> time ./TimingBernoulli
0.1.1 8.3.2019 TimingBernoulli b75233f6759bb64e8a240f382ea3e63ac1b303b7
g++ 8.3.0 Mar  8 2019 11:23:25
-Ofast -DNDEBUG -march=native -fwhole-program -static -fno-finite-math-only -fno-unsafe-math-optimizations -fno-associative-math -fno-reciprocal-math  -fno-signed-zeros -fno-math-errno -fno-trapping-math
1000000000 3 1: 124986269 0.124986269
1e+09 0.125

real    0m5.370s
user    0m5.353s
sys     0m0.001s

Roughly 187e6 generations per sec, 93% of the bernoulli-speed.

It seems the simpler options given by
Random> make Optimisation_options="-Ofast -DNDEBUG -march=native -static" numerics_options="-fno-finite-math-only" TimingBernoulli12
produce the same results.


On cs-wsok:

Random> time ./TimingBernoulli
0.1.1 8.3.2019 TimingBernoulli b75233f6759bb64e8a240f382ea3e63ac1b303b7
g++ 8.3.0 Mar  8 2019 11:18:48
-Ofast -DNDEBUG -march=native -fwhole-program -static -fno-finite-math-only -fno-unsafe-math-optimizations -fno-associative-math -fno-reciprocal-math  -fno-signed-zeros -fno-math-errno -fno-trapping-math
1000000000 3 1: 124986269 0.124986269
1e+09 0.125
real    0m3.830s
user    0m3.826s
sys     0m0.001s

Roughly 261e6 generations per sec, 91% of the bernoulli-speed.

Here it actually seems that
Random> make Optimisation_options="-Ofast -DNDEBUG -march=native -static" numerics_options="-fno-finite-math-only" TimingBernoulli12
yields a somewhat slower exectuble:
csoliver@cs-wsok:~/OKplatform/OKsystem/OKlib/Satisfiability/Transformers/Generators/Random> time ./TimingBernoulli
0.1.1 8.3.2019 TimingBernoulli eaca56ce83ef28df55e9326f2dc7c15e10c7e9bd
g++ 8.3.0 Mar 14 2019 19:32:54
-Ofast -DNDEBUG -march=native -static -fno-finite-math-only
1000000000 3 1: 124986269 0.124986269
1e+09 0.125
real    0m3.953s
user    0m3.949s
sys     0m0.001s

and here actually "-fwhole-program" seems helpful:
csoliver@cs-wsok:~/OKplatform/OKsystem/OKlib/Satisfiability/Transformers/Generators/Random> time ./TimingBernoulli
0.1.1 8.3.2019 TimingBernoulli eaca56ce83ef28df55e9326f2dc7c15e10c7e9bd
g++ 8.3.0 Mar 14 2019 19:34:44
-Ofast -DNDEBUG -march=native -fwhole-program -static -fno-finite-math-only
1000000000 3 1: 124986269 0.124986269
1e+09 0.125
real    0m3.825s
user    0m3.821s
sys     0m0.001s


On csverify:

csoliver@csverify:~/oklibrary/Satisfiability/Transformers/Generators/Random$ time ./TimingBernoulli
0.1.1 8.3.2019 TimingBernoulli b75233f6759bb64e8a240f382ea3e63ac1b303b7
g++ 8.2.0 Mar  8 2019 11:21:36
-Ofast -DNDEBUG -march=native -fwhole-program -static -fno-finite-math-only -fno-unsafe-math-optimizations -fno-associative-math -fno-reciprocal-math  -fno-signed-zeros -fno-math-errno -fno-trapping-math
1000000000 3 1: 124986269 0.124986269
1e+09 0.125
real    0m2.360s
user    0m2.360s
sys     0m0.000s

Roughly 424e6 generations per sec, which is 81% of the bernoulli-speed.
A surprising drop.

Here it seems that
Random$ make Optimisation_options="-Ofast -DNDEBUG -march=native -static" numerics_options="-fno-finite-math-only" TimingBernoulli12
has the same effect.

*/

#include <iostream>

#include <Numerics/FloatingPoint.hpp>

#include <cassert>

#include "Numbers.hpp"
#include "Environment.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.4",
        "15.3.2019",
        __FILE__};

  using namespace RandGen;

  constexpr gen_uint_t N_default = 1e9L;
  constexpr gen_uint_t e_default = 3;

}

int main(const int argc, const char* const argv[]) {

  const gen_uint_t N = (argc == 1) ? N_default : FloatingPoint::toUInt(argv[1]);
  const gen_uint_t e = (argc <= 2) ? e_default : FloatingPoint::toUInt(argv[2]);
  const gen_uint_t size = FloatingPoint::exp2(e);
  const gen_uint_t x = (argc <= 3) ? 1 : FloatingPoint::toUInt(argv[3]);
  assert(x <= size);


  gen_uint_t count_true = 0;
  randgen_t g;
  Bernoulli b(g,x,e);
  for (gen_uint_t i = 0; i < N; ++i) count_true += b();


  std::cout << proginfo;

  using FloatingPoint::float80;
  std::cout << N << " " << e << " " << x << ": " << count_true << " " << FloatingPoint::Wrap(float80(count_true) / N) << "\n";
  std::cout << float80(N) << " " << float80(x) / size << "\n";

}

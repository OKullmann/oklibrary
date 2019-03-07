// Oliver Kullmann, 4.3.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Timing of discard-function for Mersenne-Twister

Example (annotation on next line)

Random> time ./TimingDiscard 1e7 1e3
# number of discarded states and number of rounds (these are the default values)
TimingDiscard 0.1.0 4.3.2019 88d743a28de5bf0d1840e4fa83bc88ab2295488f
# name, version, last change-date, git ID
g++ 8.3.0 Mar  4 2019 17:25:19
# compiler-version, compilation-date
-Ofast -DNDEBUG -march=native -fwhole-program -static -fno-finite-math-only -fno-unsafe-math-optimizations -fno-associative-math -fno-reciprocal-math  -fno-signed-zeros -fno-math-errno -fno-trapping-math
# optimisation options
10000000 1000 6991338432609355100
# both arguments plus the output of the generator at the end
1e+10
# The product of discards and rounds, in float80-precision.

real    0m13.112s
user    0m13.077s
sys     0m0.002s

Results:

On csltok:

Random> time ./TimingDiscard
TimingDiscard 0.1.0 4.3.2019 0d66287ff89672b2621ff04b0b504d7e6bc53f76
g++ 8.3.0 Mar  4 2019 17:35:51
-Ofast -DNDEBUG -march=native -fwhole-program -static -fno-finite-math-only -fno-unsafe-math-optimizations -fno-associative-math -fno-reciprocal-math  -fno-signed-zeros -fno-math-errno -fno-trapping-math
10000000 1000 6991338432609355100
1e+10
real    0m13.623s
user    0m13.570s
sys     0m0.008s

Random> time ./TimingDiscard 1e10 1e0
TimingDiscard 0.1.0 4.3.2019 0d66287ff89672b2621ff04b0b504d7e6bc53f76
g++ 8.3.0 Mar  4 2019 17:35:51
-Ofast -DNDEBUG -march=native -fwhole-program -static -fno-finite-math-only -fno-unsafe-math-optimizations -fno-associative-math -fno-reciprocal-math  -fno-signed-zeros -fno-math-errno -fno-trapping-math
10000000000 1 6991338432609355100
1e+10
real    0m12.933s
user    0m12.889s
sys     0m0.011s

Random> time ./TimingDiscard 1e0 1e10
TimingDiscard 0.1.0 4.3.2019 0d66287ff89672b2621ff04b0b504d7e6bc53f76
g++ 8.3.0 Mar  4 2019 17:35:51
-Ofast -DNDEBUG -march=native -fwhole-program -static -fno-finite-math-only -fno-unsafe-math-optimizations -fno-associative-math -fno-reciprocal-math  -fno-signed-zeros -fno-math-errno -fno-trapping-math
1 10000000000 6991338432609355100
1e+10
real    0m24.427s
user    0m24.334s
sys     0m0.026s

Remark: on csltok the running times are quite variable; in reality the two
first times seem very close.

So roughly 770e6 discards per sec, which is 3.4x faster than generation.


On cs-wsok:

Random> time ./TimingDiscard
TimingDiscard 0.1.0 4.3.2019 0d66287ff89672b2621ff04b0b504d7e6bc53f76
g++ 8.3.0 Mar  4 2019 17:33:18
-Ofast -DNDEBUG -march=native -fwhole-program -static -fno-finite-math-only -fno-unsafe-math-optimizations -fno-associative-math -fno-reciprocal-math  -fno-signed-zeros -fno-math-errno -fno-trapping-math
10000000 1000 6991338432609355100
1e+10
real    0m8.917s
user    0m8.884s
sys     0m0.006s

Random> time ./TimingDiscard 1e10 1e0
TimingDiscard 0.1.0 4.3.2019 0d66287ff89672b2621ff04b0b504d7e6bc53f76
g++ 8.3.0 Mar  4 2019 17:33:18
-Ofast -DNDEBUG -march=native -fwhole-program -static -fno-finite-math-only -fno-unsafe-math-optimizations -fno-associative-math -fno-reciprocal-math  -fno-signed-zeros -fno-math-errno -fno-trapping-math
10000000000 1 6991338432609355100
1e+10
real    0m8.943s
user    0m8.937s
sys     0m0.001s

Random> time ./TimingDiscard 1e0 1e10
TimingDiscard 0.1.0 4.3.2019 0d66287ff89672b2621ff04b0b504d7e6bc53f76
g++ 8.3.0 Mar  4 2019 17:33:18
-Ofast -DNDEBUG -march=native -fwhole-program -static -fno-finite-math-only -fno-unsafe-math-optimizations -fno-associative-math -fno-reciprocal-math  -fno-signed-zeros -fno-math-errno -fno-trapping-math
1 10000000000 6991338432609355100
1e+10
real    0m17.821s
user    0m17.804s
sys     0m0.005s

So roughly 1.12e9 discards per sec, which is 3.5x faster than generation.


On csverify:

Random$ time ./TimingDiscard
TimingDiscard 0.1.0 4.3.2019 0d66287ff89672b2621ff04b0b504d7e6bc53f76
g++ 8.2.0 Mar  4 2019 17:37:35
-Ofast -DNDEBUG -march=native -fwhole-program -static -fno-finite-math-only -fno-unsafe-math-optimizations -fno-associative-math -fno-reciprocal-math  -fno-signed-zeros -fno-math-errno -fno-trapping-math
10000000 1000 6991338432609355100
1e+10

real    0m3.519s
user    0m3.515s
sys     0m0.004s

Random$ time ./TimingDiscard 1e10 1e0
TimingDiscard 0.1.0 4.3.2019 0d66287ff89672b2621ff04b0b504d7e6bc53f76
g++ 8.2.0 Mar  4 2019 17:37:35
-Ofast -DNDEBUG -march=native -fwhole-program -static -fno-finite-math-only -fno-unsafe-math-optimizations -fno-associative-math -fno-reciprocal-math  -fno-signed-zeros -fno-math-errno -fno-trapping-math
10000000000 1 6991338432609355100
1e+10
real    0m3.398s
user    0m3.398s
sys     0m0.000s

Random$ time ./TimingDiscard 1e0 1e10
TimingDiscard 0.1.0 4.3.2019 0d66287ff89672b2621ff04b0b504d7e6bc53f76
g++ 8.2.0 Mar  4 2019 17:37:35
-Ofast -DNDEBUG -march=native -fwhole-program -static -fno-finite-math-only -fno-unsafe-math-optimizations -fno-associative-math -fno-reciprocal-math  -fno-signed-zeros -fno-math-errno -fno-trapping-math
1 10000000000 6991338432609355100
1e+10
real    0m11.211s
user    0m11.211s
sys     0m0.000s

So roughly 2.94e9 discards per sec, which is 5.2x faster than generation.
Interesting that here the factor is considerably bigger than on the other
two machines.

*/

#include <string>
#include <iostream>

#include <Numerics/FloatingPoint.hpp>

#include "Numbers.hpp"
#include "Environment.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.3",
        "7.3.2019",
        "TimingDiscard"};

  using namespace RandGen;

  constexpr gen_uint_t discard_default = 1e7L;
  constexpr gen_uint_t rounds_default = 1000;

}

int main(const int argc, const char* const argv[]) {

  const gen_uint_t discard = (argc == 1) ? discard_default : FloatingPoint::toUInt(argv[1]);
  const gen_uint_t rounds = (argc <= 2) ? rounds_default : FloatingPoint::toUInt(argv[2]);


  randgen_t g;
  for (gen_uint_t i = 0; i < rounds; ++i) g.discard(discard);


  std::cout << proginfo;

  std::cout << discard << " " << rounds << " " << g() << "\n";

  std::cout << FloatingPoint::float80(discard) * rounds << "\n";

}

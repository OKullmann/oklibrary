// Oliver Kullmann, 4.3.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Timing of discard-function for Mersenne-Twister

Example (annotation on next line)

Random> ./TimingDiscard 1e7 1e3
# number of discarded states and number of rounds (default values)
10000000 1000 2026949688120583636
# both arguments plus the output of the generator at the end
1e+10
# The product of discards and rounds, in float80-precision.


Results:

On csltok:

Random> ./RunTime ./TimingDiscard
program name:       TimingDiscard
 version:           0.2.4
 last change:       19.3.2019
 git-id:            8faf8629885596dfc2342df79985d6e30898160c
machine name:       csltok.swansea.ac.uk
 bogomips:          4788.21
compiler version:   g++ 8.3.0
 date:              Mar_19_2019 19:05:29
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only
Output program:
10000000 1000 2026949688120583636
1e+10
MIN + MAX user times:
12.86 13.23

Random> ./RunTime ./TimingDiscard 1e10 1e0
program name:       TimingDiscard
 version:           0.2.4
 last change:       19.3.2019
 git-id:            8faf8629885596dfc2342df79985d6e30898160c
machine name:       csltok.swansea.ac.uk
 bogomips:          4788.21
compiler version:   g++ 8.3.0
 date:              Mar_19_2019 19:05:29
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only
Output program:
10000000000 1 2026949688120583636
1e+10
MIN + MAX user times:
12.80 13.22

Random> ./RunTime ./TimingDiscard 1e0 1e10
program name:       TimingDiscard
 version:           0.2.4
 last change:       19.3.2019
 git-id:            8faf8629885596dfc2342df79985d6e30898160c
machine name:       csltok.swansea.ac.uk
 bogomips:          4788.21
compiler version:   g++ 8.3.0
 date:              Mar_19_2019 19:05:29
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only
Output program:
1 10000000000 2026949688120583636
1e+10
MIN + MAX user times:
24.33 25.58

So roughly 1e10 / 12.80 ~ 781e6 discards per sec, which is
781 / 225 ~ 3.5 x faster than generation.


On cs-wsok:

Random> ./RunTime ./TimingDiscard
program name:       TimingDiscard
 version:           0.2.4
 last change:       19.3.2019
 git-id:            8faf8629885596dfc2342df79985d6e30898160c
machine name:       cs-wsok
 bogomips:          5986.74
compiler version:   g++ 8.3.0
 date:              Mar_19_2019 19:13:18
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only
Output program:
10000000 1000 2026949688120583636
1e+10
MIN + MAX user times:
8.73 9.00

Random> ./RunTime ./TimingDiscard 1e10 1e0
program name:       TimingDiscard
 version:           0.2.4
 last change:       19.3.2019
 git-id:            8faf8629885596dfc2342df79985d6e30898160c
machine name:       cs-wsok
 bogomips:          5986.74
compiler version:   g++ 8.3.0
 date:              Mar_19_2019 19:13:18
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only
Output program:
10000000000 1 2026949688120583636
1e+10
MIN + MAX user times:
8.69 8.97

Random> ./RunTime ./TimingDiscard 1e0 1e10
program name:       TimingDiscard
 version:           0.2.4
 last change:       19.3.2019
 git-id:            8faf8629885596dfc2342df79985d6e30898160c
machine name:       cs-wsok
 bogomips:          5986.74
compiler version:   g++ 8.3.0
 date:              Mar_19_2019 19:13:18
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only
Output program:
1 10000000000 2026949688120583636
1e+10
MIN + MAX user times:
17.50 17.77

So roughly 1e10 / 8.69 ~ 1.15e9 discards per sec, which is
1150 / 317 ~ 3.6 x faster than generation.


On csverify:

Random$ ./RunTime ./TimingDiscard
program name:       TimingDiscard
 version:           0.2.4
 last change:       19.3.2019
 git-id:            8faf8629885596dfc2342df79985d6e30898160c
machine name:       csverify
 bogomips:          7183.87
compiler version:   g++ 8.2.0
 date:              Mar_19_2019 19:21:09
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only
Output program:
10000000 1000 2026949688120583636
1e+10
MIN + MAX user times:
3.38 3.55

Random$ ./RunTime ./TimingDiscard 1e10 1e0
program name:       TimingDiscard
 version:           0.2.4
 last change:       19.3.2019
 git-id:            8faf8629885596dfc2342df79985d6e30898160c
machine name:       csverify
 bogomips:          7183.87
compiler version:   g++ 8.2.0
 date:              Mar_19_2019 19:21:09
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only
Output program:
10000000000 1 2026949688120583636
1e+10
MIN + MAX user times:
3.31 3.59

Random$ ./RunTime ./TimingDiscard 1e0 1e10
program name:       TimingDiscard
 version:           0.2.4
 last change:       19.3.2019
 git-id:            8faf8629885596dfc2342df79985d6e30898160c
machine name:       csverify
 bogomips:          7183.87
compiler version:   g++ 8.2.0
 date:              Mar_19_2019 19:21:09
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only
Output program:
1 10000000000 2026949688120583636
1e+10
MIN + MAX user times:
11.00 11.34

So roughly 1e10 / 3.31 ~ 3.02e9 discards per sec, which is
3310 / 568 ~ 5.83 x faster than generation.

Interesting that here the factor is considerably bigger than on the other
two machines.

*/

#include <string>
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
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/TimingDiscard.cpp",
        "GPL v3"};

  using namespace RandGen;

  constexpr gen_uint_t discard_default = 1e7L;
  constexpr gen_uint_t rounds_default = 1000;

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  const gen_uint_t discard = (argc == 1) ? discard_default : FloatingPoint::toUInt(argv[1]);
  const gen_uint_t rounds = (argc <= 2) ? rounds_default : FloatingPoint::toUInt(argv[2]);


  RandGen_t g;
  for (gen_uint_t i = 0; i < rounds; ++i) g.discard(discard);


  std::cout << discard << " " << rounds << " " << g() << "\n";

  std::cout << FloatingPoint::float80(discard) * rounds << "\n";

}

// Oliver Kullmann, 16.3.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Timing of various forms of ilogp2

Example (annotation on next line)

Random> ./Timingilogp2 1e7 1
# number of rounds and index of algorithm (default values)
10000000 1 20160000000
# the arguments plus the sum of all logarithms
1e+07
# The argument in float80-precision.


Results:

On csltok:

Random> ./RunTime ./Timingilogp2 1e7 1
program name:       Timingilogp2
 version:           0.1.2
 last change:       19.3.2019
 git-id:            71a5cd17b56fe3930fa17f6bc3d420088092f290
machine name:       csltok.swansea.ac.uk
 bogomips:          4788.21
compiler version:   g++ 8.3.0
 date:              Mar_19_2019 22:27:09
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only
Output program:
10000000 1 20160000000
1e+07
MIN + MAX user times:
3.44 3.51

A slowdown to the previous version by 3.44 / 3.01 ~ 114%.

Random> ./RunTime ./Timingilogp2 1e7 2
program name:       Timingilogp2
 version:           0.1.2
 last change:       19.3.2019
 git-id:            71a5cd17b56fe3930fa17f6bc3d420088092f290
machine name:       csltok.swansea.ac.uk
 bogomips:          4788.21
compiler version:   g++ 8.3.0
 date:              Mar_19_2019 22:27:09
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only
Output program:
10000000 2 20160000000
1e+07
MIN + MAX user times:
1.61 1.67

Random> ./RunTime ./Timingilogp2 1e7 3
program name:       Timingilogp2
 version:           0.1.2
 last change:       19.3.2019
 git-id:            71a5cd17b56fe3930fa17f6bc3d420088092f290
machine name:       csltok.swansea.ac.uk
 bogomips:          4788.21
compiler version:   g++ 8.3.0
 date:              Mar_19_2019 22:27:09
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only
Output program:
10000000 3 20160000000
1e+07
MIN + MAX user times:
22.04 22.57

A slowdown to the previous version by 22.04 / 21.36 ~ 103%.

Random> ./RunTime ./Timingilogp2 1e7 4
program name:       Timingilogp2
 version:           0.1.1
 last change:       19.3.2019
 git-id:            ab7c0e5bddce380d43b683649f90df701d1a3a64
machine name:       csltok.swansea.ac.uk
 bogomips:          4788.21
compiler version:   g++ 8.3.0
 date:              Mar_19_2019 21:02:01
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only
Output program:
Timingilogp2 0.1.1 19.3.2019 ab7c0e5bddce380d43b683649f90df701d1a3a64
csltok.swansea.ac.uk 4788.21
g++ 8.3.0 Mar_19_2019 21:02:01
--std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only
10000000 4 20160000000
1e+07
MIN + MAX user times:
19.12 19.39

So Algorithm 2 is fastest.

Indeed, when just having Algorithm 2, then a run-time of 0.7s can be achieved,
more than twice as fast as above (unfortunately a common mis-compilation).


On cs-wsok:

Random> ./RunTime ./Timingilogp2 1e7 1
program name:       Timingilogp2
 version:           0.1.2
 last change:       19.3.2019
 git-id:            e6443576a3391e7b6db909f2782b9d1b2800bfad
machine name:       cs-wsok
 bogomips:          5986.74
compiler version:   g++ 8.3.0
 date:              Mar_19_2019 22:35:53
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only
Output program:
10000000 1 20160000000
1e+07
MIN + MAX user times:
2.81 2.84

Random> ./RunTime ./Timingilogp2 1e7 2
program name:       Timingilogp2
 version:           0.1.2
 last change:       19.3.2019
 git-id:            e6443576a3391e7b6db909f2782b9d1b2800bfad
machine name:       cs-wsok
 bogomips:          5986.74
compiler version:   g++ 8.3.0
 date:              Mar_19_2019 22:35:53
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only
Output program:
10000000 2 20160000000
1e+07
MIN + MAX user times:
1.11 1.12

Random> ./RunTime ./Timingilogp2 1e7 3
program name:       Timingilogp2
 version:           0.1.2
 last change:       19.3.2019
 git-id:            e6443576a3391e7b6db909f2782b9d1b2800bfad
machine name:       cs-wsok
 bogomips:          5986.74
compiler version:   g++ 8.3.0
 date:              Mar_19_2019 22:35:53
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only
Output program:
10000000 3 20160000000
1e+07
MIN + MAX user times:
15.37 15.58

Random> ./RunTime ./Timingilogp2 1e7 4
program name:       Timingilogp2
 version:           0.1.2
 last change:       19.3.2019
 git-id:            e6443576a3391e7b6db909f2782b9d1b2800bfad
machine name:       cs-wsok
 bogomips:          5986.74
compiler version:   g++ 8.3.0
 date:              Mar_19_2019 22:35:53
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only
Output program:
10000000 4 20160000000
1e+07
MIN + MAX user times:
5.17 5.21

A slowdown to the previous version by 5.17 / 4.57 ~ 113%.

Algorithm 4 is surprisingly fast here.


On csverify:

Random$ ./RunTime ./Timingilogp2 1e7 1
rogram name:       Timingilogp2
 version:           0.1.2
 last change:       19.3.2019
 git-id:            e6443576a3391e7b6db909f2782b9d1b2800bfad
machine name:       csverify
 bogomips:          7183.87
compiler version:   g++ 8.2.0
 date:              Mar_19_2019 22:36:43
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only
Output program:
10000000 1 20160000000
1e+07
MIN + MAX user times:
2.36 2.39

Random$ ./RunTime ./Timingilogp2 1e7 2
program name:       Timingilogp2
 version:           0.1.2
 last change:       19.3.2019
 git-id:            e6443576a3391e7b6db909f2782b9d1b2800bfad
machine name:       csverify
 bogomips:          7183.87
compiler version:   g++ 8.2.0
 date:              Mar_19_2019 22:36:43
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only
Output program:
10000000 2 20160000000
1e+07
MIN + MAX user times:
0.63 0.67

A slowdown to the previous version by 0.63 / 0.61 ~ 103%.

Random$ ./RunTime ./Timingilogp2 1e7 3
program name:       Timingilogp2
 version:           0.1.2
 last change:       19.3.2019
 git-id:            e6443576a3391e7b6db909f2782b9d1b2800bfad
machine name:       csverify
 bogomips:          7183.87
compiler version:   g++ 8.2.0
 date:              Mar_19_2019 22:36:43
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only
Output program:
10000000 3 20160000000
1e+07
MIN + MAX user times:
8.64 8.68

A speedup to the previous version by 12.10 / 8.64 ~ 140%.

Random$ ./RunTime ./Timingilogp2 1e7 4
program name:       Timingilogp2
 version:           0.1.2
 last change:       19.3.2019
 git-id:            e6443576a3391e7b6db909f2782b9d1b2800bfad
machine name:       csverify
 bogomips:          7183.87
compiler version:   g++ 8.2.0
 date:              Mar_19_2019 22:36:43
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only
Output program:
10000000 4 20160000000
1e+07
MIN + MAX user times:
3.48 3.62

A slowdown to the previous version by 3.48 / 3.35 ~ 104%.


Strange that on csltok Algorithm 4 is so much slower -- there should be no
hardware-dependency here?

*/

#include <string>
#include <iostream>
#include <array>
#include <bitset>
#include <algorithm>

#include <Numerics/FloatingPoint.hpp>
#include <ProgramOptions/Environment.hpp>

#include "Numbers.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.3",
        "23.3.2019",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/Timingilogp2.cpp",
        "GPL v3"};

  using namespace RandGen;

  constexpr gen_uint_t rounds_default = 1e7L;

  typedef std::array<gen_uint_t,64> array_t;
  constexpr array_t fill_powers() {
    array_t a{};
    for (int i = 0; i < 64; ++i) a[i] = iexp2(i);
    return a;
  }
  constexpr array_t powers{fill_powers()};

  enum class T {a1=1, a2=2, a3=3, a4=4};
  constexpr T choice_default = T::a1;
  T read_choice(const std::string& s) {
    if (s == "2") return T::a2;
    else if (s == "3") return T::a3;
    else if (s == "4") return T::a4;
    else return choice_default;
  }


  inline constexpr gen_uint_t ilogp2_1(const gen_uint_t x) noexcept {
    return std::ilogb(x);
  }
  // Unfortunately, std::bitset::count is not constexpr:
  inline gen_uint_t ilogp2_2(const gen_uint_t x) noexcept {
    return std::bitset<64>(x-1).count();
  }
  inline constexpr gen_uint_t ilogp2_3(gen_uint_t x) noexcept {
    unsigned res = 0;
    for (; x != 0; x >>= 1, ++res);
    return res-1;
  }
  inline gen_uint_t ilogp2_4(const gen_uint_t x) noexcept {
    return std::lower_bound(powers.begin(), powers.end(), x) - powers.begin();
  }

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  const gen_uint_t rounds = (argc == 1) ? rounds_default : FloatingPoint::toUInt(argv[1]);
  gen_uint_t correct_sum = rounds * (63 * 64)/2;
  const T choice = (argc <= 2) ? choice_default : read_choice(argv[2]);


  gen_uint_t sum = 0;
  for (gen_uint_t i = 0; i < rounds; ++i)
    for (int j = 0; j < 64; ++j)
      switch(choice) {
      case T::a1 : sum += ilogp2_1(powers[j]); break;
      case T::a2 : sum += ilogp2_2(powers[j]); break;
      case T::a3 : sum += ilogp2_3(powers[j]); break;
      case T::a4 : sum += ilogp2_4(powers[j]); break;
      }
  if (sum != correct_sum) {
    std::cerr << "ERROR: sum=" << sum << " # " << correct_sum << "\n";
    return 1;
  }


  std::cout << rounds << " " << int(choice) << " " << sum << "\n";

  std::cout << FloatingPoint::float80(rounds) << "\n";

}

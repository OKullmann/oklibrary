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
Timingilogp2 0.0.7 16.3.2019 1d449e267a5f3085b9d6cffc0655ee6280fb3c94
csltok.swansea.ac.uk 4788.21
g++ 8.3.0 Mar_16_2019 18:56:37
--std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static -fno-finite-math-only
# usual program-information
10000000 1 20160000000
# the arguments plus the sum of all logarithms
1e+07
# The argument in float80-precision.


Results:

On csltok:



On cs-wsok:



On csverify:


*/

#include <string>
#include <iostream>
#include <array>
#include <bitset>
#include <algorithm>

#include <Numerics/FloatingPoint.hpp>

#include "Numbers.hpp"
#include "Environment.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.0.7",
        "16.3.2019",
        __FILE__};

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


  std::cout << proginfo;

  std::cout << rounds << " " << int(choice) << " " << sum << "\n";

  std::cout << FloatingPoint::float80(rounds) << "\n";

}

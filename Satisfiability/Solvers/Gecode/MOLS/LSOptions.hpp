// Oliver Kullmann, 23.4.2024 (Swansea)
/* Copyright 2024 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Options for latin squares
*/

#ifndef LSOPTIONS_JUT1BBcWg02
#define LSOPTIONS_JUT1BBcWg02

#include <ProgramOptions/Environment.hpp>

namespace LSOptions {

  // Square rotations:
  enum class SR {
    t = 0, // transposition
    at = 1, // antitransposition
    d = 2, // moving diagonals to rows
    ad = 3, // moving antidiagonals to rows
    n2 = 4, // (i,j) -> (i,-j)
    sd = 5, // (i,j) -> (i+j,j-i)
    sh = 6, // shift
    sc = 7, // scaling
  };
  constexpr int SRsize = int(SR::sc) + 1;
  unsigned args(const SR sr) noexcept {
    // using enum SR; not with g++ 10.5.0
    if (sr == SR::sh) return 2;
    else if (sr == SR::sc) return 1;
    else return 0;
  }
}
namespace Environment {
  template <> struct RegistrationPolicies<LSOptions::SR> {
    static constexpr const char* sname = "sr";
    static constexpr int size = LSOptions::SRsize;
    static constexpr std::array<const char*, size>
      string {"t", "at", "d", "ad", "n2", "sd", "sh", "sc"};
    static constexpr std::array<const char*, size>
      estring {"transposition", "antitransposition",
        "diags2rows", "antidiags2rows",
        "negate-j", "sum-difference", "shift", "scaling"};
  };
}
namespace LSOptions {
  std::ostream& operator <<(std::ostream& out, const SR sr) {
    return out << Environment::W2(sr);
  }
}

#endif

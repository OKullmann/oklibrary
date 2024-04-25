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
    id = 3, // moving rows to diagonals,
    ad = 4, // moving antidiagonals to rows
    iad = 5, // moving rows to antidiagonals
  };
  constexpr int SRsize = int(SR::iad) + 1;
}
namespace Environment {
  template <> struct RegistrationPolicies<LSOptions::SR> {
    static constexpr const char* sname = "sr";
    static constexpr int size = LSOptions::SRsize;
    static constexpr std::array<const char*, size>
      string {"t", "at", "d", "id", "ad", "iad"};
    static constexpr std::array<const char*, size>
      estring {"transposition", "antitransposition",
        "diags2rows", "rows2diags",
        "antidiags2rows", "rows2antidiags"};
  };
}
namespace LSOptions {
  std::ostream& operator <<(std::ostream& out, const SR sr) {
    return out << Environment::W2(sr);
  }
}

#endif

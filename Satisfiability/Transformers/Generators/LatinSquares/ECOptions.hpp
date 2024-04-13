// Oliver Kullmann, 13.4.2024 (Swansea)
/* Copyright 2024 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  - enum class AC (none, noncyclic)

*/

#ifndef ECOPTIONS_Q1prTqOTeb
#define ECOPTIONS_Q1prTqOTeb

#include <ProgramOptions/Environment.hpp>

namespace ECOptions {

  // Additional constraints:
  enum class AC {
    none = 0,
    noncyclic = 1
  };
  constexpr int ACsize = int(AC::noncyclic) + 1;
}

namespace Environment {
  template <> struct RegistrationPolicies<ECOptions::AC> {
    static constexpr const char* sname = "ac";
    static constexpr int size = ECOptions::ACsize;
    static constexpr std::array<const char*, size>
      string {"0", "nc"};
    static constexpr std::array<const char*, size>
      estring {"none", "noncyclic"};
  };
}

namespace ECOptions{

  std::ostream& operator <<(std::ostream& out, const AC ac) {
    return out << Environment::W2(ac);
  }

}

#endif

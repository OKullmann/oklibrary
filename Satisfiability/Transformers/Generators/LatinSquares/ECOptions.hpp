// Oliver Kullmann, 13.4.2024 (Swansea)
/* Copyright 2024 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  - enum class NC (none, noncyclic)

*/

#ifndef ECOPTIONS_Q1prTqOTeb
#define ECOPTIONS_Q1prTqOTeb

#include <ProgramOptions/Environment.hpp>

namespace ECOptions {

  // Additional constraints:

  enum class NC {
    none = 0,
    noncyclic = 1
  };
  constexpr int NCsize = int(NC::noncyclic) + 1;
}

namespace Environment {
  template <> struct RegistrationPolicies<ECOptions::NC> {
    static constexpr const char* sname = "ac";
    static constexpr int size = ECOptions::NCsize;
    static constexpr std::array<const char*, size>
      string {"nc0", "nc1"};
    static constexpr std::array<const char*, size>
      estring {"none", "noncyclic"};
  };
}

namespace ECOptions{

  std::ostream& operator <<(std::ostream& out, const NC ac) {
    return out << Environment::W2(ac);
  }

}

#endif

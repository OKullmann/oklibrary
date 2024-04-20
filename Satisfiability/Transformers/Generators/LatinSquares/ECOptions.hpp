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

  // queens-cube -> conjunction of cells:
  enum class CC {
    none = 0,
    cell_conjunctions = 1
  };
  constexpr int CCsize = int(CC::cell_conjunctions) + 1;

  // cell -> disjunction of queens-cubes:
  enum class DQ {
    none = 0,
    qc_disjunctions = 1
  };
  constexpr int DQsize = int(DQ::qc_disjunctions) + 1;

  typedef std::tuple<NC,CC,DQ> full_ac_t;

}

namespace Environment {
  template <> struct RegistrationPolicies<ECOptions::NC> {
    static constexpr const char* sname = "nc";
    static constexpr int size = ECOptions::NCsize;
    static constexpr std::array<const char*, size>
      string {"nc0", "nc1"};
    static constexpr std::array<const char*, size>
      estring {"none", "noncyclic"};
  };
  template <> struct RegistrationPolicies<ECOptions::CC> {
    static constexpr const char* sname = "cc";
    static constexpr int size = ECOptions::CCsize;
    static constexpr std::array<const char*, size>
      string {"cc0", "cc1"};
    static constexpr std::array<const char*, size>
      estring {"none", "cell-conjunctions"};
  };
  template <> struct RegistrationPolicies<ECOptions::DQ> {
    static constexpr const char* sname = "dq";
    static constexpr int size = ECOptions::DQsize;
    static constexpr std::array<const char*, size>
      string {"dq0", "dq1"};
    static constexpr std::array<const char*, size>
      estring {"none", "queens-cubes-disjunctions"};
  };
}

namespace ECOptions{

  std::ostream& operator <<(std::ostream& out, const NC nc) {
    return out << Environment::W2(nc);
  }
  std::ostream& operator <<(std::ostream& out, const CC cc) {
    return out << Environment::W2(cc);
  }
  std::ostream& operator <<(std::ostream& out, const DQ dq) {
    return out << Environment::W2(dq);
  }

}

#endif

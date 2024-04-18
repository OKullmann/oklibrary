// Oliver Kullmann, 18.2.2024 (Swansea)
/* Copyright 2024 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  - enum class LT (row, column, pdiag, padiag, qdiag, qadiag)
   - rc(LT) -> bool
   - pandiagonal(LT) -> bool
   - queendiagonal(LT) -> bool

  - enum class CF (amo, alo, eo)
   - allows_type(CF) -> bool

  - enum class CT (prime, seco, secoeup)
   - has_uep(CT) -> bool

  - line_options_t = tuple<LT,CF,CT>

*/

#ifndef PQOPTIONS_DV3Tssn5k6
#define PQOPTIONS_DV3Tssn5k6

#include <tuple>
#include <ostream>

#include <ProgramOptions/Environment.hpp>

namespace PQOptions {

  // Line-type:
  enum class LT {
    row = 0,
    column = 1,
    pdiag = 2,
    padiag = 3,
    qdiag = 4,
    qadiag = 5
  };
  constexpr int LTsize = int(LT::qadiag) + 1;
  constexpr bool rc(const LT lt) noexcept {
    return lt == LT::row or lt == LT::column;
  }
  constexpr bool pandiagonal(const LT lt) noexcept {
    return lt == LT::pdiag or lt == LT::padiag;
  }
  constexpr bool queendiagonal(const LT lt) noexcept {
    return lt == LT::qdiag or lt == LT::qadiag;
  }


  // Constraint-form:
  enum class CF {
    amo = 0,
    alo = 1,
    eo = 2
  };
  constexpr int CFsize = int(CF::eo) + 1;
  constexpr bool allows_type(const CF cf) noexcept {
    return cf != CF::alo;
  }


  // Constraint-type:
  enum class CT {
    prime = 0,
    seco = 1,
    secouep = 2
  };
  constexpr int CTsize = int(CT::secouep) + 1;
  const std::string no_output = "NO OUTPUT: ";
  constexpr bool allowed(const CF cf, const CT ct) noexcept {
    return ct == CT::prime or allows_type(cf);
  }
  constexpr bool has_uep(const CT ct) noexcept {
    return ct != CT::seco;
  }


  typedef std::tuple<LT,CF,CT> line_options_t;
}

namespace Environment {
  template <> struct RegistrationPolicies<PQOptions::LT> {
    static constexpr const char* sname = "lt";
    static constexpr int size = PQOptions::LTsize;
    static constexpr std::array<const char*, size>
      string {"r", "c", "pd", "pad", "qd", "qad"};
    static constexpr std::array<const char*, size>
      estring {"row", "column",
        "pandiag", "panantidiag",
        "queendiag", "queenantidiag"};
  };
  template <> struct RegistrationPolicies<PQOptions::CF> {
    static constexpr const char* sname = "cf";
    static constexpr int size = PQOptions::CFsize;
    static constexpr std::array<const char*, size>
      string {"amo", "alo", "eo"};
    static constexpr std::array<const char*, size>
      estring {"at-most-one", "at-least-one", "exactly-one"};
  };
  template <> struct RegistrationPolicies<PQOptions::CT> {
    static constexpr const char* sname = "ct";
    static constexpr int size = PQOptions::CTsize;
    static constexpr std::array<const char*, size>
      string {"prime", "seco", "secouep"};
    static constexpr std::array<const char*, size>
      estring {"prime-implicates", "sequential-counter",
        "seq-counter-unique-extension-property"};
  };
}

namespace PQOptions {
  std::ostream& operator <<(std::ostream& out, const LT lt) {
    return out << Environment::W2(lt);
  }
  std::ostream& operator <<(std::ostream& out, const CF cf) {
    return out << Environment::W2(cf);
  }
  std::ostream& operator <<(std::ostream& out, const CT ct) {
    return out << Environment::W2(ct);
  }
}

#endif

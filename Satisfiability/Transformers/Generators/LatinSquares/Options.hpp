// Oliver Kullmann, 3.8.2020 (Swansea)
/* Copyright 2020 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#ifndef OPTIONS_3R4etWCIqM
#define OPTIONS_3R4etWCIqM

#include <ostream>

#include <ProgramOptions/Environment.hpp>

namespace Options {

  enum class SymP { reduced=0, full=1 };
  std::ostream& operator <<(std::ostream& out, const SymP opt) {
    switch (opt) {
    case SymP::reduced: return out << "reduced";
    case SymP::full: return out << "full";
    default : return out << "SymP::unknown=" << int(opt);}
  }

  enum class EAloP { inactive=-1,
                     none=0, val=1, pair=2, pairuep=3, both=4, bothuep=5 };
  inline constexpr bool has_pair(const EAloP o) {
    return o != EAloP::none and o != EAloP::val;
  }
  inline constexpr bool has_val(const EAloP o) {
    return o == EAloP::val or o == EAloP::both or o == EAloP::bothuep;
  }
  inline constexpr bool has_uep(const EAloP o) {
    return o == EAloP::pairuep or o == EAloP::bothuep;
  }
  std::ostream& operator <<(std::ostream& out, const EAloP opt) {
    switch (opt) {
    case EAloP::none: return out << "none";
    case EAloP::val: return out << "values";
    case EAloP::pair: return out << "pairs";
    case EAloP::pairuep : return out << "pairs_uep";
    case EAloP::both: return out << "values_pairs";
    case EAloP::bothuep: return out << "values_pairs_uep";
    case EAloP::inactive: return out << "inactive";
    default : return out << "EAloP::unknown=" << int(opt);}
  }

  enum class EulP { inactive=-1, full=0, positive=1, };
  std::ostream& operator <<(std::ostream& out, const EulP opt) {
    switch (opt) {
    case EulP::full: return out << "full";
    case EulP::positive: return out << "positive";
    case EulP::inactive: return out << "inactive";
    default : return out << "EulP::unknown=" << int(opt);}
  }

  enum class PrimeP { full=0, min=1 };
  std::ostream& operator <<(std::ostream& out, const PrimeP opt) {
    switch (opt) {
    case PrimeP::full: return out << "full";
    case PrimeP::min: return out << "minimal";
    default : return out << "PrimeP::unknown=" << int(opt);}
  }
}

namespace Environment {
  template <>
  struct RegistrationPolicies<Options::SymP> {
    static constexpr int size = int(Options::SymP::full) + 1;
    static constexpr std::array<const char*, size> string
      {"r", "f"};
  };
  template <>
  struct RegistrationPolicies<Options::EAloP> {
    static constexpr int size = int(Options::EAloP::bothuep) + 1;
    static constexpr std::array<const char*, size> string
      {"L0", "Lv", "Lp", "Lpu", "Lb", "Lbu"};
  };
  template <>
  struct RegistrationPolicies<Options::EulP> {
    static constexpr int size = int(Options::EulP::positive) + 1;
    static constexpr std::array<const char*, size> string
      {"fE", "pE"};
  };
  template <>
  struct RegistrationPolicies<Options::PrimeP> {
    static constexpr int size = int(Options::PrimeP::min) + 1;
    static constexpr std::array<const char*, size> string
      {"fP", "mP"};
  };
}

#endif

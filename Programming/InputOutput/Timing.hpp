// Oliver Kullmann, 2.3.2021 (Swansea)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  - CombO, ParO, SrO, OrO, HeO

  - Error

*/

#ifndef TIMING_tq2ePIcTdj
#define TIMING_tq2ePIcTdj

#include <string>
#include <tuple>
#include <iostream>

#include <ProgramOptions/Environment.hpp>

namespace Timing {

   // Codes MUST correspond to Registration
  enum class CombO {full=0, quiet=1};
  constexpr int CombOsize = 2;

  enum class ParO {show=0, noshow=1};
  constexpr int ParOsize = 2;
  enum class SrO {full=0, nohead=1, none=2};
  constexpr int SrOsize = 3;
  enum class OrO {show=0, noshow=1};
  constexpr int OrOsize = 2;
  enum class HeO {show=0, noshow=1};
  constexpr int HeOsize = 2;

  constexpr char sep = ',';
  typedef std::tuple<ParO, SrO, OrO, HeO> option_t;
}
namespace Environment {
  template <> struct RegistrationPolicies<Timing::CombO> {
    static constexpr int size = Timing::CombOsize;
    static constexpr std::array<const char*, size> string {"full", "quiet"};
  };
  template <> struct RegistrationPolicies<Timing::ParO> {
    static constexpr int size = Timing::ParOsize;
    static constexpr std::array<const char*, size> string {"+par", "-par"};
  };
  template <> struct RegistrationPolicies<Timing::SrO> {
    static constexpr int size = Timing::SrOsize;
    static constexpr std::array<const char*, size> string
      {"+sres", "sres", "-sres"};
  };
  template <> struct RegistrationPolicies<Timing::OrO> {
    static constexpr int size = Timing::OrOsize;
    static constexpr std::array<const char*, size> string {"+ores", "-ores"};
  };
  template <> struct RegistrationPolicies<Timing::HeO> {
    static constexpr int size = Timing::HeOsize;
    static constexpr std::array<const char*, size> string {"+head", "-head"};
  };
}
namespace Timing {
  std::ostream& operator <<(std::ostream& out, const CombO m) {
    switch (m) {
    case CombO::full : return out << "full-output";
    default : return out << "quiet-output";}
  }
  std::ostream& operator <<(std::ostream& out, const ParO m) {
    switch (m) {
    case ParO::show : return out << "show-params";
    default : return out << "noshow-params";}
  }
  std::ostream& operator <<(std::ostream& out, const SrO m) {
    switch (m) {
    case SrO::full : return out << "full-single-results";
    case SrO::nohead : return out << "header-only-single-results";
    default : return out << "no-single-results";}
  }
  std::ostream& operator <<(std::ostream& out, const OrO m) {
    switch (m) {
    case OrO::show : return out << "show-statistics";
    default : return out << "noshow-statistics";}
  }
  std::ostream& operator <<(std::ostream& out, const HeO m) {
    switch (m) {
    case HeO::show : return out << "show-header";
    default : return out << "noshow-header";}
  }

  using FloatingPoint::UInt_t;
  using FloatingPoint::float80;

  constexpr UInt_t N_default = 20;

  enum class Error {
    invalid_code = 1,
    invalid_N = 2,
    invalid_combination = 3,
    unknown_combination = 4,
    empty_command = 5,
    remove_out = 6,
    remove_err = 7,
    os_error = 8,
    wrong_code = 9,
    system_call = 11,
  };

  option_t read_params(const std::string& arg, const std::string& e) {
    if (arg.empty()) return {};
    const std::optional<CombO> o = Environment::read<CombO>(arg);
    if (not o) {
      std::cerr << e << "Invalid option-parameter: \"" << arg << "\".\n";
      std::exit(int(Error::invalid_combination));
    }
    switch(o.value()) {
    case CombO::full : return {};
    case CombO::quiet :
      return {ParO::noshow, SrO::none, OrO::show, HeO::noshow};
    default : {
      std::cerr << e << "Unknown option-value: \"" << o.value() << "\".\n";
      std::exit(int(Error::unknown_combination));
    }}
  }
  option_t read_params(const std::string& arg0, const std::string& arg1, const std::string& e) {
    if (arg1.empty()) return {};
    const option_t res0 = read_params(arg0, e);
    return Environment::translate<option_t>(res0)(arg1, sep);
  }

  const std::string header_sr = "i u e s p m";
  const std::string header_or = "N umin umean umax emin emean emax smin "
    "smean smax pmin pmean pmax mmin mmean mmax";

}

#endif

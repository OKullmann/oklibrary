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

  const std::string header_sr = "i u e s p m";
  const std::string header_or = "N umin umean umax emin emean emax smin "
    "smean smax pmin pmean pmax mmin mmean mmax";

  enum class Error {
    pnumber = 1,
    empty_command = 2,
    invalid_code = 3,
    invalid_N = 4,
    remove_out = 5,
    remove_err = 6,
    os_error = 7,
    wrong_code = 8,
    system_call = 9,
  };

}

#endif

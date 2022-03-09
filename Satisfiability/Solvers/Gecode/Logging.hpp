// Oleg Zaikin, 9.3.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

Logging backtracking tree data for Gecode-based branchings.

*/

#ifndef LOGGING_kb005ZOtd6
#define LOGGING_kb005ZOtd6

#include <ProgramOptions/Environment.hpp>

namespace Logging {

  using log_t = std::ostream*;

  typedef std::uint64_t count_t;
  typedef std::vector<int> values_t;

  // Level of logging information about nodes.
  // none    - no logging.
  // reduced - id, depth, branching variable, values of child branches,
  // full    - as reduced, but with states of variables before and after
  //           lookahead reduction.
  enum class LogLvlO {none=0, reduced=1, full=2};

  std::ostream& operator <<(std::ostream& out, const Logging::LogLvlO llo) {
    switch (llo) {
    case Logging::LogLvlO::reduced : return out << "reduced-logging";
    case Logging::LogLvlO::full : return out << "full-logging";
    default : return out << "no-logging";}
  }

// Class for logging tree-data.
  class TreeLog {
    log_t log;
    LogLvlO loglvl;
  public:
    TreeLog(log_t log = nullptr, const LogLvlO loglvl = LogLvlO::none) :
            log(log), loglvl(loglvl) {}

    // Add data to a log:
    void add(const count_t dpth, const count_t id, const int branchvar,
                   const values_t values) noexcept {
      assert(not values.empty());
      if (log == nullptr or loglvl == LogLvlO::none) return;
      // First write basic data:
      *log << id << " " << dpth << " " << branchvar << " ";
      for (auto& val : values) *log << val << " ";
      *log << std::endl;
      // Write states of variables if given:
      if (loglvl == LogLvlO::full) {
        // XXX
      }
    }
  };

}

namespace Environment {
  template <>
  struct RegistrationPolicies<Logging::LogLvlO> {
    static constexpr int size = int(Logging::LogLvlO::full)+1;
    static constexpr std::array<const char*, size> string
    {"nolog", "rdcdlog", "fulllog"};
  };
}

#endif

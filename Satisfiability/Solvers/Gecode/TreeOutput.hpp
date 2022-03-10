// Oleg Zaikin, 9.3.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

Output backtracking tree data for Gecode-based branchings.

For each node the following data is printed to std::ostream:
id, depth, branching variable, branching width, values of the branching
variable (each of which corresponds to a branch):
The delimiter is space.

*/

#ifndef TREEOUTPUT_kb005ZOtd6
#define TREEOUTPUT_kb005ZOtd6

#include <ProgramOptions/Environment.hpp>

namespace TreeOutput {

  using log_t = std::ostream*;

  typedef std::uint64_t count_t;
  typedef std::vector<int> values_t;

  // Level of logging information about nodes.
  // none    - no logging.
  // reduced - id, depth, branching variable, values of child branches,
  // full    - as reduced, but with states of variables before and after
  //           lookahead reduction.
  enum class LogLvlO {none=0, reduced=1, full=2};

  std::ostream& operator <<(std::ostream& out, const TreeOutput::LogLvlO llo) {
    switch (llo) {
    case TreeOutput::LogLvlO::reduced : return out << "reduced-logging";
    case TreeOutput::LogLvlO::full : return out << "full-logging";
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
    void add(const count_t id, const count_t dpth, const int branchvar,
             const values_t values) noexcept {
      assert(not values.empty());
      if (log == nullptr or loglvl == LogLvlO::none) return;
      // First write basic data:
      *log << id << " " << dpth << " " << branchvar << " " << values.size()
           << " ";
      for (count_t i=0; i<values.size(); ++i) {
        *log << values[i];
        if (values.size() > 1 and i < values.size() - 1) *log << " ";
      }
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
  struct RegistrationPolicies<TreeOutput::LogLvlO> {
    static constexpr int size = int(TreeOutput::LogLvlO::full)+1;
    static constexpr std::array<const char*, size> string
    {"nolog", "rdcdlog", "fulllog"};
  };
}

#endif

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

  using treeoutput_t = std::ostream*;

  typedef std::uint64_t count_t;
  typedef std::vector<int> values_t;

  // Types of tree output.
  // none    - no output.
  // reduced - id, depth, branching variable, values of child branches,
  // full    - as reduced, but with states of variables before and after
  //           lookahead reduction.
  enum class TreeOutputO {none=0, reduced=1, full=2};

  std::ostream& operator <<(std::ostream& out, const TreeOutput::TreeOutputO llo) {
    switch (llo) {
    case TreeOutput::TreeOutputO::reduced : return out << "reduced-logging";
    case TreeOutput::TreeOutputO::full : return out << "full-logging";
    default : return out << "no-logging";}
  }

// Class for logging tree-data.
  class TreeOutput {
    treeoutput_t out;
    TreeOutputO outlvl;
  public:
    TreeOutput(treeoutput_t out = nullptr,
               const TreeOutputO outlvl = TreeOutputO::none) :
               out(out), outlvl(outlvl) {}

    // Add data to a out:
    void add(const count_t id, const count_t dpth, const int branchvar,
             const values_t values) noexcept {
      assert(not values.empty());
      if (out == nullptr or outlvl == TreeOutputO::none) return;
      // First write basic data:
      *out << id << " " << dpth << " " << branchvar << " " << values.size()
           << " ";
      for (count_t i=0; i<values.size(); ++i) {
        *out << values[i];
        if (values.size() > 1 and i < values.size() - 1) *out << " ";
      }
      *out << std::endl;
      // Write states of variables if given:
      if (outlvl == TreeOutputO::full) {
        // XXX
      }
    }
  };

}

namespace Environment {
  template <>
  struct RegistrationPolicies<TreeOutput::TreeOutputO> {
    static constexpr int size = int(TreeOutput::TreeOutputO::full)+1;
    static constexpr std::array<const char*, size> string
    {"noout", "rdcdout", "fullout"};
  };
}

#endif

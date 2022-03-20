// Oliver Kullmann, 20.3.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Statistics on the components of the conflict-graph


EXAMPLES:

Bicliques> echo -e "p cnf 4 6\n3 0\n-4 0\n1 2 0 \n-1 -2 0\n2 3 0\n4 0\n" | ./CNF2statscg
L1 3 : 1 2 3; 1 2
1:1 2:1 3:1
L2 3 : 1 1 1; 0 1

The clause-list:
p cnf 4 6
3 0
-4 0
1 2 0
-1 -2 0
2 3 0
4 0

The "level-1 statistics" about the components:
3 connected components, min-size 1, average-size 2, max-size 3, standard
deviation 1, median 2.

Then the count of the sizes:
size 1 once, size 2 once, size 3 once.

Finally the "level-1 statistics" about the counts (of the sizes):
3 different sizes, min-count 1, average-count 1, max-count 1, standard
deviation 0, median 1.

*/

#include <iostream>

#include <ProgramOptions/Environment.hpp>

#include "DimacsTools.hpp"
#include "ConflictGraphs.hpp"
#include "GraphTraversal.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.0",
        "20.3.2022",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Bicliques/CNF2statscg.cpp",
        "GPL v3"};

  using namespace DimacsTools;
  using namespace ConflictGraphs;
  using namespace GraphTraversal;

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg << "\n\n"
    " reads a strict Dimacs-file from standard input, and prints statistics to standard output.\n"
;
    return true;
  }

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  std::cout << StatsCC(cc_by_dfs(read_strict_Dimacs(std::cin))).S;

}

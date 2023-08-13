// Oliver Kullmann, 28.2.2022 (Swansea)
/* Copyright 2022, 2023 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Translating a CNF to its conflict-graph

  The clause-indices (0-based) are used as the vertices.
  For the input the simple graph-format is used (one line per vertex,
  showing the vertex and the adjacent vertices).


EXAMPLES:

Bicliques> echo -e "p cnf 4 4\n0\n1 2 0\n-1 -2 0\n3 4 0\n" | ./CNF2cg
# 4 1 1
0
1 2
2 1
3

The header says "4 vertices, 1 edges, undirected".

Only the statistics:
Bicliques> cat data/A_131_3964_1 | CNF2cg ""
3964 157484


TODOS:

0. Likely best not to store the graph (never).
   - So computing the neighbourhood of a clause(-vertex) on its own.
   - DONE (for the moment just outputting the number of edges)
     Then we can also just computing degree-statistics (or just the
     number of edges).

1. Likely we should have the same argument-conventions as
   Graph2BCcompGraph.cpp.

2. Write application tests.

*/

#include <iostream>

#include <ProgramOptions/Environment.hpp>

#include "DimacsTools.hpp"
#include "ConflictGraphs.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.2.1",
        "13.8.2023",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Bicliques/CNF2cg.cpp",
        "GPL v3"};

  using namespace DimacsTools;
  using namespace ConflictGraphs;

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg
         << "[OPT] \n\n"
    " reads a strict Dimacs-file from standard input, and prints the conflict-graph to standard output:\n\n"
    "  - The optional arguments switches to statistics-only mode.\n\n"
;
    return true;
  }

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  if (argc > 2) {
    std::cerr << error <<
      "At most one argument"
      " needed, but " << argc-1 << " provided.\n";
    return 1;
  }

  const bool stats_only = argc==2;

  if (stats_only) {
    const auto S = conflictgraph_degree_stats(read_strict_Dimacs(std::cin));
    std::cout << S.first.N() << " " << S.second << "\n";
  }
  else
    std::cout << conflictgraph(read_strict_Dimacs(std::cin));

}

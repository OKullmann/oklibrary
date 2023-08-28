// Oliver Kullmann, 2.4.2023 (Swansea)
/* Copyright 2023 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Statistics for clause-lists

  Clause-lengths, variables, and basics of the conflict-graph


EXAMPLES:

Bicliques> cat data/A_131_3964_1 | ./CNFstats
Dimacs: 131 3964; L = 4750

Clauses L1 3964 : 1 1.19828 5; 0.54893 1
1:3392 2:412 3:120 4:26 5:14
L2 5 : 14 792.8 3392; 1461.85 120

Variables 131 131 131
  trivial-pure-singular-1singular-non1singular-nonsingular 0 0 25 25 0 106
degree L1 131 : 2 36.2595 486; 60.0192 14
2:25 4:15 6:4 8:2 10:9 12:10 14:8 16:6 18:4 20:5 22:1 24:1 26:1 28:1 36:1 38:2 44:2 48:1 52:1 54:1 56:2 60:1 64:1 72:1 76:1 78:2 80:2 82:1 84:1 86:1 96:1 102:1 104:5 108:2 110:1 114:2 116:2 120:1 188:1 306:1 486:1
L2 41 : 1 3.19512 25; 4.61638 1
...
Conflict graph 3964 157484
Degrees L1 3964 : 1 79.4571 435; 85.9679 52
...

*/


#include <ProgramOptions/Environment.hpp>

#include "DimacsTools.hpp"
#include "ConflictGraphs.hpp"
#include "GraphTraversal.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.1",
        "11.8.2023",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Bicliques/CNFstats.cpp",
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
    " reads a strict Dimacs-file from standard input, and prints statistics to standard output.\n\n"
;
    return true;
  }

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  const DimacsClauseList F = read_strict_Dimacs(std::cin);
  std::cout << "Dimacs: " << F.first.n << " " << F.first.c << "; L = "
            << num_litocc(F) << "\n" << std::endl;
  std::cout << "Clauses " << length_statistics(F) << std::endl;
  const auto occ = allocc(F);
  std::cout << VarStatistics(occ) << std::endl;
  const auto G = conflictgraph(F.first.c, occ);
  std::cout << "Conflict graph " << G.n() << " " << G.m() << std::endl;
  std::cout << "Degrees " << degree_statistics(G);
  std::cout.flush();
  std::cout << "Sizes of components " << StatsCC(cc_by_dfs(G));
}

// Oliver Kullmann, 12.5.2023 (Swansea)
/* Copyright 2023, 2025 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Computing the biclique-compatibility graph of a graph


TODOS:

1. Enable output in dimacs- or metis-format (as in GraphConversion).


EXAMPLES:

Using the separator "-":

Bicliques> ./GraphGen grid 5 1 | ./Graph2BCcompGraph "" -
# "./Graph2BCcompGraph" "" "-"
# input 5 4
# 4 3 1
1,1-2,1 2,1-3,1
2,1-3,1 1,1-2,1 3,1-4,1
3,1-4,1 2,1-3,1 4,1-5,1
4,1-5,1 3,1-4,1

Using a provided separator, and without comments:

Bicliques> ./GraphGen grid 5 1 | ./Graph2BCcompGraph -com X
# 4 3 1
1,1X2,1 2,1X3,1
2,1X3,1 1,1X2,1 3,1X4,1
3,1X4,1 2,1X3,1 4,1X5,1
4,1X5,1 3,1X4,1

Turning off the vertex-naming, and without parameters:
Bicliques> ./GraphGen grid 5 1 | ./Graph2BCcompGraph -dp ""
# "./Graph2BCcompGraph" "-dp" ""
# input 5 4
0 1
1 0 2
2 1 3
3 2

Just computing the parameters (and degree-statistics):

Bicliques> ./GraphGen grid 5 1 | ./Graph2BCcompGraph -trans ""
# "./Graph2BCcompGraph" "-trans" ""
# input 5 4
# 4 : 1 1.5 2; 0.57735
# 4 3

A larger example:
Bicliques> time cat data/A_131_3964_1 | ./CNF2cg | ./Graph2BCcompGraph -trans ""
# "./Graph2BCcompGraph" "-trans" ""
# input 3964 157484
# 157484 : 3 30001.5 83233; 27098.4
# 157484 2362378400
real	0m6.453s
user	0m6.467s
sys	0m0.019s
Directly using CNF-input:
Bicliques> time cat data/A_131_3964_1 | ./Graph2BCcompGraph -trans "" X
real	0m5.842s
user	0m5.829s
sys	0m0.016s

Using 4 threads:
Bicliques> time cat data/A_131_3964_1 | ./CNF2cg | ./Graph2BCcompGraph -trans 4
# "./Graph2BCcompGraph" "-trans" "4"
# input 3964 157484
# 157484 : 3 30001.5 83233; 27098.4
# 157484 2362378400
real	0m2.326s
user	0m7.287s
sys	0m0.026s
Directly using CNF-input:
Bicliques> time cat data/A_131_3964_1 | ./Graph2BCcompGraph -trans 4 X
real	0m1.551s
user	0m6.136s
sys	0m0.004s


Now only showing the number of edges in the conflict-graph
and the derived bc-comp-graph:
Bicliques> cat data/A_131_3964_1 | ./CNF2cg | ./Graph2BCcompGraph -trans,-com ""
# 157484 2362378400
Directly using CNF-input:
Bicliques> cat data/A_131_3964_1 | ./Graph2BCcompGraph -trans,-com "" X
# 157484 2362378400


See plans/general.txt.
*/

#include <iostream>

#include <ProgramOptions/Environment.hpp>
#include <Numerics/NumInOut.hpp>

#include "Graphs.hpp"
#include "Bicliques2SAT.hpp"
#include "Bicliques.hpp"
#include "ConflictGraphs.hpp"
#include "DimacsTools.hpp"

#include "BCC2SAT.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.4.3",
        "11.4.2025",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Bicliques/Graph2BCcompGraph.cpp",
        "GPL v3"};

  using namespace Bicliques2SAT;

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg
         << " format-options separator [input-option]\n\n"
    " format-options : " << Environment::WRP<DC>{} << "\n"
    "                : " << Environment::WRP<DP>{} << "\n"
    "                : " << Environment::WRP<CS>{} << "\n"
    " separator      : a string (possibly empty)\n"
    " input-option   : if present, CNF-input is processed\n\n"
    " reads a graph from standard input, and prints the"
    " biclique-compatibility graph to standard output:\n\n"
    "  - Arguments \"\" (the empty string) yield the default-values.\n"
    "  - The empty string as separator turns off the vertex-naming.\n"
    "  - When using \"-trans\", that is, counting-only, then sep can be"
    " the number of threads.\n\n"

;
    return true;
  }

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  if (argc != 3 and argc != 4) {
    std::cerr << error <<
      "Exactly two or three arguments (form-opt, sep, [input-opt])"
      " needed, but " << argc-1 << " provided.\n";
    return 1;
  }

  const auto formopt =
    Environment::translate<format_options_t>()(argv[1], ',');
  const auto [comments, parameters, translation] = formopt;
  const std::string sep = argv[2];
  const bool graph_input = argc == 3;

  const auto G = graph_input ? make_AdjVecUInt(std::cin, Graphs::GT::und) :
    ConflictGraphs::conflictgraph(DimacsTools::read_strict_Dimacs(std::cin));
  BCC2SAT::commandline_output(formopt, "# ", std::cout, argc, argv);
  if (comments == DC::with)
    std::cout << "# input " << G.n() << " " << G.m() << std::endl;
  if (translation == CS::with) {
    if (parameters == DP::with)
      std::cout << Bicliques::bccomp_graph<3>(G, sep);
    else {
      const auto BG = Bicliques::bccomp_graph<3>(G, sep);
      BG.output_body(std::cout);
    }
  }
  else {
    const auto [stats, E] = sep.empty() ?
      Bicliques::bccom_degree_stats<3>(G) :
      Bicliques::bccom_degree_stats_parallel<3>(G,
                                                FloatingPoint::to_UInt(sep));
    if (comments == DC::with)
      std::cout << "# " << stats << "\n";
    if (parameters == DP::with)
      std::cout << "# " << G.m() << " " << E << "\n";
  }

}

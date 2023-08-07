// Oliver Kullmann, 12.5.2023 (Swansea)
/* Copyright 2023 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Computing the biclique-compatibility graph of a graph

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

Bicliques> time cat data/A_131_3964_1 | ./CNF2cg | ./Graph2BCcompGraph -trans ""
# "./Graph2BCcompGraph" "-trans" ""
# input 3964 157484
# 157484 : 3 30001.5 83233; 27098.4
# 157484 2362378400
real	0m6.453s
user	0m6.467s
sys	0m0.019s


See plans/general.txt.
*/

#include <iostream>

#include <ProgramOptions/Environment.hpp>

#include "Graphs.hpp"
#include "Bicliques2SAT.hpp"
#include "Bicliques.hpp"

#include "BCC2SAT.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.4.0",
        "7.8.2023",
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
         << " format-options separator\n\n"
    " format-options : " << Environment::WRP<DC>{} << "\n"
    "                : " << Environment::WRP<DP>{} << "\n"
    "                : " << Environment::WRP<CS>{} << "\n"
    " separator      : a string (possibly empty)\n\n"
    " reads a graph from standard input, and prints the biclique-compatibility graph to standard output:\n\n"
    "  - Arguments \"\" (the empty string) yield the default-values.\n"
    "  - The empty string as separator turns off the vertex-naming.\n\n"

;
    return true;
  }

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  if (argc != 3) {
    std::cerr << error <<
      "Exactly two arguments (form-opt, sep)"
      " needed, but " << argc-1 << " provided.\n";
    return 1;
  }

  const auto formopt =
    Environment::translate<format_options_t>()(argv[1], ',');
  const auto [comments, parameters, translation] = formopt;
  const std::string sep = argv[2];

  const auto G = make_AdjVecUInt(std::cin, Graphs::GT::und);
  BCC2SAT::commandline_output(formopt, "# ", std::cout, argc, argv);
  if (comments == DC::with)
    std::cout << "# input " << G.n() << " " << G.m() << std::endl;
  if (translation == CS::with) {
    if (parameters == DP::with)
      std::cout << Bicliques::bccomp_graph<3>(G, sep);
    else {
      const auto BG = Bicliques::bccomp_graph<3>(G, sep);
      BG.output(std::cout);
    }
  }
  else {
    const auto [stats, E] = Bicliques::bccom_degree_stats<3>(G);
    if (comments == DC::with)
      std::cout << "# " << stats << "\n";
    if (parameters == DP::with)
      std::cout << "# " << G.m() << " " << E << "\n";
  }

}

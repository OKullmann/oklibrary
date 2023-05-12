// Oliver Kullmann, 12.5.2023 (Swansea)
/* Copyright 2023 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Computing the biclique-compatibility graph of a graph

EXAMPLES:

Using the default-separator "-":

Bicliques> ./GraphGen grid 5 1 | ./Graph2BCcompGraph
# 4 3 1
1,1-2,1 2,1-3,1
2,1-3,1 1,1-2,1 3,1-4,1
3,1-4,1 2,1-3,1 4,1-5,1
4,1-5,1 3,1-4,1

Using a provided separator:

Bicliques> ./GraphGen grid 5 1 | ./Graph2BCcompGraph "X"
# 4 3 1
1,1X2,1 2,1X3,1
2,1X3,1 1,1X2,1 3,1X4,1
3,1X4,1 2,1X3,1 4,1X5,1
4,1X5,1 3,1X4,1

Turning off the vertex-naming:
Bicliques> ./GraphGen grid 5 1 | ./Graph2BCcompGraph ""
# 4 3 1
0 1
1 0 2
2 1 3
3 2


See plans/general.txt.
*/

#include <iostream>

#include <ProgramOptions/Environment.hpp>

#include "Graphs.hpp"
#include "Bicliques.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.1",
        "12.5.2023",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Bicliques/Graph2BCcompGraph.cpp",
        "GPL v3"};

  using namespace Graphs;
  using namespace Bicliques;

  const std::string error = "ERROR[" + proginfo.prg + "]: ";
  const std::string default_sep = "-";

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg << " [sep=\"" << default_sep << "]\"\n\n"
    " reads a graph from standard input, and prints the biclique-compatibility graph to standard output:\n\n"
    "  - Arguments \"\" (the empty string) turns off the vertex-naming.\n"

;
    return true;
  }

  std::string read_sep(const int argc, const char* const argv[]) noexcept {
    if (argc == 1) return default_sep;
    else return argv[1];
  }

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  if (argc > 2) {
    std::cerr << error <<
      "At most one argument allowed, but " << argc-1 << " provided.\n";
    return 1;
  }

  const std::string sep = read_sep(argc, argv);

  std::cout << bccomp_graph_bydef(make_AdjVecUInt(std::cin, GT::und), sep);

}

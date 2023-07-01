// Oliver Kullmann, 1.7.2023 (Swansea)
/* Copyright 2023 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Transforming a graph from the library-format to matrix-format,
  via the adjacency-matrix

  Assuming an undirected graph.


EXAMPLES:

Bicliques> ./GraphGen biclique 3 4 | ./Graph2Matrix_debug
7 7

0 0 0 1 1 1 1
0 0 0 1 1 1 1
0 0 0 1 1 1 1
1 1 1 0 0 0 0
1 1 1 0 0 0 0
1 1 1 0 0 0 0
1 1 1 0 0 0 0

A cycle of lengtj 4:
Bicliques> echo -e "A B\nB C\nC D\nD A\n" | ./Graph2Matrix_debug
4 4

0 1 0 1
1 0 1 0
0 1 0 1
1 0 1 0

*/

#include <iostream>

#include <ProgramOptions/Environment.hpp>

#include "Graphs.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.0",
        "1.7.2023",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Bicliques/Graph2Matrix.cpp",
        "GPL v3"};

  using namespace Graphs;

  const std::string error = "ERROR[" + proginfo.prg + "]: ";
  const std::string default_sep = "-";

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg << "\n\n"
    " reads a graph from standard input, and prints the adjacency-matrix to standard output.\n"

;
    return true;
  }

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  if (argc != 1) {
    std::cerr << error <<
      "No argument allowed, but " << argc-1 << " provided.\n";
    return 1;
  }

  output_matrix(make_AdjVecUInt(std::cin, GT::und), std::cout);

}

// Oliver Kullmann, 16.8.2023 (Swansea)
/* Copyright 2023 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Transforming the 2-colouring-problem for a graph (or the cnflictgraph of
  a CNF) into a SAT-problem

EXAMPLES:
*/

#include <iostream>

#include <ProgramOptions/Environment.hpp>

#include "Graphs.hpp"
#include "ConflictGraphs.hpp"
#include "DimacsTools.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.0.5",
        "16.8.2023",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Bicliques/Graph2BCcompGraph.cpp",
        "GPL v3"};

  using namespace Graphs;

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg
         << " [input-option]\n\n"
    " input-option   : if present, CNF-input is processed\n\n"
    " reads a graph from standard input, and outputs the CNF-translation of its 2-colouring-problem.\n\n"
;
    return true;
  }

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  if (argc > 2) {
    std::cerr << error <<
      "At most one argument ([input-opt])"
      " needed, but " << argc-1 << " provided.\n";
    return 1;
  }

  const bool graph_input = argc == 1;

  const auto G = graph_input ? make_AdjVecUInt(std::cin, GT::und) :
    ConflictGraphs::conflictgraph(DimacsTools::read_strict_Dimacs(std::cin));
  bipart2SAT(std::cout, G);

}

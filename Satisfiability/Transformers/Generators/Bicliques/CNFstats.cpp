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

*/


#include <ProgramOptions/Environment.hpp>

#include "DimacsTools.hpp"
#include "ConflictGraphs.hpp"
#include "GraphTraversal.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.0.4",
        "2.4.2023",
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

}

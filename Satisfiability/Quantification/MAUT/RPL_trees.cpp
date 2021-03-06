// Oliver Kullmann, 28.14.2021 (Swansea)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Small trees for RPL (reverse pure literals)


TODOS:

1. Implement the algorithm for enumerating maximal independent sets
   with polynomial delay by Tsukiyama, Ide, Ariyoshi, Shirakawa:
    - See ComputerAlgebra/Graphs/Lisp/IndependentSets.mac
      for the algorithm at Maxima-level.
    - This is for graphs; for the clause-sets we use the complementation-
      structure given by the literals.

*/

#include <string>
#include <iostream>

#include "RPL_trees.hpp"

#include <ProgramOptions/Environment.hpp>

namespace {

  const Environment::ProgramInfo proginfo{
        "0.5.1",
        "2.5.2021",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Quantification/MAUT/RPL_trees.cpp",
        "GPL v3"};

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
      "> " << proginfo.prg << " [input]\n\n"
      " input     : standard input " << "\n\n"
      ;
    std::cout <<
    " computes a splitting tree and prints the tree-sizes:\n\n"
    "  - Trailing arguments can be left out, then using their default-values.\n"
    "  - Arguments \"\" (the empty string) yield also the default-values,\n"
      ;
    return true;
  }

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  const MAUT::ClauseSet F = MAUT::read(std::cin);

  typedef MAUT::FirstOpen<MAUT::COUNTTYPE> bt_t;
  bt_t B(F);
  B.solve();
  std::cout << MAUT::ClauseSet::header() << " "
            << bt_t::ret_t::header() << "\n";
  std::cout << F << " " << B. result << "\n";

}

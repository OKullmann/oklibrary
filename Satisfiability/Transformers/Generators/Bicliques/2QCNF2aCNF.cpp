// Oliver Kullmann, 20.3.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Extracting the a-part from a strict 2-QCNF


EXAMPLES:

Bicliques> echo -e "p cnf 4 6\na 3 4 0\n3 0\n-4 0\n1 2 0 \n-1 -2 0\n2 3 0\n4 0\n" | ./2QCNF2aCNF
p cnf 2 6
1 0
-2 0
0
0
1 0
2 0

TODOS:

1. Generalise to arbitrary QCNF as input:
    - Call a "global" variable a universal variable on which every existential
      variable depends (there might be no such global variables).
    - Keeping now from each clause exactly those literals
      belonging to global variables.

2. Generalise to DQCNF
    - The set of global variables now is the intersection of the
      dependencies of all existential variables.

*/

#include <iostream>

#include <ProgramOptions/Environment.hpp>

#include "DimacsTools.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.0",
        "20.3.2022",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Bicliques/2QCNF2aCNF.cpp",
        "GPL v3"};

  using namespace DimacsTools;

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg << "\n\n"
    " reads a strict 2-QDimacs-file from standard input, and prints the extracted a-part to standard output.\n"
;
    return true;
  }

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  extract_apart_strict2qcnf(std::cin, std::cout);
}

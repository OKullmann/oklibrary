// Oliver Kullmann, 15.4.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Extracting the g-part from a strict QCNF

  Call a "global" variable a universal variable on which every existential
  variable depends (there might be no such global variables).
  Now from each clause exactly those literals belonging to global variables
  are kept.
  This generalises 2QCNF2aCNF, except for the case when there are no global
  variables: here now the number of clauses output is zero.

EXAMPLE:

  QBRG "a10 e10 a10 e10 a10 10" "5*6,10" "u,o" 1

produces

p cnf 60 5
a 1 2 3 4 5 6 7 8 9 10 0
e 11 12 13 14 15 16 17 18 19 20 0
a 21 22 23 24 25 26 27 28 29 30 0
e 31 32 33 34 35 36 37 38 39 40 0
a 41 42 43 44 45 46 47 48 49 50 0
e 51 52 53 54 55 56 57 58 59 60 0
-4 -11 17 -18 -22 -28 -29 -45 -51 -58 0
-2 -8 -10 -14 17 -19 21 -42 44 51 0
12 15 19 25 -26 38 -41 -44 52 -55 0
-2 -3 8 10 -25 26 31 -38 -51 59 0
-9 -10 -14 20 -26 30 38 -43 -51 54 0

which yields

Bicliques> QBRG "a10 e10 a10 e10 a10 10" "5*6,10" "u,o" 1 | ./QCNF2gCNF
p cnf 10 5
-4 0
-2 -8 -10 0
0
-2 -3 8 10 0
-9 -10 0


TODOS:

1. Generalise to DQCNF
    - The set of global variables now is the intersection of the
      dependencies of all existential variables.


*/


#include <iostream>

#include <ProgramOptions/Environment.hpp>

#include "DimacsTools.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.0",
        "15.4.2022",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Bicliques/QCNF2aCNF.cpp",
        "GPL v3"};

  using namespace DimacsTools;

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg << "\n\n"
    " reads a strict QDimacs-file from standard input, and prints the extracted global-part to standard output.\n"
;
    return true;
  }

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  extract_gpart_strictqcnf(std::cin, std::cout);
}

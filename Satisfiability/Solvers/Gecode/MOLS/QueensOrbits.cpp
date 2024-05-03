// Oliver Kullmann, 1.6.2024 (Swansea)
/* Copyright 2024 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Expanding queens-solutions to their orbits

EXAMPLES:

N=11:
Given unique representatives, we get all queens-solutions:
MOLS> echo "0 5 10 4 9 3 8 2 7 1 6" | ./QueensOrbits_debug
0 5 10 4 9 3 8 2 7 1 6
0 8 5 2 10 7 4 1 9 6 3
0 9 7 5 3 1 10 8 6 4 2
0 7 3 10 6 2 9 5 1 8 4
0 6 1 7 2 8 3 9 4 10 5
0 3 6 9 1 4 7 10 2 5 8
0 2 4 6 8 10 1 3 5 7 9
0 4 8 1 5 9 2 6 10 3 7
Each given queens-solutions is expanded on its own:
MOLS> echo -e "0 5 10 4 9 3 8 2 7 1 6\n0 8 5 2 10 7 4 1 9 6 3" | ./QueensOrbits_debug
0 5 10 4 9 3 8 2 7 1 6
0 8 5 2 10 7 4 1 9 6 3
0 9 7 5 3 1 10 8 6 4 2
0 7 3 10 6 2 9 5 1 8 4
0 6 1 7 2 8 3 9 4 10 5
0 3 6 9 1 4 7 10 2 5 8
0 2 4 6 8 10 1 3 5 7 9
0 4 8 1 5 9 2 6 10 3 7
0 8 5 2 10 7 4 1 9 6 3
0 2 4 6 8 10 1 3 5 7 9
0 7 3 10 6 2 9 5 1 8 4
0 6 1 7 2 8 3 9 4 10 5
0 3 6 9 1 4 7 10 2 5 8
0 9 7 5 3 1 10 8 6 4 2
0 4 8 1 5 9 2 6 10 3 7
0 5 10 4 9 3 8 2 7 1 6
MOLS> echo -e "0 2 4 1 3\n0 2 4 6 1 3 5" | ./QueensOrbits_debug
0 2 4 1 3
0 3 1 4 2
0 2 4 6 1 3 5
0 5 3 1 6 4 2
0 4 1 5 2 6 3
0 3 6 2 5 1 4

QueensOrbits can act as a reverse of QueensSymmetries (modulo the order
of the output -- one orbit after the other):

MOLS> N=13; CPandiagonal $N "" | clasp 0 | CP_clasp_first_columns.awk -v N=$N -v mode=1 | Sort > IN
MOLS> wc -l IN
348 IN
MOLS> cat IN | ./QueensSymmetries_debug OUT > /dev/null
MOLS> wc -l OUT
5 OUT
MOLS> cat OUT | ./QueensOrbits | Sort > IN2
MOLS> diff IN IN2
(file-content is identical)

*/

#include <iostream>
#include <set>

#include <ProgramOptions/Environment.hpp>

#include "BasicLatinSquares.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.0.9",
        "3.5.2024",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Solvers/Gecode/MOLS/QueensOrbits.cpp",
        "GPL v3"};

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  using namespace BasicLatinSquares;

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
      "> " << proginfo.prg << " [filename] \n\n"
      "reads queens-solutions from standard input and prints their orbits"
      " to standard output.\n\n"
 ;
    return true;
  }

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  do {
    std::string line;
    std::getline(std::cin, line);
    const ls_row_t Q = FloatingPoint::to_vec_unsigned<size_t>(line, ' ');
    if (Q.empty()) return 0;
    Environment::out_line(std::cout, Q); std::cout << "\n";
    std::set<ls_row_t> seen({Q});
    for (const auto& Q2 : QOrbitS(Q))
      if (not seen.contains(Q2)) {
        seen.insert(Q2);
        Environment::out_line(std::cout, Q2); std::cout << "\n";
      }
  } while (std::cin);

}

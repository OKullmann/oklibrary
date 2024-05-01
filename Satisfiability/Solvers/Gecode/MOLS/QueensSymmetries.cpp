// Oliver Kullmann, 29.4.2024 (Swansea)
/* Copyright 2024 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Computing the equivalence classes of the solutions to the toroidal
  N-Queens problem

   - The solutions are standardised (top-left cell is occupied).
   - The symmetry group is the group of order 8*phi(N) of pandiagonal
     symmetries fixing the top-left cell (i.e., (0,0)), plus the
     queens-shifts.

EXAMPLES:

We use out standard order of queens-solutions, given by lexicographical
order of the corresponding row-cyclic solutions.

For example:

MOLS> N=5; CPandiagonal $N "" | clasp 0 | passextractpos.awk | Sort | CP_clasp_first_columns.awk -v N=$N -v mode=1
 0 3 1 4 2
 0 2 4 1 3

MOLS> for N in 5 7 11 13 17 19; do CPandiagonal $N "" | clasp 0 | passextractpos.awk | Sort | CP_clasp_first_columns.awk -v N=$N -v mode=1 | ./QueensSymmetries_debug; echo; done
5 2
# 1
2

7 4
# 1
4

11 8
# 1
8

13 348
# 5
8 104 156 78 2

17 8276
# 23
8 136 2176 272 1088 544 544 272 272 544 272 544 544 34 272 136 136 272 68 68 68 2 4

19 43184
# 23
8 1368 1368 2736 2736 304 2736 2736 1368 2736 1368 1368 2736 1368 2736 2736 1368 2736 2736 2736 2736 456 8


*/

#include <iostream>

#include <ProgramOptions/Environment.hpp>
#include <Transformers/Generators/LatinSquares/Algorithms.hpp>

#include "BasicLatinSquares.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.1",
        "1.5.2024",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Solvers/Gecode/MOLS/QueensSymmetries.cpp",
        "GPL v3"};

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  using namespace BasicLatinSquares;

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
      "> " << proginfo.prg << " [+] \n\n"
      "reads from standard input and outputs statistics:\n"
      " - \"+\" means that also queens-shifts are applied.\n\n"
 ;
    return true;
  }

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  const Algorithms::Cubing_t cubes =
    Algorithms::read_queens_cubing(std::cin);
  if (cubes.m == 0) return 0;
  std::cout << cubes.N << " " << cubes.m << std::endl;
  const auto [E, numcl] = all_qorbitsS(cubes.A);
  std::cout << "# " << numcl << "\n";
  std::vector<size_t> counts(numcl);
  for (const size_t c : E) ++counts[c-1];
  Environment::out_line(std::cout, counts);
  std::cout << "\n";
}

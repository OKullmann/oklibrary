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
     symmetries fixing the top-left cell (i.e., (0,0)).

EXAMPLES:

We use out standard order of queens-solutions, given by lexicographical
order of the corresponding row-cyclic solutions.

For example:

MOLS> N=5; CPandiagonal $N "" | clasp 0 | passextractpos.awk | Sort | CP_clasp_first_columns.awk -v N=$N -v mode=1
 0 3 1 4 2
 0 2 4 1 3

MOLS> for N in 5 7 11 13 17 19; do CPandiagonal $N "" | clasp 0 | passextractpos.awk | Sort | CP_clasp_first_columns.awk -v N=$N -v mode=1 | ./QueensSymmetries_debug; echo; done
 2
# 1
2

7 4
# 1
4

11 8
# 1
8

13 348
# 12
8 96 96 12 24 24 24 24 8 24 6 2

17 8276
# 120
8 128 128 128 128 128 128 128 128 128 128 128 128 128 128 128 128 128 8 128 128 128 128 128 64 64 128 64 32 16 32 32 64 128 128 64 128 128 64 64 128 128 32 128 128 32 128 128 32 128 64 64 64 64 32 32 32 64 128 64 64 128 128 64 64 32 128 64 64 128 128 64 64 64 32 16 32 128 64 128 32 32 32 16 32 32 32 32 64 64 32 32 32 32 32 32 32 32 32 32 32 32 32 32 32 16 16 32 32 32 32 8 2 2 4 16 8 4 4 4

19 43184
# 307
8 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 72 144 144 144 144 144 144 144 144 144 144 144 72 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 72 144 144 72 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 16 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 72 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 72 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 144 72 144 144 144 144 144 144 144 144 144 144 24 8

*/

#include <iostream>

#include <ProgramOptions/Environment.hpp>
#include <Transformers/Generators/LatinSquares/Algorithms.hpp>

#include "BasicLatinSquares.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.0.9",
        "29.4.2024",
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
      "> " << proginfo.prg << "\n\n"
      "reads from standard input and outputs statistics.\n\n"
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
  const auto [E, numcl] = all_qorbits(cubes.A);
  std::cout << "# " << numcl << "\n";
  std::vector<size_t> counts(numcl);
  for (const size_t c : E) ++counts[c-1];
  Environment::out_line(std::cout, counts);
  std::cout << "\n";
}

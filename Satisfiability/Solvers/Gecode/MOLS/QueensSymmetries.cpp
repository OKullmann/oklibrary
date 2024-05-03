// Oliver Kullmann, 29.4.2024 (Swansea)
/* Copyright 2024 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Computing the equivalence classes of the solutions to the toroidal
  N-Queens problem

   - The solutions are assumed to be standardised (top-left cell is occupied).
   - The symmetry group is the group of order 8*phi(N) of pandiagonal
     symmetries fixing the top-left cell (i.e., (0,0)), plus the N
     queens-shifts.
   - The representatives stored are the respective firsts in the given
     order.
   - Also the sizes of the equivalence-classes use that order.

EXAMPLES:

We use out standard order of queens-solutions, given by lexicographical
order of the corresponding row-cyclic solutions.

For example:

MOLS> N=5; CPandiagonal $N "" | clasp 0 | passextractpos.awk | Sort | CP_clasp_first_columns.awk -v N=$N -v mode=1
 0 3 1 4 2
 0 2 4 1 3

MOLS> for N in 5 7 11 13 17 19; do CPandiagonal $N "" | clasp 0 | passextractpos.awk | Sort | CP_clasp_first_columns.awk -v N=$N -v mode=1 | ./QueensSymmetries_debug; echo; done
5 2 160
closed 1
2
L1 1 : 2 2 2 ; 0 2
2:1
L2 1 : 1 1 1 ; 0 1

7 4 336
closed 1
4
L1 1 : 4 4 4 ; 0 4
4:1
L2 1 : 1 1 1 ; 0 1

11 8 880
closed 1
8
L1 1 : 8 8 8 ; 0 8
8:1
L2 1 : 1 1 1 ; 0 1

13 348 1248
closed 1
8 104 156 78 2
L1 5 : 2 69.6 156 ; 65.3514 78
2:1 8:1 78:1 104:1 156:1
L2 5 : 1 1 1 ; 0 1

17 8276 2176
closed 1
8 136 2176 272 1088 544 544 272 272 544 272 544 544 34 272 136 136 272 68 68 68 2 4
L1 23 : 2 359.826 2176 ; 473.622 272
2:1 4:1 8:1 34:1 68:3 136:3 272:6 544:5 1088:1 2176:1
L2 10 : 1 2.3 6 ; 1.88856 1

19 43184 2736
closed 1
8 1368 1368 2736 2736 304 2736 2736 1368 2736 1368 1368 2736 1368 2736 2736 1368 2736 2736 2736 2736 456 8
L1 23 : 8 1877.57 2736 ; 1003.36 2736
8:2 304:1 456:1 1368:7 2736:12
L2 5 : 1 4.6 12 ; 4.82701 2

Explanation of output (last example):
1. N, number of queens-solutions, number of (formal) symmetries (maximal
   size of equivalence class)
2. whether the input was closed under isomorphism ("1" is "true").
3. The sizes of the isomorphism-classes (in the order as given).
4. For the sizes the standard statistics: count : minimum, average, maximum ;
   standard deviation, median.
5. For each of the sizes their counts.
6. The standard statistics for the count-values.

With the representatives:
MOLS> N=13; CPandiagonal $N "" | clasp 0 | passextractpos.awk | Sort | CP_clasp_first_columns.awk -v N=$N -v mode=1 | ./QueensSymmetries_debug OUT
13 348 1248
closed 1
8 104 156 78 2
L1 5 : 2 69.6 156 ; 65.3514 78
2:1 8:1 78:1 104:1 156:1
L2 5 : 1 1 1 ; 0 1
MOLS> cat OUT
0 11 9 7 5 3 1 12 10 8 6 4 2
0 11 9 7 2 4 1 8 10 12 6 3 5
0 11 9 4 6 1 10 2 7 12 8 3 5
0 11 6 2 7 12 4 9 1 10 5 3 8
0 8 3 11 6 1 9 4 12 7 2 10 5

Remark: The expansion of the representatives into their orbits is obtained
by QueensOrbits.cpp.

*/

#include <iostream>

#include <ProgramOptions/Environment.hpp>
#include <Transformers/Generators/LatinSquares/Algorithms.hpp>
#include <Numerics/Statistics.hpp>

#include "BasicLatinSquares.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.2",
        "3.5.2024",
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
      "> " << proginfo.prg << " [filename] \n\n"
      "reads from standard input and outputs statistics:\n"
      " - if filename given, then representatives are stored in it.\n\n"
 ;
    return true;
  }

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  std::ofstream file = argc == 1 ? std::ofstream() : std::ofstream(argv[1]);
  if (not file) {
    assert(argc >= 2);
    std::cerr << error << "Can not open outputfile \"" << argv[1]
              << "\".\n";
    return 1;
  }

  const Algorithms::Cubing_t cubes =
    Algorithms::read_queens_cubing(std::cin);
  if (cubes.m == 0) return 0;
  std::cout << cubes.N << " " << cubes.m << " "
            << 8*phi(cubes.N)*cubes.N << std::endl;
  const auto res = all_qorbitsS(cubes.A);
  const auto& [E, numcl, closed] = res;
  std::cout << "closed " << closed << "\n";
  std::vector<size_t> counts(numcl);
  for (const size_t c : E) ++counts[c-1];
  Environment::out_line(std::cout, counts);
  std::cout << "\n";
  const GenStats::FreqStats<size_t, FloatingPoint::float80> S(counts);
  std::cout << S; std::cout.flush();
  if (file.is_open()) res.representatives(file, cubes.A);
}

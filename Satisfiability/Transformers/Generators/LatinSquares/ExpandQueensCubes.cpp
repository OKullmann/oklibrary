// Oliver Kullmann, 28.3.2024 (Swansea)
/* Copyright 2024 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Expanding a cubing for "Queen 0" to k dimensions, by translation (to be
  used in C&C)

   - The dimension N is the (constant) size of the initial "cubing".
   - The initial "cubing" are the N-Pan-Queens solutions with one queen
     sitting top-left (coming from standard input).
   - Represented as vectors of size N of elements from {0, ..., N-1}.

EXAMPLES:

Trivial mode:

LatinSquares> N=13; CPandiagonal $N "" | clasp 0 | CP_clasp_first_columns.awk -v N=$N | ./ExpandQueensCubes_debug 1 count
13 348: 1
348
LatinSquares> N=17; CPandiagonal $N "" | clasp 0 | CP_clasp_first_columns.awk -v N=$N | ./ExpandQueensCubes_debug 1 count
17 8276: 1
8276
LatinSquares> N=17; CPandiagonal +$N "" | clasp 0 | CP_clasp_first_columns.awk -v N=$N | ./ExpandQueensCubes_debug 1 count
17 28: 1
28

Considering deeper splitting (but still only counting):

LatinSquares> N=13; CPandiagonal $N "" | clasp 0 | CP_clasp_first_columns.awk -v N=$N | ./ExpandQueensCubes_debug 2 count
13 348: 2
54116
LatinSquares> N=17; time CPandiagonal $N "" | clasp 0 | CP_clasp_first_columns.awk -v N=$N | ./ExpandQueensCubes 2 count
17 8276: 2
29296454
real	0m15.698s
user	0m16.405s
sys	0m0.086s

Complete solution mode for pandiagonal problems (with abbreviated output):

LatinSquares> N=5; CPandiagonal $N "" | clasp 0 | CP_clasp_first_columns.awk -v N=$N | ./ExpandQueensCubes_debug $N ci
0 0 0 0 0
1 1 1 1 1

Explanation:
The "queens cubes" are:
LatinSquares> N=5; CPandiagonal $N "" | clasp 0 | CP_clasp_first_columns.awk -v N=$N
 0 3 1 4 2
 0 2 4 1 3
This is interpreted here as two solutions to the "toroidal N-Queens problems",
that is, "0 3 1 4 2" means
LatinSquares> echo "0 3 1 4 2" | CP_clasp_queens.awk
  0  *  *  *  *
  *  *  *  0  *
  *  0  *  *  *
  *  *  *  *  0
  *  *  0  *  *
Actually, the solutions of CPandiagonal have a different direct interpretation,
namely it gives the first column of the row-cyclic pandiagional solution,
not the positions of the queens (the zeros). The correct per-solution
interpretation is obtained by
LatinSquares> N=5; CPandiagonal $N "" | clasp 0 | CP_clasp_first_columns.awk -v N=$N -v mode=1
 0 2 4 1 3
 0 3 1 4 2
but this gives the same total set of solutions, and thus we typically ignore
this.
Back to the explanation of the two solutions "0 0 0 0 0" and "1 1 1 1 1"
in the cube-indices-interpretation: it gives the cubes-indices, appropriately
shifting the solutions, in the original list of cubes. So we have only
the two cyclic solutions, repeating the same pattern.
In dimacs-output we get
LatinSquares> N=5; CPandiagonal $N "" | clasp 0 | CP_clasp_first_columns.awk -v N=$N | ./ExpandQueensCubes_debug $N d
v 1 41 56 96 111 7 47 62 77 117 13 28 68 83 123 19 34 74 89 104 25 40 55 95 110 0
v 1 36 71 81 116 7 42 52 87 122 13 48 58 93 103 19 29 64 99 109 25 35 70 80 115 0
which gives the N^2 cells in the direct encoding, using N^3 variables
(maintaining the original order, so the first five numbers are the five
positive literals for digit 0, then digit 1, and so on).
This can be interpreted as a "partial square" via
MOLS> echo "v 1 41 56 96 111 7 47 62 77 117 13 28 68 83 123 19 34 74 89 104 25 40 55 95 110 0" | Pass2PSquares 5 /dev/stdin
A
0 1 2 3 4
2 3 4 0 1
4 0 1 2 3
1 2 3 4 0
3 4 0 1 2


LatinSquares> N=6; CPandiagonal $N "" | clasp 0 | CP_clasp_first_columns.awk -v N=$N | ./ExpandQueensCubes_debug $N ci
Empty input.
LatinSquares> N=7; CPandiagonal $N "" | clasp 0 | CP_clasp_first_columns.awk -v N=$N | ./ExpandQueensCubes_debug $N ci
0 0 0 0 0 0 0
1 1 1 1 1 1 1
2 2 2 2 2 2 2
3 3 3 3 3 3 3
LatinSquares> N=11; CPandiagonal $N "" | clasp 0 | CP_clasp_first_columns.awk -v N=$N | ./ExpandQueensCubes_debug $N ci
0 0 0 0 0 0 0 0 0 0 0
1 1 1 1 1 1 1 1 1 1 1
2 2 2 2 2 2 2 2 2 2 2
3 3 3 3 3 3 3 3 3 3 3
4 4 4 4 4 4 4 4 4 4 4
5 5 5 5 5 5 5 5 5 5 5
6 6 6 6 6 6 6 6 6 6 6
7 7 7 7 7 7 7 7 7 7 7

A longer experiment:
LatinSquares> N=13; time CPandiagonal $N "" | clasp 0 | CP_clasp_first_columns.awk -v N=$N | ./ExpandQueensCubes $N ci > PAN13
real    4651m57.218s
user    4651m33.554s
sys     0m0.547s
(on server2)
LatinSquares$ wc -l PAN13
12386 PAN13

Complete solution mode for pandiagonal strong Sudoku problems ("strong":
every digit can be arbitrarily shifted and still fulfills the Sudoku
condition); still just abbreviated output:

LatinSquares> N=13; time CPandiagonal +$N "" | clasp 0 | CP_clasp_first_columns.awk -v N=$N | ./ExpandQueensCubes $N ci | wc -l
346
real	12m39.064s
user	12m39.062s
sys	0m0.021s
LatinSquares> N=13; time CPandiagonal +$N "" | clasp 0 | CP_clasp_first_columns.awk -v N=$N | ./ExpandQueensCubes $N count
13 112: 13
346
real	12m30.450s
user	12m30.436s
sys	0m0.013s


(Compared to 398 ordinary Pandiagonal Sudokus.)


Running through all splittings on columns in order (strong Sudokus):

LatinSquares> N=17; for k in {1..13}; do echo -n "$k: "; CPandiagonal +$N "" | clasp 0 | CP_clasp_first_columns.awk -v N=$N | ./ExpandQueensCubes $k count | sed 1d; done
1: 28
2: 372
3: 2972
4: 11624
5: 20474
6: 17894
7: 7076
8: 2122
9: 714
10: 152
11: 50
12: 30
13: 28

*/

#include <iostream>

#include <ProgramOptions/Environment.hpp>

#include "Commandline.hpp"
#include "Algorithms.hpp"
#include "EQOptions.hpp"
#include "ECEncoding.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.3",
        "15.4.2024",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/LatinSquares/ExpandQueensCubes.cpp",
        "GPL v3"};

  const std::string error = "ERROR[" + proginfo.prg + "]: ";
  constexpr int commandline_args = 2;

  using namespace EQOptions;

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
      "> " << proginfo.prg <<
      " k output-form\n\n"
      " - k            : unsigned integer\n"
      " - output-form  : " << Environment::WRPO<OT>{} << "\n\n" <<
      "reads from standard input, and outputs to standard output:\n"
      "  - the first option-value is used for the empty string \"\".\n\n"
 ;
    return true;
  }

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  if (argc != commandline_args + 1) {
    std::cerr << error << "Exactly " << commandline_args << " command-line"
      " arguments needed (k, of), but the real number is " << argc-1 << ".\n";
    return 1;
  }

  const auto [k, dummy] = Commandline::read_dim(argv[1], error);
  const auto ot0 = Environment::read<OT>(argv[2]);
  if (not ot0) {
    std::cerr << error << "The output-form could not be read from"
        " string \"" << argv[2] << "\".\n";
    return 1;
  }
  const OT ot = ot0.value();
  const bool output = ot != OT::count_only;

  const auto init_cubes = Algorithms::read_queens_cubing(std::cin);
  if (init_cubes.m == 0) {
    std::cout << "Empty input.\n";
    return 0;
  }

  if (not output) {
    std::cout << init_cubes.N << " " << init_cubes.m <<
      ": " << k << std::endl;
    Algorithms::all_solutions(init_cubes, k);
    std::cout << Algorithms::expand_total_count << "\n";
  }
  else {
    if (k >=  init_cubes.N) {
      Algorithms::all_solutions(init_cubes, std::cout, ot);
      return 0;
    }
    Algorithms::all_solutions(init_cubes, k, std::cout, ot);
  }
}

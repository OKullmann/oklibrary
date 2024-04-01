// Oliver Kullmann, 28.3.2024 (Swansea)
/* Copyright 2024 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Epanding a cubing for "Queen 0" to k dimensions, by translation.

   - The dimension N is the (constant) size of the initial "cubing".
   - The initial "cubing" are the N-Pan-Queens solutions with one queen
     sitting top-left (coming from standard input).
   - Represented as vectors of size N of elements from {0, ..., N-1}.

EXAMPLES:

Trivial mode:

LatinSquares> N=13; CPandiagonal $N "" | clasp 0 | CP_clasp_first_columns.awk -v N=$N | ./ExpandQueensCubes_debug 1 ""
13 348: 1
LatinSquares> N=17; CPandiagonal +$N "" | clasp 0 | CP_clasp_first_columns.awk -v N=$N | ./ExpandQueensCubes_debug 1 ""
17 28: 1

Complete solution mode for pandiagonal problems:

LatinSquares> N=5; CPandiagonal $N "" | clasp 0 | CP_clasp_first_columns.awk -v N=$N | ./ExpandQueensCubes_debug +$N ci
0 0 0 0 0
1 1 1 1 1
LatinSquares> N=6; CPandiagonal $N "" | clasp 0 | CP_clasp_first_columns.awk -v N=$N | ./ExpandQueensCubes_debug +$N ci
Empty input.
LatinSquares> N=7; CPandiagonal $N "" | clasp 0 | CP_clasp_first_columns.awk -v N=$N | ./ExpandQueensCubes_debug +$N ci
0 0 0 0 0 0 0
1 1 1 1 1 1 1
2 2 2 2 2 2 2
3 3 3 3 3 3 3
LatinSquares> N=11; CPandiagonal $N "" | clasp 0 | CP_clasp_first_columns.awk -v N=$N | ./ExpandQueensCubes_debug +$N ci
0 0 0 0 0 0 0 0 0 0 0
1 1 1 1 1 1 1 1 1 1 1
2 2 2 2 2 2 2 2 2 2 2
3 3 3 3 3 3 3 3 3 3 3
4 4 4 4 4 4 4 4 4 4 4
5 5 5 5 5 5 5 5 5 5 5
6 6 6 6 6 6 6 6 6 6 6
7 7 7 7 7 7 7 7 7 7 7

LatinSquares> N=13; time CPandiagonal $N "" | clasp 0 | CP_clasp_first_columns.awk -v N=$N | ./ExpandQueensCubes +$N > PAN13
XXX running server2 XXX

Complete solution mode for pandiagonal strong Sudoku problems ("strong":
every digit can be arbitrarily shifted and still fulfills the Sudoku
condition):

LatinSquares> N=13; time CPandiagonal +$N "" | clasp 0 | CP_clasp_first_columns.awk -v N=$N | ./ExpandQueensCubes +$N ci | wc -l
346
real	13m15.241s
user	13m15.153s
sys	0m0.030s

(Compared to 398 ordinary Pandiagonal Sudokus.)

Splitting on columns in order:

LatinSquares> N=17; for k in {1..13}; do echo -n "$k: "; CPandiagonal +$N "" | clasp 0 | CP_clasp_first_columns.awk -v N=$N | ./ExpandQueensCubes +$k ci | wc -l; done
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

#include <cstdint>

#include <ProgramOptions/Environment.hpp>

#include "Commandline.hpp"
#include "Algorithms.hpp"
#include "EQOptions.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.0.13",
        "1.4.2024",
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
      " [+]k output-form\n\n"
      " - [+]k            : unsigned integer\n"
      " - output-form     : " << Environment::WRPO<OT>{} << "\n\n" <<
      "reads from standard input, and outputs to standard output:\n"
      "  - default is to output only statistics\n"
      "    - \"+\" means to output instead the expanded cubing\n"
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

  const auto [k, output] = Commandline::read_dim(argv[1], error);
  const auto ot0 = Environment::read<OT>(argv[2]);
  if (not ot0) {
    std::cerr << error << "The output-form could not be read from"
        " string \"" << argv[2] << "\".\n";
    return 1;
  }
  const OT ot = ot0.value();

  const auto init_cubes = Algorithms::read_queens_cubing(std::cin);
  if (init_cubes.m == 0) {
    std::cout << "Empty input.\n";
    return 0;
  }

  if (not output) {
    std::cout << init_cubes.N << " " << init_cubes.m <<
      ": " << k << std::endl;
    if (k == 1) return 0;
    // XXX
  }
  else {
    if (k >=  init_cubes.N) {
      Algorithms::all_solutions(init_cubes, std::cout, ot);
      return 0;
    }
    Algorithms::all_solutions(init_cubes, k, {}, std::cout, ot);
  }
}

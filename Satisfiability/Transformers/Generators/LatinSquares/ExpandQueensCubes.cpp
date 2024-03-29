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

LatinSquares> N=13; CPandiagonal $N "" | clasp 0 | CP_clasp_first_columns.awk -v N=$N | ./ExpandQueensCubes_debug 1
13 348: 1
LatinSquares> N=17; CPandiagonal +$N "" | clasp 0 | CP_clasp_first_columns.awk -v N=$N | ./ExpandQueensCubes_debug 1
17 28: 1


*/

#include <iostream>

#include <cstdint>

#include <ProgramOptions/Environment.hpp>

#include "Commandline.hpp"
#include "Algorithms.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.0.2",
        "28.3.2024",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/LatinSquares/ExpandQueensCubes.cpp",
        "GPL v3"};

  const std::string error = "ERROR[" + proginfo.prg + "]: ";
  constexpr int commandline_args = 1;

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
      "> " << proginfo.prg <<
      " [+]k""\n\n"
      "reads from standard input, and outputs to standard output:\n"
      "  - default is to output only statistics\n"
      "  - \"+\" means to output instead the expanded cubing.\n\n"
 ;
    return true;
  }

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  if (argc != commandline_args + 1) {
    std::cerr << error << "Exactly " << commandline_args << " command-line"
      " arguments needed (k), but the real number is " << argc-1 << ".\n";
    return 1;
  }

  const auto [k, output] = Commandline::read_dim(argv[1], error);
  const auto init_cubes = Algorithms::read_queens_cubing(std::cin);

  if (not output) {
    std::cout << init_cubes.N << " " << init_cubes.m <<
      ": " << k << std::endl;
    if (k == 1) return 0;
    // XXX
  }
  else {
    // XXX
  }
}

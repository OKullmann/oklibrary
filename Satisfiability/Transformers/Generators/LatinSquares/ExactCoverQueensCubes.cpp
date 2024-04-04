// Oliver Kullmann, 3.4.2024 (Swansea)
/* Copyright 2024 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Same input as ExpandQueensCubes.cpp, but now creating a program
  to be compiled with the system
    Ben Lynn <blynn@cs.stanford.edu> https://github.com/blynn/dlx
  for running the DLX-algorithms to compute all solutions

USAGE:

LatinSquares> ExactCoverQueensCubes

reads from standard input, determines N (dimension) and m (number of
cubes), and creates the file

  EC_QC_N_m.c

To be compiled by

LatinSquares> gcc -O3 -Wall -o EC_QC_N_m EC_QC_N_m.c dlx.c

EXAMPLES:

LatinSquares> N=5; CPandiagonal $N "" | clasp 0 | CP_clasp_first_columns.awk -v N=$N
 0 3 1 4 2
 0 2 4 1 3
> N=5; CPandiagonal $N "" | clasp 0 | CP_clasp_first_columns.awk -v N=$N | ./ExactCoverQueensCubes
XXX

LatinSquares> N=13; CPandiagonal $N "" | clasp 0 | CP_clasp_first_columns.awk -v N=$N | ./ExactCoverQueensCubes
LatinSquares> time gcc -O3 -Wall -o EC_QC_13_348 EC_QC_13_348.c dlx.c
real	0m8.201s
user	0m7.897s
sys	0m0.304s
LatinSquares> time ./EC_QC_13_348 > AUS13
real	7m49.270s
user	7m49.219s
sys	0m0.008s
LatinSquares> wc -l AUS13
12386 AUS13

N=17: XXX


*/

#include <iostream>
#include <fstream>

#include <ProgramOptions/Environment.hpp>

#include "Algorithms.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.0",
        "4.4.2024",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/LatinSquares/ExactCoverQueensCubes.cpp",
        "GPL v3"};

  const std::string error = "ERROR[" + proginfo.prg + "]: ";
  constexpr int commandline_args = 0;

  const std::string prefix = "EC_QC_", suffix = ".c";

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
      "> " << proginfo.prg <<
      "\n\n"
      "reads from standard input, established N, m, and creates file\n"
      "  " << prefix << "N_m" << suffix << " .\n\n"
 ;
    return true;
  }

  const std::string init_part =
R"(#include <stdio.h>
#include "dlx.h"
int main() {
  dlx_t d = dlx_new();

)";

  const std::string final_part = R"(
  void f(int row[], const int n) {
    for (int i = 0; i < n; ++i) printf(" %d", row[i]);
    printf("\n");
  }
  dlx_forall_cover(d, f);
  dlx_clear(d);
  return 0;
}
)";

  using UInt_t = Algorithms::UInt_t;
  constexpr UInt_t cell_index(const UInt_t i, const UInt_t j,
                              const UInt_t N) noexcept {
    assert(i < N); assert(j < N);
    return i * N + j;
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

  const auto init_cubes = Algorithms::read_queens_cubing(std::cin);
  if (init_cubes.m == 0) {
    std::cout << "Empty input.\n";
    return 0;
  }

  const std::string basefilename = prefix + std::to_string(init_cubes.N) +
    "_" + std::to_string(init_cubes.m);
  const std::string filename = basefilename + suffix;
  std::ofstream file(filename);
  if (not file) {
    std::cerr << error << "Can not open file \"" << filename << "\" for"
      " writing.\n";
    return 1;
  }
  std::cout << "Compile with:\ngcc -O3 -Wall -o " << basefilename <<
    " " << filename << " dlx.c\n";

  file << init_part;
  for (UInt_t co = 0, dlx_row=0; co < init_cubes.N; ++co)
    for (UInt_t cu = 0; cu < init_cubes.m; ++cu, ++dlx_row) {
      const auto Q = init_cubes.queens({co,cu});
      for (UInt_t i = 0; i < init_cubes.N; ++i) {
        const UInt_t j = Q[i];
        const UInt_t index = cell_index(i, j, init_cubes.N);
        file << "dlx_set(d," << dlx_row << "," << index << ");";
      }
      file << std::endl;
    }
  file << final_part;
}

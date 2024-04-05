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
Main block of QC_5_2.c:
dlx_set(d,0,0);dlx_set(d,0,8);dlx_set(d,0,11);dlx_set(d,0,19);dlx_set(d,0,22);
dlx_set(d,1,0);dlx_set(d,1,7);dlx_set(d,1,14);dlx_set(d,1,16);dlx_set(d,1,23);
dlx_set(d,2,1);dlx_set(d,2,9);dlx_set(d,2,12);dlx_set(d,2,15);dlx_set(d,2,23);
dlx_set(d,3,1);dlx_set(d,3,8);dlx_set(d,3,10);dlx_set(d,3,17);dlx_set(d,3,24);
dlx_set(d,4,2);dlx_set(d,4,5);dlx_set(d,4,13);dlx_set(d,4,16);dlx_set(d,4,24);
dlx_set(d,5,2);dlx_set(d,5,9);dlx_set(d,5,11);dlx_set(d,5,18);dlx_set(d,5,20);
dlx_set(d,6,3);dlx_set(d,6,6);dlx_set(d,6,14);dlx_set(d,6,17);dlx_set(d,6,20);
dlx_set(d,7,3);dlx_set(d,7,5);dlx_set(d,7,12);dlx_set(d,7,19);dlx_set(d,7,21);
dlx_set(d,8,4);dlx_set(d,8,7);dlx_set(d,8,10);dlx_set(d,8,18);dlx_set(d,8,21);
dlx_set(d,9,4);dlx_set(d,9,6);dlx_set(d,9,13);dlx_set(d,9,15);dlx_set(d,9,22);
LatinSquares> ./EC_QC_5_2
 0 2 4 6 8
 1 3 5 7 9


LatinSquares> N=13; CPandiagonal $N "" | clasp 0 | CP_clasp_first_columns.awk -v N=$N | ./ExactCoverQueensCubes
LatinSquares> time gcc -O3 -Wall -o EC_QC_13_348 EC_QC_13_348.c dlx.c
real	0m8.201s
user	0m7.897s
sys	0m0.304s
LatinSquares> time ./EC_QC_13_348 > OUT13
real	7m33.318s
user	7m33.131s
sys	0m0.176s
LatinSquares> wc -l OUT13
12386 OUT13

On a machien with larger memory (~ 27GB needed):
LatinSquares> N=17; CPandiagonal $N "" | clasp 0 | CP_clasp_first_columns.awk -v N=$N | ./ExactCoverQueensCubes
/usr/bin/ld: warning: /tmp/ccrF7UBT.o: requires executable stack (because the .note.GNU-stack section is executable)
real    8m25.847s
user    8m5.052s
sys     0m20.759s
LatinSquares> ulimit -s unlimited
LatinSquares> ls -l EC_QC_17_8276*
-rwxr-xr-x 1 oliver users 43045072 Apr  4 05:35 EC_QC_17_8276
-rw-r--r-- 1 oliver users 49960512 Apr  4 05:26 EC_QC_17_8276.c
LatinSquares> time ./EC_QC_17_8276
XXX

TODOS:

0. Provide for translating the output:
  - Another program could do the translation (given the same input).
  - Or the created C-program prints the squares.
   - Perhaps this is indicated by passing N, M as the two parameters of
     the program.
  - Likely the following output-format "d,r" (division, remainder) is always
    better: it shows for the "digits" 0,...,N-1 the choice of queens-solutions
    (appropriately shifted); the current output (the raw indices of the "rows")
    is translated as follows, using m=M:
awk -v m=M 'BEGIN{PROCINFO["sorted_in"]="@ind_num_asc"}{delete A;for (i=1;i<=NF;++i) A[$i]="";for (i in A) {d=int(i/m);r=i%m;printf " %d,%d",d,r}printf "\n"}' OUTPUT

1. Interpretation of the queens in the example:
  - Since CP_clasp_first_columns.awk uses the default mode=0,
    the original output is the first column of the cyclic pandiagonal square.
  - This then gets re-interpreted as the positions of the queens.
  - So using mode=1 would be more appropriate.

2. Replace dlx.h|c by a native program:
  - Only needing the same input as ExactCoverQueensCubes.
  - One thread for each of the m possibilities for the first "queen" (digit).

*/

#include <iostream>
#include <fstream>

#include <ProgramOptions/Environment.hpp>

#include "Algorithms.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.1",
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
  setbuf(stdout, NULL);
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

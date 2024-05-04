// Oliver Kullmann, 26.4.2024 (Swansea)
/* Copyright 2024 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Reads queens-solutions from standard-input, and determines their expansions
  in all directions

EXAMPLES:

MOLS> N=5; CPandiagonal $N "" | clasp 0 | passextractpos.awk | Sort | CP_clasp_first_columns.awk -v N=$N -v mode=1 | ./Qcyclicity_debug
# 2
1 sizes and their frequencies:
 1:2
Sum: 2
1 directions and their frequencies:
 (0,1):2

MOLS> N=13; CPandiagonal $N "" | clasp 0 | passextractpos.awk | Sort | CP_clasp_first_columns.awk -v N=$N -v mode=1 | ./Qcyclicity_debug
# 348
3 sizes and their frequencies:
 1:10 4:234 6:104
Sum: 1570
12 directions and their frequencies:
 (0,1):348 (1,0):338 (1,1):338 (1,2):26 (1,3):26 (1,4):26 (1,6):26 (1,7):26 (1,9):26 (1,10):26 (1,11):26 (1,12):338

Remark 1: 1*10 + 4*234 + 6*104 = 1570 = 348 + 3*338 + 8*26
Remark 2: Using only some representatives, we get a subset of the directions:
MOLS> N=13; CPandiagonal $N "" | clasp 0 | passextractpos.awk | Sort | CP_clasp_first_columns.awk -v N=$N -v mode=1 | ./QueensSymmetries S13 > /dev/null
MOLS> cat S13 | ./Qcyclicity
# 5
Sizes and frequencies:
 1:2 4:2 6:1
Sum: 16
6 directions and their frequencies:
 (0,1):5 (1,0):3 (1,1):3 (1,4):1 (1,9):1 (1,12):3


MOLS> N=17; time CPandiagonal $N "" | clasp 0 | passextractpos.awk | Sort | CP_clasp_first_columns.awk -v N=$N -v mode=1 | ./Qcyclicity
# 8276
3 sizes and their frequencies:
 1:14 4:8024 8:238
Sum: 34014
16 directions and their frequencies:
 (0,1):8276 (1,0):8262 (1,1):8262 (1,2):68 (1,3):68 (1,5):102 (1,6):68 (1,7):102 (1,8):68 (1,9):68 (1,10):102 (1,11):68 (1,12):102 (1,14):68 (1,15):68 (1,16):8262
real	0m9.774s
user	0m10.615s
sys	0m0.061s

MOLS> N=19; time CPandiagonal $N "" | clasp 0 | passextractpos.awk | Sort | CP_clasp_first_columns.awk -v N=$N -v mode=1 | ./Qcyclicity
# 43184
4 sizes and their frequencies:
 1:16 4:42408 6:456 9:304
Sum: 175120
20 directions and their frequencies:
 (0,1):43184 (1,0):43168 (1,1):43168 (1,2):190 (1,3):190 (1,4):114 (1,5):114 (1,6):190 (1,7):114 (1,8):114 (1,9):190 (1,10):190 (1,11):114 (1,12):114 (1,13):190 (1,14):114 (1,15):114 (1,16):190 (1,17):190 (1,18):43168
real	2m59.097s
user	3m4.322s
sys	0m0.480s

The sequence of numbers of pandiagonal squares which are cyclic at least
in some direction for N = 5, 7, 11, 13, 17, 19:
2 4 8 1570 34014 175120
After subtraction of N-3 and with interspersed 0's this is
https://oeis.org/A343867 ("semicyclic"):
0, 0, 0, 0, 0, 0, 1560, 0, 34000, 175104, 0, 22417824, 313235960, 0, 83574857328, 1729671003296

*/

#include <iostream>
#include <map>

#include <cassert>

#include <ProgramOptions/Environment.hpp>
#include <Numerics/NumInOut.hpp>

#include "BasicLatinSquares.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.1",
        "4.5.2024",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Solvers/Gecode/MOLS/Qcyclicity.cpp",
        "GPL v3"};

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  using namespace BasicLatinSquares;

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg << "\n\n"
      "reads queens-solutions from standard input, and outputs statistics.\n\n"
;
    return true;
  }

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  std::map<size_t, size_t> sizes_counts;
  std::map<ModDir, size_t> directions_counts;
  size_t count = 0;
  while (std::cin) {
    std::string line;
    std::getline(std::cin, line);
    const ls_row_t Q = FloatingPoint::to_vec_unsigned<size_t>(line, ' ');
    if (Q.empty()) break;
    const expansions_t E = all_expansions(Q);
    assert(not E.empty());
    ++count; ++sizes_counts[E.size()];
    for (const auto& [S,d] : E)
      ++directions_counts[d];
  };

  std::cout << "# " << count << "\n" << sizes_counts.size()
            << " sizes and their frequencies:\n";
  size_t sum = 0;
  for (const auto& [s,c] : sizes_counts) {
    sum += s*c;
    std::cout << " " << s << ":" << c;
  }
  std::cout << "\nSum: " << sum << "\n" << directions_counts.size()
            << " directions and their frequencies:\n";
  for (const auto& [d,c] : directions_counts)
    std::cout << " " << d << ":" << c;
  std::cout << "\n";
}

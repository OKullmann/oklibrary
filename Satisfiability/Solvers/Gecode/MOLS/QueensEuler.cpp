// Oliver Kullmann, 30.4.2024 (Swansea)
/* Copyright 2024 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Computing the most basic toroidal N-Quees solutions, the
  "Euler solutions".

EXAMPLES:

MOLS> for m in {1..10}; do N1=$((6*m-1)); N2=$((6*m+1)); for N in $N1 $N2; do echo -n "$N "; ./QueensEuler_debug $N | wc -l; done; done
5 2
7 4
11 8
13 10
17 14
19 16
23 20
25 10
29 26
31 28
35 8
37 34
41 38
43 40
47 44
49 28
53 50
55 16
59 56
61 58

For prime N with gcd(N,6)=1 there are exactly N-3 solutions.

The queens-solutions:
MOLS> ./QueensEuler_debug 5
0 2 4 1 3
0 3 1 4 2

Expanded into row-cyclic pandiagonal latin squares:
MOLS> ./QueensEuler_debug 5 | CP_clasp_expand.awk -v mode=1
  0  1  2  3  4
  3  4  0  1  2
  1  2  3  4  0
  4  0  1  2  3
  2  3  4  0  1

  0  1  2  3  4
  2  3  4  0  1
  4  0  1  2  3
  1  2  3  4  0
  3  4  0  1  2

MOLS> ./QueensEuler_debug 7
0 2 4 6 1 3 5
0 3 6 2 5 1 4
0 4 1 5 2 6 3
0 5 3 1 6 4 2
MOLS> ./QueensEuler_debug 7 | CP_clasp_expand.awk -v mode=1
  0  1  2  3  4  5  6
  5  6  0  1  2  3  4
  3  4  5  6  0  1  2
  1  2  3  4  5  6  0
  6  0  1  2  3  4  5
  4  5  6  0  1  2  3
  2  3  4  5  6  0  1

  0  1  2  3  4  5  6
  4  5  6  0  1  2  3
  1  2  3  4  5  6  0
  5  6  0  1  2  3  4
  2  3  4  5  6  0  1
  6  0  1  2  3  4  5
  3  4  5  6  0  1  2

  0  1  2  3  4  5  6
  3  4  5  6  0  1  2
  6  0  1  2  3  4  5
  2  3  4  5  6  0  1
  5  6  0  1  2  3  4
  1  2  3  4  5  6  0
  4  5  6  0  1  2  3

  0  1  2  3  4  5  6
  2  3  4  5  6  0  1
  4  5  6  0  1  2  3
  6  0  1  2  3  4  5
  1  2  3  4  5  6  0
  3  4  5  6  0  1  2
  5  6  0  1  2  3  4

Checking that all solutions are cyclic in all four directions:
MOLS> ./QueensEuler_debug 25 | CP_clasp_expand.awk -v mode=1 | ./LScyclicity
N 25
11 1111
11 1111
11 1111
11 1111
11 1111
11 1111
11 1111
11 1111
11 1111
11 1111
count 10

Determining the (standard) symmetries:
MOLS> for m in {1..10}; do N1=$((6*m-1)); N2=$((6*m+1)); for N in $N1 $N2; do ./QueensEuler_debug $N | ./QueensSymmetries; echo; done; done
5 2
# 1
2

7 4
# 1
4

11 8
# 1
8

13 10
# 2
8 2

17 14
# 3
8 2 4

19 16
# 2
8 8

23 20
# 3
8 4 8

25 10
# 2
8 2

29 26
# 4
8 8 8 2

31 28
# 4
8 8 8 4

35 8
# 1
8

37 34
# 5
8 8 8 2 8

41 38
# 6
8 8 8 8 2 4

43 40
# 5
8 8 8 8 8

47 44
# 6
8 8 8 4 8 8

49 28
# 4
8 8 8 4

53 50
# 7
8 8 8 8 8 8 2

55 16
# 2
8 8

59 56
# 7
8 8 8 8 8 8 8

61 58
# 8
8 8 8 8 8 8 2 8


*/

#include <iostream>

#include <ProgramOptions/Environment.hpp>
#include <Numerics/NumInOut.hpp>

#include "BasicLatinSquares.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.0.9",
        "30.4.2024",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Solvers/Gecode/MOLS/QueensEuler.cpp",
        "GPL v3"};

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  using namespace BasicLatinSquares;

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
      "> " << proginfo.prg << "N\n\n"
      "prints the Euler-solutions to standard output.\n\n"
 ;
    return true;
  }

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  if (argc != 2) {
    std::cerr << error << "Exactly N required, but " << argc-1 << " arguments"
      " provided.\n";
    return 1;
  }

  const size_t N = FloatingPoint::to_UInt(argv[1]);
  for (size_t d = 0; d < N; ++d) {
    if (not is_euler_solution(N, d)) continue;
    Environment::out_line(std::cout, euler_solution(N, d));
    std::cout << "\n";
  }
}

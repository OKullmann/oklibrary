// Oliver Kullmann, 23.4.2024 (Swansea)
/* Copyright 2024 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Application of symmetries and related transformations to squares

EXAMPLES:

1  2  3  4
5  6  7  8
9  10 11 12
13 14 15 16

Rows:
1 2 3 4
5 6 7 8
9 10 11 12
13 14 15 16

Columns:
1 5 9 13
2 6 10 14
3 7 11 15
4 8 12 16

Diagonals:
1 6 11 16
2 7 12 13
3 8 9 14
4 5 10 15

Antidiagonals:
4 7 10 13
3 6 9 16
2 5 12 15
1 8 11 14

Transposition (reflection at the main diagonal):
MOLS> echo -e "1 2 3 4\n5 6 7 8\n9 10 11 12\n13 14 15 16" | ./LSrotation_debug -t
1 5 9 13
2 6 10 14
3 7 11 15
4 8 12 16
Antiransposition (reflection at the main antidiagonal):
MOLS> echo -e "1 2 3 4\n5 6 7 8\n9 10 11 12\n13 14 15 16" | ./LSrotation_debug -at
16 12 8 4
15 11 7 3
14 10 6 2
13 9 5 1

Moving the diagonals into the rows:
MOLS> echo -e "1 2 3 4\n5 6 7 8\n9 10 11 12\n13 14 15 16" | ./LSrotation_debug -d
1 6 11 16
2 7 12 13
3 8 9 14
4 5 10 15

Moving the antidiagonals into the rows:
MOLS> echo -e "1 2 3 4\n5 6 7 8\n9 10 11 12\n13 14 15 16" | ./LSrotation_debug -ad
4 7 10 13
3 6 9 16
2 5 12 15
1 8 11 14

Keeping the first column, and otherwise reversing the direction of
every row:
MOLS> echo -e "1 2 3 4\n5 6 7 8\n9 10 11 12\n13 14 15 16" | ./LSrotation_debug -n2
1 4 3 2
5 8 7 6
9 12 11 10
13 16 15 14

Move the rows into the diagonals and the columns into the antidiagonals
(replace (i,j) by (i+j,j-i) mod N):
Not a permutation for even N:
MOLS> echo -e "1 2 3 4\n5 6 7 8\n9 10 11 12\n13 14 15 16" | ./LSrotation_debug -sd
11 0 14 0
0 12 0 15
16 0 9 0
0 13 0 10
MOLS> echo -e "1 2 3\n4 5 6\n7 8 9" | ./LSrotation_debug -sd
1 6 8
9 2 4
5 7 3

Vertical and horizontal shift:
MOLS> echo -e "1 2 3 4\n5 6 7 8\n9 10 11 12\n13 14 15 16" | ./LSrotation_debug -sh 1 2
15 16 13 14
3 4 1 2
7 8 5 6
11 12 9 10

Scaling the coordinates with factor 3:
MOLS> echo -e "1 2 3 4\n5 6 7 8\n9 10 11 12\n13 14 15 16" | ./LSrotation_debug -sc 3
1 4 3 2
13 16 15 14
9 12 11 10
5 8 7 6
Not a permutation iff the factor and N have a nontrivial common factor:
MOLS> echo -e "1 2 3 4\n5 6 7 8\n9 10 11 12\n13 14 15 16" | ./LSrotation_debug -sc 2
11 0 12 0
0 0 0 0
15 0 16 0
0 0 0 0



TODOS:

1. Perhaps one should just reproduce the empty lines as found
   in the input?

*/

#include <iostream>

#include <cstdlib>

#include <ProgramOptions/Environment.hpp>
#include <Numerics/NumInOut.hpp>

#include "BasicLatinSquares.hpp"
#include "LSOptions.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.4",
        "27.4.2024",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Solvers/Gecode/MOLS/LSrotation.cpp",
        "GPL v3"};

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  using namespace BasicLatinSquares;
  using namespace LSOptions;

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg << " [-]rot\n\n"
      " - rot            : " << Environment::WRPO<SR>{} << "\n\n"
      "reads squares from standard input, and applies the transformation"
      " to standard output:\n"
      " - \"-\" means without row-standardisation.\n\n"
;
    return true;
  }

  std::tuple<SR, bool, size_t, size_t>
  read_rot(const int argc, const char* const argv[]) noexcept {
    if (argc == 1) {
      std::cerr << error << "At least one command-line arguments needed"
        " (rot).\n";
      std::exit(1);
    }
    std::string s(argv[1]);
    const bool without_stand = s.starts_with("-");
    if (without_stand) s = s.substr(1);
    const auto sr0 = Environment::read<SR>(s);
    if (not sr0) {
      std::cerr << error << "rotation not readable from \""
                << s << "\".\n";
      std::exit(1);
    }
    const SR sr = sr0.value();
    const auto numargs = args(sr);
    if (numargs == 0) return {sr, without_stand, 0, 0};
    if (argc == 2) {
      std::cerr << error << "At least one numerical argument needed for \""
                << s << "\".\n";
      std::exit(1);
    }
    const size_t x = FloatingPoint::toUInt(argv[2]);
    if (numargs == 1) return {sr, without_stand, x, 0};
    assert(numargs == 2);
    if (argc == 3) {
      std::cerr << error << "At least two numerical arguments needed for \""
                << s << "\".\n";
      std::exit(1);
    }
    const size_t y = FloatingPoint::toUInt(argv[3]);
    return {sr, without_stand, x, y};
  }

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  const auto [sr, without_stand, x, y] = read_rot(argc, argv);

  bool first = true;
  do {
    ls_t S;
    do S = in_strictls(std::cin); while (S.empty() and std::cin);
    if (S.empty()) return 0;
    if (not sqshape(S)) {
      std::cerr << error << "Square not square-shaped:\n";
      out(std::cerr, S);
      return 1;
    }
    switch (sr) {
    case SR::t : transpositionm(S); break;
    case SR::at : antitranspositionm(S); break;
    case SR::d : S = moddiags2rows(S); break;
    case SR::ad : S = modantidiags2rows(S); break;
    case SR::n2 : S = negationj(S); break;
    case SR::sd : S = sumdiff(S); break;
    case SR::sh : S = shift(S,x,y); break;
    case SR::sc : S = scaling(S,x); break;
    }
    if (not without_stand) {
      if (not sqval(S)) {
        std::cerr << error << "Square not square-valued:\n";
        out(std::cerr, S);
        return 1;
      }
      rstandardisem(S);
    }
    if (not first) std::cout << "\n";
    else first = false;
    out(std::cout, S);
  } while (std::cin);
}

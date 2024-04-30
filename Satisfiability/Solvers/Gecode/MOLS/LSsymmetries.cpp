// Oliver Kullmann, 23.4.2024 (Swansea)
/* Copyright 2024 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Application of symmetries and related transformations to squares
  and queens-solutions (toroidal)

   - The squares on standard input might be separated by empty lines,
     but don't have to; the output will separate them in any case with
     one empty line.
   - Also queens-solutions can be input, then each on its own line (empty
     lines finish the reading here).

EXAMPLES:

First considering the square

1  2  3  4
5  6  7  8
9  10 11 12
13 14 15 16

with

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
MOLS> echo -e "1 2 3 4\n5 6 7 8\n9 10 11 12\n13 14 15 16" | ./LSsymmetries_debug -t
1 5 9 13
2 6 10 14
3 7 11 15
4 8 12 16
Antiransposition (reflection at the main antidiagonal):
MOLS> echo -e "1 2 3 4\n5 6 7 8\n9 10 11 12\n13 14 15 16" | ./LSsymmetries_debug -at
16 12 8 4
15 11 7 3
14 10 6 2
13 9 5 1

Moving the diagonals into the rows:
MOLS> echo -e "1 2 3 4\n5 6 7 8\n9 10 11 12\n13 14 15 16" | ./LSsymmetries_debug -d
1 6 11 16
2 7 12 13
3 8 9 14
4 5 10 15

Moving the antidiagonals into the rows:
MOLS> echo -e "1 2 3 4\n5 6 7 8\n9 10 11 12\n13 14 15 16" | ./LSsymmetries_debug -ad
4 7 10 13
3 6 9 16
2 5 12 15
1 8 11 14

Keeping the first column, and otherwise reversing the direction of
every row:
MOLS> echo -e "1 2 3 4\n5 6 7 8\n9 10 11 12\n13 14 15 16" | ./LSsymmetries_debug -n2
1 4 3 2
5 8 7 6
9 12 11 10
13 16 15 14

Move the rows into the diagonals and the columns into the antidiagonals
(replace (i,j) by (i+j,j-i) mod N):
Not a permutation for even N:
MOLS> echo -e "1 2 3 4\n5 6 7 8\n9 10 11 12\n13 14 15 16" | ./LSsymmetries_debug -sd
11 0 14 0
0 12 0 15
16 0 9 0
0 13 0 10
MOLS> echo -e "1 2 3\n4 5 6\n7 8 9" | ./LSsymmetries_debug -sd
1 6 8
9 2 4
5 7 3

Vertical and horizontal shift:
MOLS> echo -e "1 2 3 4\n5 6 7 8\n9 10 11 12\n13 14 15 16" | ./LSsymmetries_debug -sh 1 2
15 16 13 14
3 4 1 2
7 8 5 6
11 12 9 10

Scaling the coordinates with factor 3:
MOLS> echo -e "1 2 3 4\n5 6 7 8\n9 10 11 12\n13 14 15 16" | ./LSsymmetries_debug -sc 3
1 4 3 2
13 16 15 14
9 12 11 10
5 8 7 6
Not a permutation iff the factor and N have a nontrivial common factor:
MOLS> echo -e "1 2 3 4\n5 6 7 8\n9 10 11 12\n13 14 15 16" | ./LSsymmetries_debug -sc 2
11 0 12 0
0 0 0 0
15 0 16 0
0 0 0 0

"Queens-shifting":
MOLS> for x in {0..13}; do echo "0 11 9 7 2 4 1 8 10 12 6 3 5" | ./LSsymmetries_debug +sh $x; done
0 11 9 7 2 4 1 8 10 12 6 3 5
0 11 9 4 6 3 10 12 1 8 5 7 2
0 11 6 8 5 12 1 3 10 7 9 4 2
0 8 10 7 1 3 5 12 9 11 6 4 2
0 2 12 6 8 10 4 1 3 11 9 7 5
0 10 4 6 8 2 12 1 9 7 5 3 11
0 7 9 11 5 2 4 12 10 8 6 1 3
0 2 4 11 8 10 5 3 1 12 7 9 6
0 2 9 6 8 3 1 12 10 5 7 4 11
0 7 4 6 1 12 10 8 3 5 2 9 11
0 10 12 7 5 3 1 9 11 8 2 4 6
0 2 10 8 6 4 12 1 11 5 7 9 3
0 8 6 4 2 10 12 9 3 5 7 1 11
0 11 9 7 2 4 1 8 10 12 6 3 5



TODOS:

1. Perhaps one should just reproduce the empty lines as found
   in the input (for handling squares)?

*/

#include <iostream>

#include <cstdlib>

#include <ProgramOptions/Environment.hpp>
#include <Numerics/NumInOut.hpp>

#include "BasicLatinSquares.hpp"
#include "LSOptions.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.2.1",
        "30.4.2024",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Solvers/Gecode/MOLS/LSsymmetries.cpp",
        "GPL v3"};

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  using namespace BasicLatinSquares;
  using namespace LSOptions;

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg << " [-+]perm [x] [y]\n\n"
      " - perm          : " << Environment::WRPO<SP>{} << "\n"
      " - x, y          : natural numbers (arguments for shift and scaling\n\n"
      "reads squares from standard input, and applies the transformation,"
      " to standard output:\n"
      " - \"-\" means without row-standardisation\n"
      " - \"+\" means input is not squares, but queens-solutions.\n\n"
;
    return true;
  }

  // Variations:
  enum class VR { square = 0, s_no_renaming = 1, queens = 2 };
  VR get_variation(const std::string& s) noexcept {
    if (s.starts_with("-")) return VR::s_no_renaming;
    else if (s.starts_with("+")) return VR::queens;
    else return VR::square;
  }
  std::tuple<SP, VR, size_t, size_t>
  read_perm(const int argc, const char* const argv[]) noexcept {
    assert(argc >= 1);
    if (argc == 1) {
      std::cerr << error << "At least one command-line arguments needed"
        " (rot).\n";
      std::exit(1);
    }
    std::string s(argv[1]);
    const VR vr = get_variation(s);
    if (vr != VR::square) s = s.substr(1);
    const auto sp0 = Environment::read<SP>(s);
    if (not sp0) {
      std::cerr << error << "symmetry-type not readable from \""
                << s << "\".\n";
      std::exit(1);
    }
    const SP sp = sp0.value();
    const auto numargs = args(sp);
    if (numargs == 0) return {sp, vr, 0, 0};
    if (argc == 2) {
      std::cerr << error << "At least one numerical argument needed for \""
                << s << "\".\n";
      std::exit(1);
    }
    const size_t x = FloatingPoint::toUInt(argv[2]);
    if (numargs == 1 or argc == 3) return {sp, vr, x, 0};
    assert(numargs == 2);
    const size_t y = FloatingPoint::toUInt(argv[3]);
    return {sp, vr, x, y};
  }

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  const auto [sp, vr, x, y] = read_perm(argc, argv);

  bool first = true;
  do {
    if (vr != VR::queens) {
      ls_t S;
      do S = in_strictls(std::cin); while (S.empty() and std::cin);
      if (S.empty()) return 0;
      if (not sqshape(S)) {
        std::cerr << error << "Square not square-shaped:\n";
        out(std::cerr, S);
        return 1;
      }
      switch (sp) {
      case SP::t : transpositionm(S); break;
      case SP::at : antitranspositionm(S); break;
      case SP::d : S = moddiags2rows(S); break;
      case SP::ad : S = modantidiags2rows(S); break;
      case SP::n2 : S = negationj(S); break;
      case SP::sd : S = sumdiff(S); break;
      case SP::sh : S = shift(S,x,y); break;
      case SP::sc : S = scaling(S,x); break;
      }
      if (vr != VR::s_no_renaming) {
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
    }
    else {
      std::string line;
      std::getline(std::cin, line);
      ls_row_t Q = FloatingPoint::to_vec_unsigned<size_t>(line, ' ');
      if (Q.empty()) return 0;
      switch (sp) {
      case SP::t : Q = qtransposition(Q); break;
      case SP::sh : Q = qshift(Q,x); break;
      case SP::at : [[fallthrough]];
      case SP::d : [[fallthrough]];
      case SP::ad : std::cerr << error
                              << "Transformation not allowed for queens.\n";
        return 1;
      case SP::n2 : Q = qnegationj(Q); break;
      case SP::sd : Q = qsumdiff(Q); break;
      case SP::sc : Q = qscaling(Q,x); break;
      }
      Environment::out_line(std::cout, Q); std::cout << "\n";
    }
  } while (std::cin);
}

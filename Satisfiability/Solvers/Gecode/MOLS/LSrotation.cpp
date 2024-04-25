// Oliver Kullmann, 23.4.2024 (Swansea)
/* Copyright 2024 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Moving rows into columns, diagonals, antidiagonals, and vice versa

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


Moving the diagonals into the rows:
MOLS> echo -e "1 2 3 4\n5 6 7 8\n9 10 11 12\n13 14 15 16" | ./LSrotation_debug -d
1 6 11 16
2 7 12 13
3 8 9 14
4 5 10 15

Moving the rows into the diagonals:
MOLS> echo -e "1 2 3 4\n5 6 7 8\n9 10 11 12\n13 14 15 16" | ./LSrotation_debug -id
1 6 11 16
13 2 7 12
9 14 3 8
5 10 15 4



BUGS:

1. Moving the antidiagonals into the rows:
MOLS> echo -e "1 2 3 4\n5 6 7 8\n9 10 11 12\n13 14 15 16" | ./LSrotation_debug -ad
1 8 11 14
2 5 12 15
3 6 9 16
4 7 10 13

The first row should be the main antidiagonal.

2. iad - moving the rows into the antidiagonals:
MOLS> echo -e "1 2 3 4\n5 6 7 8\n9 10 11 12\n13 14 15 16" | ./LSrotation_debug -iad
1 6 11 16
5 10 15 4
9 14 3 8
13 2 7 12

The main diagonal should be in the main antidiagonal.


TODOS:

1. Is id natural?

2. Perhaps one should just reproduce the empty lines as found
   in the input?

*/

#include <iostream>

#include <cstdlib>

#include <ProgramOptions/Environment.hpp>

#include "BasicLatinSquares.hpp"
#include "LSOptions.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.1",
        "25.4.2024",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Solvers/Gecode/MOLS/LSrotation.cpp",
        "GPL v3"};

  const std::string error = "ERROR[" + proginfo.prg + "]: ";
  constexpr int commandline_args = 1;

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

  std::pair<SR, bool> read_rot(std::string s) noexcept {
    const bool without_stand = s.starts_with("-");
    if (without_stand) s = s.substr(1);
    const auto sr0 = Environment::read<SR>(s);
    if (not sr0) {
      std::cerr << error << "rotation not readable from \""
                << s << "\".\n";
      std::exit(1);
    }
    return {sr0.value(), without_stand};
  }

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  if (argc != commandline_args + 1) {
    std::cerr << error << "Exactly " << commandline_args << " command-line"
      " arguments needed (rot), but the real number is " << argc-1 << ".\n";
    return 1;
  }

  const auto [sr, without_stand] = read_rot(argv[1]);
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
    case SR::id : S = rows2moddiags(S); break;
    case SR::ad : S = modantidiags2rows(S); break;
    case SR::iad : S = rows2modantidiags(S); break;
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

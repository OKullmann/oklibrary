// Oliver Kullmann, 23.4.2024 (Swansea)
/* Copyright 2024 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Reads squares froms standard input, and determines whether
   - they are latin squares, pandiagonal squares
   - row-/column-/diagonal-/antidiagonal-cyclicity.
  Error if they are not all squares of the the same dimension.
*/

#include <iostream>

#include <ProgramOptions/Environment.hpp>

#include "BasicLatinSquares.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.0.9",
        "23.4.2024",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Solvers/Gecode/MOLS/LScyclicity.cpp",
        "GPL v3"};

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  using namespace BasicLatinSquares;

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg << "\n\n"
      "reads squares from standard input, and outputs per square one line of"
      " basic properties.\n\n"
;
    return true;
  }

  void print(std::ostream& out, const ls_t& S) {
    out << ls(S) << pandiagonal(S) << " ";
    Environment::out_line(out, cyclicity(S), "");
    out << std::endl;
  }
}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  ls_t S; size_t count = 0;
  while (std::cin and (S = in_strictls(std::cin)).empty());
  if (not std::cin) return 0;
  const size_t N = S.size();
  if (not sqshape(S)) {
    std::cerr << error << "The first square, with " << N << " rows, is not"
      " square-shaped.\n";
    return 1;
  }
  ++count;
  std::cout << "N " << N << "\n";
  print(std::cout, S);
  do {
    do S = in_strictls(std::cin); while (S.empty() and std::cin);
    if (S.empty()) goto END;
    ++count;
    if (S.size() != N) {
      std::cerr << error << "Square with #rows=" << S.size() << " != "
        " N=" << N << ".\n count " << count << "\n";
      return 1;
    }
    if (not sqshape(S)) {
      std::cerr << error << "Square not square-shaped.\n count " << count
                << "\n";
      return 1;
    }
    print(std::cout, S);
  } while (std::cin);
  END:
  std::cout << "count " << count << "\n";
}

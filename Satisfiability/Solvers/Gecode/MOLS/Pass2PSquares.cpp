// Oliver Kullmann, 6.3.2024 (Swansea)
/* Copyright 2024 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Reads N and filenames of partial assignments from the command-line,
  concatenates the partial assignments, interpreting them as a partial square,
  and outputting either in partial-square-format from OKlibrary, or in
  Minizinc-format.

  So this program outputs a single partial square.
  A useful tool can be passextractpos.awk (combining several v-lines into
  one, and removing the negative literals).

TODOS:

0. Mix-in partial assignments which contain initial assignments,
   and where the other assignments then were run on the result of
   RemoveVarGaps.
   - This assumes the initial assignments cover all variables which
     disappeared (which should be given in our context).
   - Having prefix "@" for the filenames indicating the initial assignments.

1. Remove leading spaces from the output.

2. Use "*" in output for untouched cells.

*/

#include <iostream>
#include <fstream>
#include <tuple>

#include <ProgramOptions/Environment.hpp>
#include <Transformers/Generators/Bicliques/DimacsTools.hpp>
#include <Transformers/Generators/LatinSquares/Commandline.hpp>

#include "PartialSquares.hpp"
#include "Conditions.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.4",
        "9.3.2024",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Solvers/Gecode/MOLS/Pass2PSquares.cpp",
        "GPL v3"};

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  using namespace PartialSquares;
  using namespace Commandline;

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg <<
      " [+][+]N partial-assignment-files*\n\n"
      " - N            : natural number\n"
      "                  \"+\" means Minizinc-output\n"
      "                  \"++\" means reading initial Minizinc from standard input\n"
      " - pass-files   : DIMACS-like format.\n\n"
;
    return true;
  }

  // one or two initial plusses:
  std::tuple<dim_t, bool, bool> readdim(std::string arg,
                                        const std::string& error) {
    const bool has_oneplus = arg.starts_with("+");
    const auto [N,has_twoplus] = read_dim(has_oneplus ? arg.substr(1) : arg,
                                          error);
    return {N, has_oneplus, has_twoplus};
  }

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  if (argc == 1) {
    std::cerr << error << "At least N required.\n";
    return 1;
  }

  const auto [N, oneplus, twoplus] = readdim(argv[1], error);
  DimacsTools::LitSet C;
  for (int i = 2; i < argc; ++i) {
    const std::string filename = argv[i];
    std::ifstream file(filename);
    if (not file) {
      std::cerr << error << "Can not open file \"" << filename << "\".\n";
      return 1;
    }
    DimacsTools::read_pass(file, C);
  }
  const auto S = proto_pass2psquare(C, N);
    // Registration of name "A":
  Conditions::Square::is =
    Environment::indexing_strings(std::vector<std::string>{"A"});
  if (oneplus) {
    if (twoplus) {
      // Assuming the Minizinc-input is small (so storing it for convenience):
      const auto lines = Environment::split(std::cin, '\n');
      bool found_solve = false;
      for (const auto& line : lines)
        if (not line.starts_with("solve ")) std::cout << line << "\n";
        else {
          if (found_solve) {
            std::cerr << error << "Found a second leading \"solve \" in"
              " standard input.\n";
            return 1;
          }
          found_solve = true;
          psquare2minizinc(std::cout, S);
          std::cout << line << "\n";
        }
      if (not found_solve) psquare2minizinc(std::cout, S);;
    }
    else
      psquare2minizinc(std::cout, S);
  }
  else
    std::cout << S;
}

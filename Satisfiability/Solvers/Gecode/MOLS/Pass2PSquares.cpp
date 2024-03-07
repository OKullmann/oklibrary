// Oliver Kullmann, 6.3.2024 (Swansea)
/* Copyright 2024 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

*/

#include <iostream>
#include <fstream>

#include <ProgramOptions/Environment.hpp>
#include <Transformers/Generators/Bicliques/DimacsTools.hpp>
#include <Transformers/Generators/LatinSquares/Commandline.hpp>

#include "PartialSquares.hpp"
#include "Conditions.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.0.7",
        "7.3.2024",
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
      " N partial-assignment-files*\n\n"
      " - N            : natural number\n"
      " - pass-files   : DIMACS-like format.\n\n"
;
    return true;
  }

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  if (argc == 1) {
    std::cerr << error << "At least N required.\n";
    return 1;
  }

  const auto [N, with_plus] = read_dim(argv[1], error);
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
  Conditions::Square::is = Environment::indexing_strings(std::vector<std::string>{"A"});
  std::cout << S;
}

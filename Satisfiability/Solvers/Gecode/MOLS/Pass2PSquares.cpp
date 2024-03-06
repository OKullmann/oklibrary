// Oliver Kullmann, 6.3.2024 (Swansea)
/* Copyright 2024 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

*/

#include <iostream>

#include <ProgramOptions/Environment.hpp>
#include <Transformers/Generators/Bicliques/DimacsTools.hpp>

#include "PartialSquares.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.0.1",
        "6.3.2024",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Solvers/Gecode/MOLS/Pass2PSquares.cpp",
        "GPL v3"};

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  using namespace PartialSquares;

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

}

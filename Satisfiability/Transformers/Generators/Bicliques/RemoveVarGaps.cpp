// Oliver Kullmann, 26.2.2024 (Swansea)
/* Copyright 2024 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  For the input-file F (the one command-line argument), rename the variables
  (monotonically) so that there are no gaps anymore (no formal variables).
  F is assumed to be strict Dimacs, however empty lines after the
  p-line are allowed (so that files produced by ApplyPass-O3-DNDEBUG
  can be used).
*/

#include <iostream>
#include <fstream>

#include <cassert>

#include <ProgramOptions/Environment.hpp>

#include "DimacsTools.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.0.1",
        "27.2.2024",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Bicliques/RemoveVarGaps.cpp",
        "GPL v3"};

  using namespace DimacsTools;

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg
         << " dimacs-file\n\n"
    " renames the variables in the strict Dimacs-file.\n\n"
;
    return true;
  }

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  if (argc != 2) {
    std::cerr << error <<
      "Exactly one argument needed (the strict Dimacs-file), but "
              << argc-1 << " provided.\n";
    return 1;
  }

}

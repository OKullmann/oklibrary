// Oliver Kullmann, 26.2.2024 (Swansea)
/* Copyright 2024 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  For the input-file F (the single command-line argument), rename the variables
  (monotonically) so that there are no gaps anymore (no formal variables).
  F is assumed to be strict Dimacs, however empty lines after the
  p-line are allowed (so that files produced by ApplyPass-O3-DNDEBUG
  can be used).

   - Removes comments.
   - Except for the renaming, the literals in the clauses stay as they
     are (same order as before), but the spacing is cleaned up.
   - Empty lines after the p-line are removed.
   - Any lines after the final clause are removed.

TODOS:

1. Output the renaming-map.
    - Via an option-argument.
    - Which has the following possibilities to state the map:
     - old -> new or new -> old or both
     - as comments, or as additional files.

2. Enable reading from standard input
    - Then output goes to standard output.
    - What to do with the renaming-info? Perhaps here only
      putting it into the comments is allowed.

*/

#include <iostream>
#include <fstream>

#include <cassert>

#include <ProgramOptions/Environment.hpp>
#include <Transformers/Generators/Random/ClauseSets.hpp>

#include "DimacsTools.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.0.10",
        "27.3.2024",
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
    " renames the variables in the strict Dimacs-file (in-place).\n\n"
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
  const std::string filename = argv[1];

  std::ifstream in(filename);
  if (not in) {
    std::cerr << error <<
      "Can not open file \"" << filename << "\" for reading.\n";
    return 1;
  }

  auto F = read_strict_Dimacs(in,
                              AllowancesStrictDimacs::empty_lines_after_pline);
  in.close();

  const auto info = RandGen::rename_clauselist(F.second);
  F.first.n = info.first;

  std::ofstream out(filename);
  if (not out) {
    std::cerr << error <<
      "Can not open file \"" << filename << "\" for writing.\n";
    return 1;
  }
  out << F;
}

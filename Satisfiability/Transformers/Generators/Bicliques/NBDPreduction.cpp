// Oliver Kullmann, 31.5.2025 (Swansea)
/* Copyright 2025 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Reading a NB-CNF in NOBOCONF format, and performing DP-reduction for
  a sequence of variables

*/

#include <iostream>
#include <exception>

#include <cassert>

#include <ProgramOptions/Environment.hpp>
#include <Numerics/NumInOut.hpp>

#include <Transformers/Generators/Random/GenClauseSets.hpp>

#include "GenResolution.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
    "0.0.6",
    "31.5.2025",
    __FILE__,
    "Oliver Kullmann",
    "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Bicliques/NBDPreduction.cpp",
    "GPL v3"};

  using namespace GenResolution;

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg
         << " V [sets] [subsumption]\n\n"
    " V              : list of variables, comma-separated\n"
    " [sets]         : default 0 (false)\n"
    " [subsumption]  : default 0 (false)\n\n"
    " reads a NB-CNF from standard-input, and prints it to"
    " standard output.\n\n"
;
    return true;
  }

  bool to_bool(const std::string& s) noexcept { return s != "0"; }

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  if (argc == 1) {
    std::cerr << error <<
      "At least one argument (V [sets] [subsumption])"
      " needed, but none provided.\n";
    return 1;
  }

  const varlist_t V = FloatingPoint::to_vec_unsigned<GenLit::var_t>(argv[1], ',');
  const bool clauseset = argc >= 3 ? to_bool(argv[2]) : false;
  const bool subsumption = argc >= 4 ? to_bool(argv[3]) : false;

  GenClauseSets::GClauseList F;
  try { std::cin >> F; }
  catch (std::exception& e) {
    std::cerr << error << "SYNTAX ERROR with clause-list:\n"
              << e.what() << "\n";
    return 1;
  }
  assert(F.valid());
  if (clauseset) F.fully_standardise();
  else F.standardise();

  const auto stats = DP_reduction(F, V, clauseset, subsumption);
  std::cout << GenClauseSets::comchar << "red-aed ";
  Environment::out_line(std::cout, stats);
  std::cout << std::endl;

  std::cout << F;
}

// Oliver Kullmann, 24.5.2025 (Swansea)
/* Copyright 2025 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Reading a NB-CNF in NOBOCONF format, and printing it after various
  modifications


EXAMPLES:

Random> echo -e "C here is starts\n1:2 2:4 0 first clause\n 4:2  0:1 0 second clause\nC comments everywhere" | ./NBCNFConversion
n 5
c 2
1:2 2:4 0
4:2 0:1 0


*/

#include <iostream>

#include <ProgramOptions/Environment.hpp>

#include "GenLit.hpp"
#include "GenClauses.hpp"
#include "GenClauseSets.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
    "0.0.5",
    "24.5.2025",
    __FILE__,
    "Oliver Kullmann",
    "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/NBCNFConversion.cpp",
    "GPL v3"};

  using namespace GenLit;
  using namespace GenClauses;
  using namespace GenClauseSets;

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg
         << "\n\n"
    " reads a NB-CNF from standard-input, and prints it to"
    " standard output.\n\n"
;
    return true;
  }

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  const bool standardise = argc > 1;

  GClauseList F;
  std::cin >> F;
  if (standardise) {
    const auto stats = F.fully_standardise();
    std::cout << comchar << " ";
    Environment::out_line(std::cout, stats);
    std::cout << "\n";
  }
  std::cout << F;
}

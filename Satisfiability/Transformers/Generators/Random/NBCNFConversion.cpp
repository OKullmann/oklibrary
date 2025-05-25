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


Full standardisation (removal of tautological clauses and repeated literals,
sorting of clauses (lexicographically) and of the clause-list (antilexico-
graphically)), and showing application of n-lines:

Random> echo -e "C here is starts\n1:2 2:4 1:2 0 first clause, with repetitions\n 4:2  0:1 0 second clause\nC comments everywhere\n5:1 5:17 0 tautology\nn 10 with formal variables" | ./NBCNFConversion l
C 1 3
n 10
c 2
1:2 2:4 0
0:1 4:2 0

The first comment shows the number of eliminated clauses and the total number
of eliminated literal-occurrences.

*/

#include <iostream>
#include <exception>

#include <ProgramOptions/Environment.hpp>

#include "GenLit.hpp"
#include "GenClauses.hpp"
#include "GenClauseSets.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
    "0.0.6",
    "25.5.2025",
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
         << "[X] \n\n"
    " X          : any argument means full standardisation\n"

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
  try { std::cin >> F; }
  catch (std::exception& e) {
    std::cerr << error << "SYNTAX ERROR:\n" << e.what() << "\n";
    return 1;
  }
  if (standardise) {
    const auto stats = F.fully_standardise();
    std::cout << comchar << " ";
    Environment::out_line(std::cout, stats);
    std::cout << "\n";
  }
  std::cout << F;
}

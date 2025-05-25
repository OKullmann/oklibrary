// Oliver Kullmann, 24.5.2025 (Swansea)
/* Copyright 2025 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Reading a NB-CNF in NOBOCONF format, and printing it (same format)
  after various modifications.


EXAMPLES:

Random> echo -e "C here is starts\n1:2 2:4 0 first clause\n 4:2  0:1 0 second clause\nC comments everywhere" | ./NBCNFConversion
C 0110
n 5
c 2
1:2 2:4 0
4:2 0:1 0

The first comment shows (0 means false):
 - sorted clauses?
 - no (consecutive) duplicated literals in clauses?
 - no (consecutive) clashes in clauses?
 - sorted clause-list?
This is refers to the original input.

If clauses are sorted, then duplicated or clashing literals must occur
consecutively, if they occur.

Same as standardisation-level 0:
Random> echo -e "C here is starts\n1:2 2:4 0 first clause\n 4:2  0:1 0 second clause\nC comments everywhere" | ./NBCNFConversion 0

The three standardisation-levels are:
  level 0 : as is
  level 1 : removal of tautological clauses and repeated literals, and
            sorting of clauses, lexicographically
  level 2: additionally sorting of the clause-list, antilexicographically.

Thus level 1 standardises all clauses, but does not sort the whole clause-list:
Random> echo -e "C here is starts\n2:4 1:2 5:4 0 first clause\n 4:2  0:1 0 second clause\nC comments everywhere" | ./NBCNFConversion 1
C 0110
C 0 0
n 6
c 2
1:2 2:4 5:4 0
0:1 4:2 0

The second comment shows the number of eliminated clauses and the total number
of eliminated literal-occurrences.

Showing the application of n-lines:
Random> echo -e "C here is starts\n1:2 2:4 1:2 5:4 0 first clause, with repetitions\n 4:2  0:1 0 second clause\nC comments everywhere\n5:1 5:17 0 tautology\nn 10 with formal variables" | ./NBCNFConversion 1
C 0100
C 1 3
n 10
c 2
1:2 2:4 5:4 0
0:1 4:2 0

Now full standardisation, and the application of c-lines:

Random> echo -e "C here is starts\nc 4\n1:2 2:4 1:2 5:4 0 first clause, with repetitions\n 4:2  0:1 0 second clause\nC comments everywhere\n20:0 20:1 0 tautology\n2:3 0 unit\nn 10 with formal variables (no effect here)" | ./NBCNFConversion 2
C 0100
C 1 1
n 21
c 3
2:3 0
0:1 4:2 0
1:2 2:4 5:4 0

We see that n and c include the tautological clauses (even if they get removed).

*/

#include <iostream>
#include <exception>

#include <cassert>

#include <ProgramOptions/Environment.hpp>

#include "GenLit.hpp"
#include "GenClauses.hpp"
#include "GenClauseSets.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
    "0.1.1",
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
         << "[L] \n\n"
    " L          : standardisation-level 0, 1, 2\n"

    " reads a NB-CNF from standard-input, and prints it to"
    " standard output.\n\n"
;
    return true;
  }

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  const unsigned std_level = argc == 1 ? 0 : std::stoul(argv[1]);

  GClauseList F;
  try { std::cin >> F; }
  catch (std::exception& e) {
    std::cerr << error << "SYNTAX ERROR:\n" << e.what() << "\n";
    return 1;
  }
  assert(F.valid());
  std::cout << comchar << " " << F.clauses_sorted()
            << not F.has_consecutive_duplicates() << F.no_consecutive_clashes()
            << F. clauselist_sorted() << std::endl;

  if (std_level > 0) {
    const auto stats = std_level == 1 ? F.standardise() : F.fully_standardise();
    std::cout << comchar << " ";
    Environment::out_line(std::cout, stats);
    std::cout << "\n";
  }
  std::cout << F;
}

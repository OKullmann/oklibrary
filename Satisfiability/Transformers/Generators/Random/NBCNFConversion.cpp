// Oliver Kullmann, 24.5.2025 (Swansea)
/* Copyright 2025 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Reading a NB-CNF in NOBOCONF format, and printing it (same format)
  after various modifications.

  See Generators/Bicliques/NBDPreduction for a program performing DP-reduction
  (can also be used to just perform subsumption-elimination).


EXAMPLES:

Random> echo -e "C here is starts\n1:2 2:4 0 first clause\n 4:2  0:1 0 second clause\nC comments everywhere" | ./NBCNFConversion
Clevel         0
Cflags         0110
n 5
c 2
1:2 2:4 0
4:2 0:1 0

With domain-sizes:
Random> echo -e "C here is starts\n1:2 2:4 0 first clause\n 4:2  0:1 0 second clause\nC comments everywhere" | ./NBCNFConversion 0 x
Clevel          0
Cflags          0110
n 5
c 2
1:2 2:4 0
4:2 0:1 0
Cv 0            2
Cv 1            3
Cv 2            5
Cv 3            0
Cv 4            3

The first comment shows the standardisation-level, the second comment shows
basic properties (where "0" means "false"):
 - sorted clauses?
 - no (consecutive) duplicated literals in clauses?
 - no (consecutive) clashes in clauses?
 - sorted clause-list?
This refers to the original input.

Duplicated or clashing literals are only recognised where occurring
consecutively (if they occur at all); for sorted clauses that catches
everything.

Same as standardisation-level 0:
Random> echo -e "C here is starts\n1:2 2:4 0 first clause\n 4:2  0:1 0 second clause\nC comments everywhere" | ./NBCNFConversion 0

The found standardisation-levels are:
  level 0 : as is
  level 1 : removal of tautological clauses and repeated literals, and
            sorting of clauses, lexicographically
  level 2 : additionally sorting of the clause-list, antilexicographically
  level 3 : additionally removing duplicated clauses (thus realising "clause-sets").

Thus level 1 standardises all clauses, but does not sort the whole clause-list:
Random> echo -e "C here is starts\n2:4 1:2 5:4 0 first clause\n 4:2  0:1 0 second clause\nC comments everywhere" | ./NBCNFConversion 1
Clevel          1
Cflags          0110
Cred-cl         0 0
n 6
c 2
1:2 2:4 5:4 0
0:1 4:2 0

The third comment shows the number of eliminated clauses and the number
of eliminated duplicated literal-occurrences (here nothing was reduced, and
thus the numbers are zero).

Showing the application of n-lines:
Random> echo -e "C here is starts\n1:2 2:4 1:2 5:4 0 first clause, with repetitions\n 4:2  0:1 0 second clause\nC comments everywhere\n5:1 5:17 0 tautology\nn 10 with formal variables" | ./NBCNFConversion 1
Clevel          1
Cflags          0100
Cred-cl         1 1
n 10
c 2
1:2 2:4 5:4 0
0:1 4:2 0

Now full standardisation, and the application of c-lines (just for checking):

Random> echo -e "C here is starts\nc 4\n1:2 2:4 1:2 5:4 0 first clause, with repetitions\n 4:2  0:1 0 second clause\nC comments everywhere\n20:0 20:1 0 tautology\n2:3 0 unit\nn 10 with formal variables (no effect here)" | ./NBCNFConversion 2
Clevel          2
Cflags          0100
Cred-cl         1 1
n 21
c 3
2:3 0
0:1 4:2 0
1:2 2:4 5:4 0

We see that n and c include the tautological clauses (even if they get
removed).
And the variable 20, which occurred only in the tautological (thus removed)
clause still has the variable-degree 2 (all variables get the smallest
degree consistent with the given data).
Any second argument triggers the output of the variable-statistics (for
levels >= 1):
Random> echo -e "c 4\n1:2 2:4 1:2 5:4 0\n 4:2  0:1 0\n10:0 10:1 0\n2:3 0" | ./NBCNFConversion 2 x
Clevel          2
Cflags          0100
Cred-cl         1 1
n 11
c 3
2:3 0
0:1 4:2 0
1:2 2:4 5:4 0
Cv 0            2 1 0010000
Cv 1            3 1 0010000
Cv 2            5 2 0010000
Cv 3            0 0 1000000
Cv 4            3 1 0010000
Cv 5            5 1 0010000
Cv 6            0 0 1000000
Cv 7            0 0 1000000
Cv 8            0 0 1000000
Cv 9            0 0 1000000
Cv 10           2 0 0100000

The statistics of variable v are:
 - domain-size of v
 - degree (number of occurrences of v)
 - 7 flags ("0" being "false", "1" being "true):
  - formal (domain-size 0)
  - trivial (domain-size >= 1, but not occurring)
  - pure (neither formal nor trivial, but with one value
    not occurring (which then must be strictly less than the
    maximum-value occurring)
  - 1-singular (not trivial, and all values occurring exactly once)
  - singular (all-but-one values occurring exactly once, the remaing
    value occurring at least once)
  - non-1-singular (singular, but not 1-singular)
  - non-singular (all values occurring at least once, and at least
    two values occurring at least twice).

Finally, std-level 3 realises "clause-sets", by also removing duplicated
clauses:
Random> echo -e "3:4 2:3 1:2 2:3 0\n0:1 0:2 0\n1:2 3:4 2:3 0\n" | ./NBCNFConversion 3 x
Clevel          3
Cflags          0100
Cred-ccl        1 1 1
n 4
c 1
1:2 2:3 3:4 0
Cv 0            3 0 0100000
Cv 1            3 1 0010000
Cv 2            4 1 0010000
Cv 3            5 1 0010000

Here the reduction-information is: removed tautological clauses, removed
duplicated clauses, removed duplicated literal-occurrences.

*/

#include <iostream>
#include <exception>
#include <algorithm>

#include <cassert>
#include <cstdlib>

#include <ProgramOptions/Environment.hpp>
#include <Numerics/NumInOut.hpp>
#include <Transformers/Generators/Bicliques/GenConflictGraphs.hpp>

#include "GenLit.hpp"
#include "GenClauses.hpp"
#include "GenClauseSets.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
    "0.3.1",
    "1.6.2025",
    __FILE__,
    "Oliver Kullmann",
    "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/NBCNFConversion.cpp",
    "GPL v3"};

  using namespace GenLit;
  using namespace GenClauses;
  using namespace GenClauseSets;

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  const unsigned initial_width = 16;
  typedef std::vector<count_t> datalist_t;

  constexpr unsigned max_std_level = 3;

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg
         << " [L] [x]\n\n"
    " L          : standardisation-level 0, 1, ... " << max_std_level << "\n"
    " x          : any second argument triggers variables-statistics"
    " for levels >= 1\n\n"
    " reads a NB-CNF from standard-input, and prints it to"
    " standard output.\n\n"
;
    return true;
  }

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  const unsigned std_level = argc == 1 ? 0 : std::min(max_std_level, [&]{
    try { return FloatingPoint::to_unsigned<unsigned>(argv[1]); }
    catch (const std::exception& e) {
      std::cerr << error << "SYNTAX ERROR with level-argument:\n"
                << e.what() << "\n";
      std::exit(1); }}());
  out_datacomment(std::cout, "level", initial_width, datalist_t{std_level});
  std::cout << std::endl;

  const bool with_var_stats = argc > 2;

  GClauseList F;
  try { std::cin >> F; }
  catch (std::exception& e) {
    std::cerr << error << "SYNTAX ERROR with clause-list:\n"
              << e.what() << "\n";
    return 1;
  }
  assert(F.valid());
  out_datacomment(std::cout, "flags", initial_width, datalist_t{
                    F.clauses_sorted(), not F.has_consecutive_duplicates(),
                      F.no_consecutive_clashes(), F. clauselist_sorted()}, "");
  std::cout << std::endl;

  if (std_level >= 1) {
    if (std_level <= 2) {
      const auto stats = std_level == 1 ? F.standardise() : F.fully_standardise();
      out_datacomment(std::cout, "red-cl", initial_width, stats);
      std::cout << std::endl;
    }
    else {
      const auto stats = F.make_clauseset();
      out_datacomment(std::cout, "red-ccl", initial_width, stats);
      std::cout << std::endl;
    }
  }
  std::cout << F;

  if (with_var_stats) {
    if (std_level == 0) {
      for (var_t v = 0; v < F.n(); ++v) {
        const std::string header = std::string("v ") + std::to_string(v);
        out_datacomment(std::cout, header, initial_width, datalist_t{F.dom[v]});
        std::cout << "\n";
      }
    }
    else {
      for (var_t v = 0; v < F.n(); ++v) {
        const std::string header = std::string("v ") + std::to_string(v);
        const auto o = GenConflictGraphs::GOccVar(F, v);
        out_datacomment(std::cout, header, initial_width,
                        datalist_t{F.dom[v], o.deg()});
        std::cout << " " << o.formal() << o.trivial() << o.pure()
                  << o.osingular() << o.singular() << o.nonosingular() << o.nonsingular();
        std::cout << std::endl;
      }
    }
  }
}

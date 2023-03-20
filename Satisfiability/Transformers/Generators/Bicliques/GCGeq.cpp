// Oliver Kullmann, 18.3.2023 (Swansea)
/* Copyright 2023 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Checking whether two QCNF's are gcg-equivalent

EXAMPLES:

Bicliques> echo -e "p cnf 4 2\na 3 4 0\ne 1 2 0\n1 2 3 4 0\n-1 -2 -3 -4 0\np cnf 3 2\na 3 0\ne 1 2 0\n1 2 3 0\n-1 -2 -3 0\n" | ./GCGeq_debug
# 1: O = p cnf 2 2
# 1: G = p cnf 4 2
# 2: O = p cnf 2 2
# 2: G = p cnf 3 2
Bicliques> echo $?
0

The log-output shows first the dimacs-parameters of the first input,
split into the other-part and the global-part, and then the same for the
second input.
There is no further output in case of equivalence, which is signalled by
return-code 0.

Bicliques> echo -e "p cnf 4 2\na 3 4 0\ne 1 2 0\n1 2 3 4 0\n-1 -2 -3 -4 0\np cnf 3 2\na 3 0\ne 1 2 0\n1 2 3 0\n-1 -2 3 0\n" | ./GCGeq_debug
# 1: O = p cnf 2 2
# 1: G = p cnf 4 2
# 2: O = p cnf 2 2
# 2: G = p cnf 3 2
sizes of components differ
Bicliques> echo $?
2

Here we have non-equivalence, signalled by a return-code 2, and there
is some explanation on the difference.

Bicliques> echo -e "p cnf 4 2\na 3 4 0\ne 1 2 0\n1 2 3 4 0\n-1 -2 -3 -4 0\np cnf 3 2\na 3 3 0\ne 1 2 0\n1 2 3 0\n-1 -3 0\n" | ./GCGeq_debug
# 1: O = p cnf 2 2
# 1: G = p cnf 4 2
# 2: O = p cnf 2 2
# 2: G = p cnf 3 2
other-parts different
  clauses with index 2 differ:
-1 -2 0
-1 0
Bicliques> echo $?
1


Remarks:

If the inputs are in files A, B, then use
Bicliques> cat A B | ./GCGeq_debug

Also the final line of the (strict) QCNF must be concluded by end-of-line.
If say both files don't have them, use
> echo > EOL
> cat A EOL B EOL | ./GCGeq_debug

Above the debug-versions are used (shown by "_debug").


TODOS:

1. Output showing the differences
2. Help should show the meaning of the return-codes.

*/

#include <iostream>

#include <ProgramOptions/Environment.hpp>

#include "DimacsTools.hpp"
#include "Bicliques2SAT.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.0.6",
        "20.3.2023",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Bicliques/GCGeq.cpp",
        "GPL v3"};

  using namespace DimacsTools;
  using namespace Bicliques2SAT;

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg << "\n\n"
    " reads two strict QDimacs-file from standard input, and returns their gcg-equivalence status via exit-code.\n\n"
;
    return true;
  }

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  const auto F1 = read_strict_GslicedCNF(std::cin);
  std::cout << "# 1: O = " << F1.O().first;
  std::cout << "# 1: G = " << F1.G().first;
  const auto F2 = read_strict_GslicedCNF(std::cin);
  std::cout << "# 2: O = " << F2.O().first;
  std::cout << "# 2: G = " << F2.G().first;
  const GlobRepl G1(F1), G2(F2);
  const auto res = gcg_equivalence(G1, G2, &std::cout);
  return int(res);
}

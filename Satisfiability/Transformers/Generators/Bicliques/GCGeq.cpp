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
# 0: O = p cnf 2 2
# 0: G = p cnf 4 2
# 0: n_g = 2
# 1: O = p cnf 2 2
# 1: G = p cnf 3 2
# 1: n_g = 1
Bicliques> echo $?
0

The log-output shows first the dimacs-parameters of the first input,
split into the other-part and the global-part, followed by the (real) number
of global variables, and then the same for the second input.
There is no further output in case of equivalence, which is signalled by
return-code 0.

Via a "+" for the number of inputs one can request strict checking, that is,
whether the other number-of-global variables are equal and not larger than the
the first one:

Bicliques> echo -e "p cnf 3 2\na 3 0\ne 1 2 0\n1 2 3 0\n-1 -2 -3 0\np cnf 4 2\na 3 4 0\ne 1 2 0\n1 2 3 4 0\n-1 -2 -3 -4 0\n" | ./GCGeq_debug +2
# 0: O = p cnf 2 2
# 0: G = p cnf 3 2
# 0: n_g = 1
# 1: O = p cnf 2 2
# 1: G = p cnf 4 2
# 1: n_g = 2
n_g = 2 > base_value = 1
Bicliques> echo $?
105

Bicliques> echo -e "p cnf 4 2\na 3 4 0\ne 1 2 0\n1 2 3 4 0\n-1 -2 -3 -4 0\np cnf 3 2\na 3 0\ne 1 2 0\n1 2 3 0\n-1 -2 -3 0\np cnf 4 2\na 3 4 0\ne 1 2 0\n1 2 3 4 0\n-1 -2 -3 -4 0\n" | ./GCGeq_debug +3
# 0: O = p cnf 2 2
# 0: G = p cnf 4 2
# 0: n_g = 2
# 1: O = p cnf 2 2
# 1: G = p cnf 3 2
# 1: n_g = 1
# 2: O = p cnf 2 2
# 2: G = p cnf 4 2
# 2: n_g = 2
n_g = 2 != opt_value = 1
Bicliques> echo $?
106



Some examples of non-gcg-equivalence:

Bicliques> echo -e "p cnf 4 2\na 3 4 0\ne 1 2 0\n1 2 3 4 0\n-1 -2 -3 -4 0\np cnf 3 2\na 3 0\ne 1 2 0\n1 2 3 0\n-1 -2 3 0\n" | ./GCGeq_debug
# 0: O = p cnf 2 2
# 0: G = p cnf 4 2
# 0: n_g = 2
# 1: O = p cnf 2 2
# 1: G = p cnf 3 2
# 1: n_g = 1
sizes of connected components differ
Bicliques> echo $?
102

Here we have non-equivalence, signalled by a return-code 102, and there
is some explanation on the difference.

Bicliques> echo -e "p cnf 4 2\na 3 4 0\ne 1 2 0\n1 2 3 4 0\n-1 -2 -3 -4 0\np cnf 3 2\na 3 3 0\ne 1 2 0\n1 2 3 0\n-1 -3 0\n" | ./GCGeq_debug
# 0: O = p cnf 2 2
# 0: G = p cnf 4 2
# 0: n_g = 2
# 1: O = p cnf 2 2
# 1: G = p cnf 3 2
# 1: n_g = 1
other-parts different
  clauses with index 2 differ:
-1 -2 0
-1 0
Bicliques> echo $?
101


Remarks:

If the inputs are in files A, B, then use
Bicliques> cat A B | ./GCGeq_debug

Also the final line of the (strict) QCNF must be concluded by end-of-line.
If say both files don't have them, use
> echo > EOL
> cat A EOL B EOL | ./GCGeq_debug

Above the debug-versions are used (shown by "_debug").


See plans/general.txt.

*/

#include <iostream>

#include <cstdlib>

#include <ProgramOptions/Environment.hpp>

#include "BCC2SAT.hpp"
#include "DimacsTools.hpp"
#include "Bicliques2SAT.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.2.0",
        "30.4.2023",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Bicliques/GCGeq.cpp",
        "GPL v3"};

  using namespace BCC2SAT;
  using namespace DimacsTools;
  using namespace Bicliques2SAT;

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  typedef FloatingPoint::uint_t uint_t;
  constexpr uint_t default_num_inputs = 2;

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
      "> " << proginfo.prg << " [num-input=" << default_num_inputs << "]\n\n"
      " reads num-input-many strict QDimacs-files from standard input,"
      " and returns their gcg-equivalence status via exit-code:\n\n"
    "  - All other files are compared to the first.\n"
    "  - A \"+\" means partial checking of optimality of the other files.\n\n"
;
    return true;
  }


  auto read_next(const GlobRepl& G0, const uint_t i) {
    const auto F = read_strict_GslicedCNF(std::cin);
    const auto res = F.V.size();
    std::cout << "# " << i << ": O = " << F.O().first;
    std::cout << "# " << i << ": G = " << F.G().first;
    std::cout << "# " << i << ": n_g = " << res << "\n";
    const GlobRepl G(F);
    const auto comp = gcg_equivalence(G0, G, &std::cout);
    if (comp != GCGE::eq) std::exit(int(comp));
    return res;
  }

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  if (argc > 2) {
    std::cerr << error <<
      "More than one arguments provided, namely " << argc-1 << ".\n";
    return int(Error::too_many_parameters);
  }

  const auto [num_inputs, strict_checking] =
    argc == 1 ? valoror_t{default_num_inputs, false} : read_valorinc(argv[1]);
  if (num_inputs == 0) return 0;

  const auto F0 = read_strict_GslicedCNF(std::cin);
  const auto base_value = F0.V.size();
  std::cout << "# 0: O = " << F0.O().first;
  std::cout << "# 0: G = " << F0.G().first;
  std::cout << "# 0: n_g = " << base_value << "\n";
  if (num_inputs == 1) return 0;
  const GlobRepl G0(F0);

  const auto opt_value = read_next(G0, 1);
  if (strict_checking and opt_value > base_value) {
    std::cout << "n_g = " << opt_value << " > base_value = " << base_value <<
      "\n";
    return int(GCGE::greater_than_base);
  }
  if (num_inputs == 2) return 0;

  for (uint_t i = 2; i < num_inputs; ++i) {
    const auto value = read_next(G0, i);
    if (strict_checking) {
      if (value > base_value) {
        std::cout << "n_g = " << value << " > base_value = " << base_value <<
          "\n";
        return int(GCGE::greater_than_base);
      }
      if (value != opt_value)  {
        std::cout << "n_g = " << value << " != opt_value = " << opt_value <<
          "\n";
        return int(GCGE::diverging_values);
      }
    }
  }
}

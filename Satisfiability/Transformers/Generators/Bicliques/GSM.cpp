// Oliver Kullmann, 20.3.2023 (Swansea)
/* Copyright 2023 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Global Slice Minimisation

  The original clause-order is kept as well as the other-variables,
  while the replacement-global variables re-use (per connected component)
  the non-pure global variables in numerical order.
  The output-clauses are sorted per slice, with the other-slice first,
  the dimacs-parameters are exact, and exactly the occurring new global
  variables are listed.


EXAMPLES:

Bicliques> cat data/Example_00.qcnf
p cnf 7 6
a 3 5 7 0
e 1 2 4 6 0
3 2 -4 0
-3 5 1 6 0
-2 -4 0
-5 -6 0
7 2 4 6 0
-7 -2 -4 -6 0
Bicliques> cat data/Example_00.qcnf | ./GSM_debug "" "" "" "" > R.qcnf
Bicliques> cat R.qcnf
p cnf 7 6
a 3 7 0
e 1 2 4 6 0
2 -4 3 0
1 6 -3 0
-2 -4 0
-6 3 0
2 4 6 7 0
-2 -4 -6 -7 0
Bicliques> cat data/Example_00.qcnf R.qcnf | ./GCGeq_debug
# 1: O = p cnf 6 6
# 1: G = p cnf 7 6
# 1: n_g = 3
# 2: O = p cnf 6 6
# 2: G = p cnf 7 6
# 2: n_g = 2


Bicliques> cat data/Example_01.qcnf
p cnf 9 6
a 8 9 3 5 7 0
e 1 2 4 6 0
9 3 2 -4 0
9 -3 5 1 6 0
-2 -4 0
-5 -6 0
7 2 4 6 8 0
-7 -2 -4 -6 0
Bicliques> cat data/Example_01.qcnf | ./GSM_debug "" "" "" "" > R.qcnf
Bicliques> cat R.qcnf
p cnf 7 6
a 3 7 0
e 1 2 4 6 0
2 -4 3 0
1 6 -3 0
-2 -4 0
-6 3 0
2 4 6 7 0
-2 -4 -6 -7 0
Bicliques> cat data/Example_01.qcnf R.qcnf | ./GCGeq_debug
# 1: O = p cnf 6 6
# 1: G = p cnf 9 6
# 1: n_g = 5
# 2: O = p cnf 6 6
# 2: G = p cnf 7 6
# 2: n_g = 2

Bicliques> cat data/Example_02.qcnf
p cnf 111 6
a 8 9 3 5 111 7 0
e 1 2 4 6 0
9 3 2 -4 0
-9 3 5 1 6 0
-2 -4 0
5 -6 9 0
7 2 4 6 8 0
-7 -2 -4 -6 0
Bicliques> cat data/Example_02.qcnf | ./GSM_debug "" "" "" "" > R.qcnf
Bicliques> cat R.qcnf
p cnf 9 6
a 7 9 0
e 1 2 4 6 0
2 -4 9 0
1 6 -9 0
-2 -4 0
-6 9 0
2 4 6 7 0
-2 -4 -6 -7 0
Bicliques> cat data/Example_01.qcnf R.qcnf | ./GCGeq_debug
Bicliques> cat data/Example_02.qcnf R.qcnf | ./GCGeq_debug
# 1: O = p cnf 6 6
# 1: G = p cnf 9 6
# 1: n_g = 5
# 2: O = p cnf 6 6
# 2: G = p cnf 9 6
# 2: n_g = 2

We note here that trivial global variables (not occurring at all) are ignored.


See plans/general.txt.

*/

#include <iostream>
#include <fstream>

#include <ProgramOptions/Environment.hpp>
#include <Transformers/Generators/Random/Numbers.hpp>

#include "DimacsTools.hpp"
#include "Bicliques2SAT.hpp"

#include "BCC2SAT.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.3.1",
        "29.3.2023",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Bicliques/GSM.cpp",
        "GPL v3"};

  using namespace Bicliques2SAT;
  using namespace BCC2SAT;

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg
         << " algo-options sb-rounds timeout seeds\n\n"
    " algo-options   : " << Environment::WRP<SB>{} << "\n"
    "                : " << Environment::WRP<PT>{} << "\n"
    "                : " << Environment::WRP<DI>{} << "\n"
    "                : " << Environment::WRP<SO>{} << "\n"
    " sb-rounds      : " << "default is " << default_sb_rounds << "\n"
    " timeout        : " << "in s, default is " << default_sec << "\n"
    " seeds          : " << "sequence, can contain \"t\" or \"r\"" << "\n\n"
    " reads a cnf from standard input, and attempts to compute an optimal representation:\n\n"
    "  - Arguments \"\" (the empty string) yield the default-values.\n"
    "  - Default-values for the options are the first possibilities given.\n"
    "  - The representation is exact (for the conflict-*graph*) iff considering bcp.\n\n"
;
    return true;
  }

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  if (argc != 5) {
    std::cerr << error <<
      "Exactly four arguments (algo-opt, sb-rounds, timeout, seeds)"
      " needed, but " << argc-1 << " provided.\n";
    return int(Error::missing_parameters);
  }

  const alg2_options_t algopt =
    Environment::translate<alg2_options_t>()(argv[1], sep);
  const var_t sb_rounds = read_var_t(argv[2], default_sb_rounds);
  const auto sec = read_uint_t(argv[3], default_sec);
  const RandGen::vec_eseed_t seeds = RandGen::extract_seeds(argv[4]);

  if (std::get<SB>(algopt) != SB::none and sb_rounds == 0) {
    std::cerr << error <<
      "Symmetry-breaking on, but number of rounds is zero.\n";
    return int(Error::bad_sb);
  }
  if (std::get<SB>(algopt) == SB::extended) {
    std::cerr << error <<
      "Extended symmetry-breaking not implemented yet.\n";
    return int(Error::bad_sb);
  }

  const auto F = DimacsTools::read_strict_GslicedCNF(std::cin);
  const GlobRepl GR(F);
  const auto R = GR.solve(nullptr, algopt, sb_rounds, sec, seeds);
  const DimacsTools::GslicedCNF Fnew(F.O(), R, F.other);
  std::cout << Fnew;
}

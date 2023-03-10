// Oliver Kullmann, 7.3.2022 (Swansea)
/* Copyright 2022, 2023 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  For input-CNF F, compute another CNF F' which has the same conflict-graph
  and the minimal number of variables.


EXAMPLES:

Bicliques> echo -e "p cnf 100 5\n0\n1 2 0\n3 4 0\n -3 5 0\n-5 0\n" | ./CNFBCC "" "" "" ""
p cnf 1 5
0
0
1 0
-1 0
1 0


Symmetry-breaking is randomised, and thus for example (using "t" for
"timestampe")

Bicliques> echo -e "p cnf 2 3\n1 2 0\n1 -2 0\n-1 -2 0\n" | ./CNFBCC "" "" "" t

yields one of (trimmed) realisations:

p cnf 2 3
1 2 0
1 -2 0
-1 0
or
p cnf 2 3
1 2 0
-1 0
1 -2 0
or
p cnf 2 3
1 2 0
-1 0
1 -2 0
or
p cnf 2 3
1 -2 0
1 2 0
-1 0


BUGS:

1.
Bicliques> echo -e "p cnf 2 4\n1 2 0\n1 -2 0\n-1 -2 0\n-1 2 0\n" | ./CNFBCC_debug partition2 "" "" t
CNFBCC_debug: CNFBCC.cpp:126: int main(int, const char* const*): Assertion `res.rt != ResultType::init_unsat_sb and res.rt != ResultType::init_unsat and res.rt != ResultType::unknown' failed.

The start-value is not sufficient here, since one doesn't start with a
precise realisation.

*/


#include <iostream>

#include <ProgramOptions/Environment.hpp>
#include <Transformers/Generators/Random/Numbers.hpp>

#include "Bicliques.hpp"
#include "DimacsTools.hpp"
#include "ConflictGraphs.hpp"
#include "Bicliques2SAT.hpp"

#include "BCC2SAT.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.2.1",
        "10.3.2023",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Bicliques/CNFBCC.cpp",
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
      " needed, but only " << argc-1 << " provided.\n";
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

  const auto F = DimacsTools::read_strict_Dimacs(std::cin);
  const auto G = ConflictGraphs::conflictgraph_bydef(F);
  assert(G.n() == F.first.c);
  BC2SAT T(G, std::min(F.first.n, G.m()));
  const auto res = T.sat_solve(nullptr, algopt, sb_rounds, sec, seeds);
  assert(res.rt != ResultType::init_unsat_sb and
         res.rt != ResultType::init_unsat and
         res.rt != ResultType::unknown);
  std::cout << Bicliques::bcc2CNF(res.bcc, F.first.c);

}

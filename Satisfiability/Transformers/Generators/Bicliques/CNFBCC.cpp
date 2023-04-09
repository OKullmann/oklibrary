// Oliver Kullmann, 7.3.2022 (Swansea)
/* Copyright 2022, 2023 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  For input-CNF F, compute another CNF F' which has the same conflict-graph
  and the minimal number of variables.
  The clauses of F are sorted.

  No splitting into connected components is performed (see GSM for this
  more general handling).


EXAMPLES:

Bicliques> echo -e "p cnf 100 5\n0\n1 2 0\n3 4 0\n -3 5 0\n-5 0\n" | ./CNFBCC "" "" "" "" "" ""
p cnf 1 5
0
0
1 0
-1 0
1 0


Symmetry-breaking is randomised, and thus for example (using "t" for
"timestampe")

Bicliques> echo -e "p cnf 2 3\n1 2 0\n1 -2 0\n-1 -2 0\n" | ./CNFBCC "" "" "" t "" ""

yields one of the following (trimmed) realisations:

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

Remarks:

The fifth argument can be a filename or /dev/stdout for logging information
on the solution-process.

For example
Bicliques> BRG "20*15,3" | ./CNFBCC "" "" "" "" logfile ""

p cnf 10 20
1 0
-1 -9 0
7 9 0
-2 5 7 -9 0
-1 4 0
6 -7 8 0
-4 5 7 8 0
-3 5 6 7 0
-3 -5 6 8 0
-4 -6 0
2 3 0
3 -6 0
-3 -4 -10 0
-5 0
2 5 0
-2 -3 5 0
-1 10 0
-4 7 0
-2 -4 -8 -9 0
1 -8 0

With "less logfile" one can see the solution-process.


Passing the option "nopre" to minisat can for large sparse instances be helpful:
Bicliques> BRG "20*15,3" | ./CNFBCC nopre "" "" "" "" ""

p cnf 10 20
1 0
-1 -10 0
2 7 10 0
-2 5 7 0
-1 3 4 0
4 -7 8 0
-4 5 8 0
5 6 7 0
-5 6 8 0
-4 -6 0
2 3 -6 9 0
3 -6 7 9 0
-3 0
-5 0
2 5 9 0
5 -9 0
-1 3 0
-4 0
-2 -4 -8 0
1 -8 0


With the last argument one can stipulate an upper-bound (search-movement
is downwards), either absolute or via "+" related to the lower-bound
by symmetry-breaking:

So
Bicliques> BRG "20*15,3" | ./CNFBCC nopre "" "" "" "" 10
yield also the above result.

For using "+k", we need to know how far off is symmetry-breaking:
Bicliques> BRG "20*15,3" | ./CNF2cg | ./BCC2SAT "" "" -cs "" "" | grep "planted"
c planted-edges                         8

Thus
Bicliques> BRG "20*15,3" | ./CNFBCC nopre "" "" "" "" +2
yield also the above result


See plans/general.txt.

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
        "0.6.0",
        "9.4.2023",
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
         << " algo-options sb-rounds timeout seeds log B\n\n"
    " algo-options   : " << Environment::WRP<SB>{} << "\n"
    "                : " << Environment::WRP<PT>{} << "\n"
    "                : " << Environment::WRP<DI>{} << "\n"
    "                : " << Environment::WRP<SO>{} << "\n"
    " sb-rounds      : " << "default is " << default_sb_rounds << "\n"
    " timeout        : " << "in s, default is " << default_sec << "\n"
    " seeds          : " << "sequence, can contain \"t\" or \"r\"" << "\n"
    " log            : " << "filename for solving-log, default is null\n"
    " B              : " << "[+]upper-bound, default is automatic upper bound\n\n"
    " reads a cnf from standard input, and attempts to compute an optimal representation:\n\n"
    "  - Arguments \"\" (the empty string) yield the default-values.\n"
    "  - Default-values for the options are the first possibilities given.\n"
    "  - By using \"/dev/stdout\" for log the log goes to standard output.\n"
    "  - Using \"+\" for B means the increment added to the lower-bound.\n"
    "  - The representation is exact (for the conflict-*graph*) iff considering bcp.\n\n"
;
    return true;
  }

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  if (argc != 7) {
    std::cerr << error <<
      "Exactly six arguments (algo-opt, sb-rounds, timeout, seeds, log, B)"
      " needed, but " << argc-1 << " provided.\n";
    return int(Error::missing_parameters);
  }

  const alg2_options_t algopt =
    Environment::translate<alg2_options_t>()(argv[1], sep);
  const var_t sb_rounds = read_var_t(argv[2], default_sb_rounds);
  const auto sec = read_uint_t(argv[3], default_sec);
  const RandGen::vec_eseed_t seeds = RandGen::extract_seeds(argv[4]);
  const auto log = read_log(argv[5], error);
  const std::string bounds_str = argv[6];

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
  const auto G = ConflictGraphs::conflictgraph(F);
  assert(G.n() == F.first.c);
  const auto bounds0 = read_bounds(bounds_str, F.first.n, G.n(), G.m());
  if (not bounds0) {
    std::cerr << error <<
      "Bounds-argument faulty.\n";
    return int(Error::faulty_parameters);
  }
  BC2SAT T(G, bounds0.value());
  const auto res = T.sat_solve(log.pointer(), algopt, sb_rounds, sec, seeds);
  assert(res.rt != ResultType::upper_unsat_sb and
         res.rt != ResultType::upper_unsat and
         res.rt != ResultType::unknown);
  log.close();
  std::cout << Bicliques::bcc2CNF(res.bcc, F.first.c);

}

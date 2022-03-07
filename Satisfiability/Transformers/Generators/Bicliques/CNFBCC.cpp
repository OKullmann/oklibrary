// Oliver Kullmann, 7.3.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  For input-CNF F, compute another CNF F' which has the same conflict-graph
  and the minimal number of variables.


EXAMPLES:

Bicliques> echo -e "p cnf 100 5\n0\n1 2 0\n3 4 0\n -3 5 0\n-5 0\n" | ./CNFBCC ""
p cnf 1 5
0
0
1 0
-1 0
1 0

Since there is randomisation involved, in general the output is also
randomised (but if the biclique-covering-number was succesfully computed,
then the number of variables is uniquely determined (the number of
clauses is always equal to the original number)).

*/


#include <iostream>
#include <algorithm>

#include <cassert>

#include <ProgramOptions/Environment.hpp>

#include "Bicliques.hpp"
#include "DimacsTools.hpp"
#include "ConflictGraphs.hpp"
#include "Bicliques2SAT.hpp"

#include "BCC2SAT.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.2",
        "7.3.2022",
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
    "> " << proginfo.prg << " algo-options"
      " [sb-rounds=" << default_sb_rounds << "]"
      " [timeout(sec)=" << default_sec << "]\n\n"
    " algo-options   : " << Environment::WRP<SB>{} << "\n\n"
    " reads a CNF from standard input, and attempts to compute its bcc-number,\n"
    " realised by a CNF printed to standard output:\n\n"
    "  - Arguments \"\" (the empty string) yield also the default-values.\n"
    "  - Default-values for the options are the first possibilities given.\n\n"
;
    return true;
  }

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  if (argc < 2) {
    std::cerr << error << "At least algo-opt"
      " needed.\n";
    return int(Error::missing_parameters);
  }

  const alg_options_t algopt =
    Environment::translate<alg_options_t>()(argv[1], sep);
  const var_t sb_rounds = argc >= 3 ?
    read_var_t(argv[2], default_sb_rounds) : default_sb_rounds;
  const auto sec = argc >= 4 ?
    read_uint_t(argv[3], default_sec) : default_sec;

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
  BC2SAT trans(G, std::min(F.first.n, G.m()));
  const auto res = trans(nullptr, algopt, sb_rounds, sec);
  assert(res.rt != ResultType::init_unsat_sb and
         res.rt != ResultType::init_unsat and
         res.rt != ResultType::unknown);
  std::cout << Bicliques::bcc2CNF(res.bcc, F.first.c);

}

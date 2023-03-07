// Oliver Kullmann, 25.2.2022 (Swansea)
/* Copyright 2022, 2023 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Translating biclique-covering problems into SAT-problems
  (see BCCbySAT for the complete packaging of repeated SAT-solving
   for computing bcc(G) of the input-graph G)


EXAMPLES:

Just obtaining statistics (by "-cs"):

Obtaining B from symmetry-breaking:
Bicliques> time ./GraphGen grid 20 20 | ./BCC2SAT "" "" "-cs" "" ""
c ** Parameters **
c V                                     400
c E                                     760
c B                                     159
c sb-option                             basic-sb
c ** Statistics **
c  bc-variables                         127200
c  edge-variables                       120840
c total-variables                       248040
c  bc-clauses                           25198320
c   bc-lit-occurrences                  50396640
c  edge-clauses                         725040
c   edge-lit-occurrences                2175120
c  cover-clauses                        760
c   cover-lit-occurrences               120840
c  unit-clauses                         477
c total-clauses                         25924597
c total-lit-occurrences                 52693077
c ** Formatting **
c comments-option                       with-comments
c dimacs-parameter-option               with-parameters
c clauses-option                        without-cs
c ** Symmetry Breaking **
c planted-edges                         159
c sb-stats                              100 : 142 149.2 159; 3.34241
c num_e-seeds                           0
c  e-seeds                              
p cnf 248040 25924597
real	0m0.079s
user	0m0.081s
sys	0m0.000s

Specifying B=200:, and 20000 symmetry-breaking-rounds:
Bicliques> time ./GraphGen grid 20 20 | ./BCC2SAT 200 "" "-cs" 20000 ""
c ** Parameters **
c V                                     400
c E                                     760
c B                                     200
c sb-option                             basic-sb
c ** Statistics **
c  bc-variables                         160000
c  edge-variables                       152000
c total-variables                       312000
c  bc-clauses                           31696000
c   bc-lit-occurrences                  63392000
c  edge-clauses                         912000
c   edge-lit-occurrences                2736000
c  cover-clauses                        760
c   cover-lit-occurrences               152000
c  unit-clauses                         495
c total-clauses                         32609255
c total-lit-occurrences                 66280495
c ** Formatting **
c comments-option                       with-comments
c dimacs-parameter-option               with-parameters
c clauses-option                        without-cs
c ** Symmetry Breaking **
c planted-edges                         165
c sb-stats                              20000 : 135 149.572 165; 3.30518
c num_e-seeds                           0
c  e-seeds                              
p cnf 312000 32609255
real	0m12.931s
user	0m12.918s
sys	0m0.009s

One sees that symmetry-breaking with 20000 attempts obtained a maximum of
165 planted edges.


TODOS:

1. DONE Read seeds

2. Provide more statistics
  - DONE See "XXX clause- and variables- numbers" in Bicliques2SAT.hpp.

*/


#include <iostream>

#include <ProgramOptions/Environment.hpp>
#include <Transformers/Generators/Random/Numbers.hpp>

#include "Graphs.hpp"
#include "Bicliques2SAT.hpp"

#include "BCC2SAT.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.7.0",
        "6.3.2023",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Bicliques/BCC2SAT.cpp",
        "GPL v3"};

  using namespace Bicliques2SAT;
  using namespace BCC2SAT;

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg
         << " B algo-options format-options sb-rounds seeds\n\n"
    " B              : " << "biclique-cover-size, default is "
         << default_B << "\n"
    " algo-options   : " << Environment::WRP<SB>{} << "\n"
    " format-options : " << Environment::WRP<DC>{} << "\n"
    "                : " << Environment::WRP<DP>{} << "\n"
    "                : " << Environment::WRP<CS>{} << "\n"
    " sb-rounds      : " << "default is " << default_sb_rounds << "\n"
    " seeds          : " << "sequence, can contain \"t\" or \"r\"" << "\n\n"
    " reads a graph from standard input, and prints the SAT-translation to standard output:\n\n"
    "  - Arguments \"\" (the empty string) yield the default-values.\n"
    "  - B=0 means the value is chosen by symmetry-breaking if enabled.\n"
    "  - Default-values for the options are the first possibilities given.\n\n"
;
    return true;
  }

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  if (argc != 6) {
    std::cerr << error <<
      "Exactly five arguments (B, algo-opt, form-opt, rounds, seeds)"
      " needed, but " << argc-1 << " provided.\n";
    return int(Error::missing_parameters);
  }

  const var_t B = read_var_t(argv[1], default_B);
  const alg_options_t algopt =
    Environment::translate<alg_options_t>()(argv[2], sep);
  const format_options_t formopt =
    Environment::translate<format_options_t>()(argv[3], sep);
  const var_t sb_rounds =
    read_var_t(argv[4], default_sb_rounds);
  const RandGen::vec_eseed_t seeds = RandGen::extract_seeds(argv[5]);

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

  const auto G = Graphs::make_AdjVecUInt(std::cin, Graphs::GT::und);
  BC2SAT trans(G, B);
  try { trans(std::cout, algopt, formopt, sb_rounds, seeds); }
  catch (const BC2SAT::Unsatisfiable& e) {
    std::cerr << "UNSAT\nB >= " << e.incomp.size() << "\n";
    return int(Error::found_unsat);
  }

}

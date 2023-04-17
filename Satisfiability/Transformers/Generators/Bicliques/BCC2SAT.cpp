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
Bicliques> time ./GraphGen grid 20 20 | ./BCC2SAT "" "" -cs "" ""
c "./BCC2SAT" "" "" "-cs" "" ""
c ** Parameters **
c B                                     +0
c sb-option                             basic-sb
c pt-option                             cover
c comments-option                       with-comments
c dimacs-parameter-option               with-parameters
c clauses-option                        without-cs
c sb-rounds                             100
c num_e-seeds                           0
c ** Symmetry Breaking **
c planted-edges                         156
c sb-stats                              100 : 143 149.17 156; 2.80712
c sb-seed                               35
c ** Statistics **
c V                                     400
c E                                     760
c B                                     156
c  bc-variables                         124800
c  edge-variables                       118560
c total-variables                       243360
c  bc-clauses                           24722880
c   bc-lit-occurrences                  49445760
c  edge-clauses                         711360
c   edge-lit-occurrences                2134080
c  cover-clauses                        760
c   cover-lit-occurrences               118560
c  unit-clauses                         468
c total-clauses                         25435468
c total-lit-occurrences                 51698868
p cnf 243360 25435468
real	0m0.082s
user	0m0.083s
sys	0m0.000s

Via appending the sb-seed to the given seed-sequence (above it is empty)
one can get the best sb-result in a single round:
Bicliques> ./GraphGen grid 20 20 | ./BCC2SAT "" "" -cs 1 35
...
c sb-rounds                             1
c num_e-seeds                           1
c  e-seeds                              35
c ** Symmetry Breaking **
c planted-edges                         156
c sb-stats                              1 : 156 156 156; 0
c sb-seed                               0


Explicitly specifying B=200, and 20000 symmetry-breaking-rounds:
Bicliques> time ./GraphGen grid 20 20 | ./BCC2SAT 200 "" -cs 20000 ""
c "./BCC2SAT" "200" "" "-cs" "20000" ""
c ** Parameters **
c B                                     200
c sb-option                             basic-sb
c pt-option                             cover
c comments-option                       with-comments
c dimacs-parameter-option               with-parameters
c clauses-option                        without-cs
c sb-rounds                             20000
c num_e-seeds                           0
c ** Symmetry Breaking **
c planted-edges                         165
c sb-stats                              20000 : 136 149.59 165; 3.2904
c sb-seed                               12440
c ** Statistics **
c V                                     400
c E                                     760
c B                                     200
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
p cnf 312000 32609255
real	0m13.533s
user	0m13.534s
sys	0m0.001s

One sees that symmetry-breaking with 20000 attempts obtained a maximum of
165 planted edges.

Since for the above 200=165+35, one obtains the same result by
Bicliques> ./GraphGen grid 20 20 | ./BCC2SAT +35 "" -cs 20000 ""
or (via a single round) b
Bicliques> ./GraphGen grid 20 20 | ./BCC2SAT +35 "" -cs 1 12440

If one uses an explicitly specified B which is lower than what symmetry-
breaking obtained, the value will be updated to that value:
Bicliques> ./GraphGen grid 20 20 | ./BCC2SAT 165 "" -cs 1 12440
c total-lit-occurrences                 54681495
p cnf 257400 26902855
Bicliques> ./GraphGen grid 20 20 | ./BCC2SAT 164 "" -cs 1 12440
c total-lit-occurrences                 54681495
p cnf 257400 26902855


One can also consider partition-problems (instead of cover-problems, the
default).
The above for partitioning (using the quadratically many prime-clauses):
Bicliques> time ./GraphGen grid 20 20 | ./BCC2SAT 200 partition2 -cs 20000 ""
c "./BCC2SAT" "200" "partition2" "-cs" "20000" ""
c ** Parameters **
c B                                     200
c sb-option                             basic-sb
c pt-option                             partition-quadratic
c comments-option                       with-comments
c dimacs-parameter-option               with-parameters
c clauses-option                        without-cs
c sb-rounds                             20000
c num_e-seeds                           0
c ** Symmetry Breaking **
c planted-edges                         165
c sb-stats                              20000 : 136 149.59 165; 3.2904
c sb-seed                               12440
c ** Statistics **
c V                                     400
c E                                     760
c B                                     200
c  bc-variables                         160000
c  edge-variables                       152000
c total-variables                       312000
c  bc-clauses                           31696000
c   bc-lit-occurrences                  63392000
c  edge-clauses                         912000
c   edge-lit-occurrences                2736000
c  cover-clauses                        760
c   cover-lit-occurrences               152000
c  partition-clauses                    15124000
c   partition-lit-occurrences           30248000
c  unit-clauses                         495
c total-clauses                         47733255
c total-lit-occurrences                 96528495
p cnf 312000 47733255
real	0m13.473s
user	0m13.457s
sys	0m0.013s


For example covering the K_4 can be done with 2 bicliques,
but partition requires 3 bicliques:
Bicliques> ./GraphGen clique 4 | ./BCC2SAT 2 "" "" "" "" | minisat /dev/stdin
SATISFIABLE
Bicliques> ./GraphGen clique 4 | ./BCC2SAT 2 partition2 "" "" "" | minisat /dev/stdin
UNSATISFIABLE


See plans/general.txt.

*/


#include <iostream>

#include <ProgramOptions/Environment.hpp>
#include <Transformers/Generators/Random/Numbers.hpp>

#include "Graphs.hpp"
#include "Bicliques2SAT.hpp"

#include "BCC2SAT.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "1.0.2",
        "17.4.2023",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Bicliques/BCC2SAT.cpp",
        "GPL v3"};

  using namespace Bicliques2SAT;
  using namespace BCC2SAT;

  const std::string error = "ERROR[" + proginfo.prg + "]: ";
  const std::string comment = "c ";

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg
         << " B algo-options format-options sb-rounds seeds\n\n"
    " B              : " << "[+]biclique-cover-size, default is \"+0\"\n"
    " algo-options   : " << Environment::WRP<SB>{} << "\n"
    "                : " << Environment::WRP<PT>{} << "\n"
    " format-options : " << Environment::WRP<DC>{} << "\n"
    "                : " << Environment::WRP<DP>{} << "\n"
    "                : " << Environment::WRP<CS>{} << "\n"
    " sb-rounds      : " << "default is " << default_sb_rounds << "\n"
    " seeds          : " << "sequence, can contain \"t\" or \"r\"" << "\n\n"
    " reads a graph from standard input, and prints the SAT-translation to standard output:\n\n"
    "  - Arguments \"\" (the empty string) yield the default-values.\n"
    "  - \"+\" for B means the increment from the symmetry-breaking result.\n"
    "  - If rounds=1, then seeds are taken as given, otherwise a round-index is appended.\n"
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

  const Bounds B = read_current(argv[1]);
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

  commandline_output(formopt, comment, std::cout, argc, argv);
  const auto G = Graphs::make_AdjVecUInt(std::cin, Graphs::GT::und);
  BC2SAT T(G, Bounds(B));
  try { T.sat_translate(std::cout, algopt, formopt, sb_rounds, seeds); }
  catch (const BC2SAT::Unsatisfiable& e) {
    std::cerr << "UNSAT\nB >= " << e.incomp.size() << "\n";
    return int(Error::found_unsat);
  }

}

// Oliver Kullmann, 25.2.2022 (Swansea)
/* Copyright 2022, 2023, 2025 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Translating biclique-covering problems into SAT-problems
  (see BCCbySAT for the complete packaging of repeated SAT-solving
   for computing bcc(G) of the input-graph G)

TODOS:

0. Using FastVC.
   - The class Bicliques2SAT::Bounds needs an overhaul.
    - Perhaps having fixed members for the bccomp- and the vc-number.
    - In this program, we should get explicit information on the "graph
      analysis", stating n, m, and the bccomp- and the vc-number (and
      how computed).
    - Symmetry-breaking is then a separate issue.
    - Perhaps for this program, we abondon the arcance class Bounds, and
      start with a new approach.
   - We need a further argument "timeout2" for the fastvc-timeout.
   - Seeds are ignored for now.
    - Though we need also the application of rounds to redumis and fastvc.
    - Then need two rounds-parameters.
    - While we could use the generic seeds-input as master-seed for
      everything.

2. Provided variations specifically for the cover-representation:
  - Bicliques2SAT::PT should get new members, after cover=0, and
    before the partition-members.
  - coverMaxR introduces for every variable/vertex v on the right-hand side of
    a biclique the implications
      (all non-neighbours of v set to false on the left-side) -> v.
    This is a positive clause of length degree(v)+1.
  - The reverse direction is already present.
  - This makes the right-hand-side of the bicliques maximal (which is thus
    determined by the left-hand-side).
  - Similarly coverMaxL makes the left-hand-side maximal.
  - And coverMaxLR is the union of both, and thus only maximal bicliques
    will be used for covering.
  - One needs also an option to turn off the trimming of edges (since
    using only maximal bicliques might be interesting).
  - Another option coverL has only (so to speak) the variables on the
    left-hand-side of the bicliques, plus the edge-variables, and the
    variables on the right-hand-side are determined by the lhs.
  - Symmetry-breaking should be independent of all of this.

3. Provide MaxSAT-translations
  - This should work for all possibilities in Bicliques2SAT::PT.
  - Adding biclique-indicator-variables for every biclique, being
    true if any vertex is set (or only if no variable of say the
    left side is set)>
  - Maximise then the negative unit-clauses of these variables.

4. Update examples below.

See plans/general.txt.


EXAMPLES:

Just obtaining statistics (by "-trans"):

Obtaining B from symmetry-breaking:
Bicliques> time ./GraphGen grid 20 20 | ./BCC2SAT "" "" -trans "" ""
c "./BCC2SAT" "" "" "-trans" "" ""
c ** Parameters **
c B                                     +0
c sb-options                            basic-sb with-ssb
c pt-option                             cover
c comments-option                       with-comments
c dimacs-parameter-option               with-parameters
c clauses-option                        without-translation
c sb-rounds                             100
c num_e-seeds                           0
c ** Symmetry Breaking **
c planted-edges                         156
c sb-stats                              100 : 143 149.17 156; 2.80712
c sb-seed                               35
c restricted-edges                      0
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
c  positive unit-clauses                468
c  negative unit-clauses                0
c  negative binary-clauses              0
c total-clauses                         25435468
c total-lit-occurrences                 51698868
p cnf 243360 25435468
real	0m0.082s
user	0m0.080s
sys	0m0.004s

The given B will be adjusted to a simple upper bound, if beyond that:
Bicliques> time ./GraphGen grid 20 20 | ./BCC2SAT 1000 "" -trans "" ""
c "./BCC2SAT" "1000" "" "-trans" "" ""
c ** Parameters **
c B                                     399
...
c restricted-edges                      243
...
c  positive unit-clauses                468
c  negative unit-clauses                29403
c  negative binary-clauses              243
c total-clauses                         65083834
c total-lit-occurrences                 132258957
p cnf 622440 65083834


Via appending the sb-seed to the given seed-sequence (above it is empty)
one can get the best sb-result in a single round:
Bicliques> ./GraphGen grid 20 20 | ./BCC2SAT "" "" -trans 1 35
...
c sb-rounds                             1
c num_e-seeds                           1
c  e-seeds                              35
c ** Symmetry Breaking **
c planted-edges                         156
c sb-stats                              1 : 156 156 156; 0
c sb-seed                               0
c restricted-edges                      0
...
p cnf 243360 25435468

Symmetry-breaking via maximal independent sets in the biclique-compatbility
graph is much more efficient:
Bicliques> ./GraphGen grid 20 20 | ./BCC2SAT "" +sbi -trans "" ""
...
c ** Symmetry Breaking **
c planted-edges                         199
c sb-stats                              100 : 190 195.87 199; 2.41901
c sb-seed                               11
c restricted-edges                      0
...
p cnf 310440 32446317

Explicitly specifying B=200, and 20000 symmetry-breaking-rounds:
Bicliques> time ./GraphGen grid 20 20 | ./BCC2SAT 200 "" -trans 20000 ""
c "./BCC2SAT" "200" "" "-trans" "20000" ""
c ** Parameters **
c B                                     200
c sb-options                            basic-sb with-ssb
c pt-option                             cover
c comments-option                       with-comments
c dimacs-parameter-option               with-parameters
c clauses-option                        without-translation
c sb-rounds                             20000
c num_e-seeds                           0
c ** Symmetry Breaking **
c planted-edges                         165
c sb-stats                              20000 : 136 149.59 165; 3.2904
c sb-seed                               12440
c restricted-edges                      35
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
c  positive unit-clauses                495
c  negative unit-clauses                595
c  negative binary-clauses              35
c total-clauses                         32609885
c total-lit-occurrences                 66281160
p cnf 312000 32609885
real	0m13.265s
user	0m13.262s
sys	0m0.004s

One sees that symmetry-breaking with 20000 attempts obtained a maximum of
165 planted edges.

Since for the above 200=165+35, one obtains the same result by
Bicliques> ./GraphGen grid 20 20 | ./BCC2SAT +35 "" -trans 20000 ""
c "./BCC2SAT" "+35" "" "-trans" "20000" ""
c ** Parameters **
c B                                     +35
...
c ** Statistics **
c V                                     400
c E                                     760
c B                                     200

or (via a single round) b
Bicliques> ./GraphGen grid 20 20 | ./BCC2SAT +35 "" -trans 1 12440

If one uses an explicitly specified B which is lower than what symmetry-
breaking obtained, the value will be updated to that value:
Bicliques> ./GraphGen grid 20 20 | ./BCC2SAT 165 "" -trans 1 12440
c total-lit-occurrences                 54681495
p cnf 257400 26902855
Bicliques> ./GraphGen grid 20 20 | ./BCC2SAT 164 "" -trans 1 12440
c B                                     165
c total-lit-occurrences                 54681495
p cnf 257400 26902855

Using a heuristics to find better symmetry-breaking:
Bicliques> time ./GraphGen grid 20 20 | ./BCC2SAT 200 +sba -trans 1000 ""
c "./BCC2SAT" "200" "+sba" "-trans" "1000" ""
c ** Parameters **
c B                                     200
c sb-options                            sorted-sb-addition with-ssb
c pt-option                             cover
c comments-option                       with-comments
c dimacs-parameter-option               with-parameters
c clauses-option                        without-translation
c sb-rounds                             1000
c num_e-seeds                           0
c ** Symmetry Breaking **
c planted-edges                         167
c sb-stats                              1000 : 146 154.908 167; 3.12783
c sb-seed                               380
c restricted-edges                      33
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
c  positive unit-clauses                501
c  negative unit-clauses                528
c  negative binary-clauses              33
c total-clauses                         32609822
c total-lit-occurrences                 66281095
p cnf 312000 32609822
real	0m0.684s
user	0m0.685s
sys	0m0.001s
Bicliques> time ./GraphGen grid 20 20 | ./BCC2SAT 200 +sba -trans 20000 ""
c planted-edges                         169
c sb-stats                              20000 : 142 154.994 169; 3.09836
c sb-seed                               18802
c restricted-edges                      31

So we get somewhat better (the exact value of bcc here is 199).
> Bicliques> time ./GraphGen grid 20 20 | ./BCC2SAT 200 +sba -trans 2000000 0
c "./BCC2SAT" "200" "+sba" "-trans" "2000000" "0"
c ** Parameters **
c B                                     200
c sb-options                            sorted-sb-addition with-ssb
c pt-option                             cover
c comments-option                       with-comments
c dimacs-parameter-option               with-parameters
c clauses-option                        without-translation
c sb-rounds                             2000000
c num_e-seeds                           1
c  e-seeds                              0
c ** Symmetry Breaking **
c planted-edges                         169
c sb-stats                              2000000 : 139 154.985 169; 3.10572
c sb-seed                               57579
c restricted-edges                      31
real    29m11.791s
user    29m11.683s
sys     0m0.005s
Bicliques> time ./GraphGen grid 20 20 | ./BCC2SAT 200 +sba -trans 20000000 1
c ** Symmetry Breaking **
c planted-edges                         174
c sb-stats                              20000000 : 138 154.988 174; 3.1085
c sb-seed                               7347840
c restricted-edges                      26

Bicliques> time ./GraphGen grid 20 20 | ./BCC2SAT 200 +sb -trans 2000000 0
c ** Symmetry Breaking **
c planted-edges                         165
c sb-stats                              2000000 : 134 149.582 165; 3.298
c sb-seed                               99863
c restricted-edges                      35
real    26m25.051s
user    26m24.931s
sys     0m0.015s

Solving the SAT-case via minisat:
Bicliques> ./GraphGen grid 20 20 | ./BCC2SAT 199 +sba "" 1 1,7347840 | minisat-2.2.0 -no-pre
|       100 |   40948  4080941  8394595 |  1496345      100     37 | 86.810 % |
|  11362814 |   40948  4080941  8394595 | 17833704 11362814    317 | 86.810 % |
===============================================================================
INDETERMINATE

OKsolver:
Bicliques> ./GraphGen grid 20 20 | ./BCC2SAT 199 +sba "" 1 1,7347840 > Grid2020.cnf
545539654 May  8 13:55 Grid2020.cnf
Bicliques> OKsolver2002 Grid2020.cnf &
looked hopeless after 5h.

However with +sbi it's an easy problem (since achieving optimal sb):
Bicliques> time ./GraphGen grid 20 20 | ./BCC2SAT "" +sbi +cs "" "" | minisat-2.2.0 -no-pre
|  Number of variables:        310440                                         |
|  Number of clauses:        32445720                                         |
|  Parse time:                   2.20 s                                       |
|  Simplification time:          0.42 s                                       |
restarts              : 1
conflicts             : 4              (2 /sec)
decisions             : 16             (0.00 % random) (6 /sec)
propagations          : 312360         (119086 /sec)
conflict literals     : 55             (0.00 % deleted)
Memory used           : 1260.00 MB
CPU time              : 2.62298 s
SATISFIABLE
real	0m8.281s
user	0m8.462s
sys	0m0.543s
OKsolver:
Bicliques> ./GraphGen grid 20 20 | ./BCC2SAT 199 +sbi "" "" "" > Grid2020.cnf
545536819 Grid2020.cnf
Bicliques> OKsolver2002 Grid2020.cnf &
Bicliques> s SATISFIABLE
c sat_status                            1
c initial_maximal_clause_length         199
c initial_number_of_variables           310440
c initial_number_of_clauses             32446317
c initial_number_of_literal_occurrences 65949197
c number_of_initial_unit-eliminations   308098
c reddiff_maximal_clause_length         195
c reddiff_number_of_variables           308098
c reddiff_number_of_clauses             32440147
c reddiff_number_of_literal_occurrences 65936279
c number_of_2-clauses_after_reduction   5690
c running_time(sec)                     5.7
c number_of_nodes                       1
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                90
c number_of_pure_literals               0
c number_of_autarkies                   1
c number_of_missed_single_nodes         0
c max_tree_depth                        0
...


One can also consider partition-problems (instead of cover-problems, the
default).
The above for partitioning (using the quadratically many prime-clauses):
Bicliques> time ./GraphGen grid 20 20 | ./BCC2SAT 200 partition2 -trans 20000 ""
c "./BCC2SAT" "200" "partition2" "-trans" "20000" ""
c ** Parameters **
c B                                     200
c sb-options                            basic-sb with-ssb
c pt-option                             partition-quadratic
c comments-option                       with-comments
c dimacs-parameter-option               with-parameters
c clauses-option                        without-translation
c sb-rounds                             20000
c num_e-seeds                           0
c ** Symmetry Breaking **
c planted-edges                         165
c sb-stats                              20000 : 136 149.59 165; 3.2904
c sb-seed                               12440
c restricted-edges                      35
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
c  positive unit-clauses                495
c  negative unit-clauses                595
c  negative binary-clauses              35
c total-clauses                         47733885
c total-lit-occurrences                 96529160
p cnf 312000 47733885
real	0m11.801s
user	0m11.801s
sys	0m0.000s


For example covering the K_4 can be done with 2 bicliques,
but partition requires 3 bicliques:
Bicliques> ./GraphGen clique 4 | ./BCC2SAT 2 "" "" "" "" | minisat /dev/stdin
SATISFIABLE
Bicliques> ./GraphGen clique 4 | ./BCC2SAT 2 partition2 "" "" "" | minisat /dev/stdin
UNSATISFIABLE

*/


#include <iostream>

#include <ProgramOptions/Environment.hpp>
#include <Transformers/Generators/Random/Numbers.hpp>

#include "Graphs.hpp"
#include "Bicliques2SAT.hpp"

#include "BCC2SAT.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "1.5.3",
        "24.4.2025",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Bicliques/BCC2SAT.cpp",
        "GPL v3"};

  using namespace Bicliques2SAT;
  using namespace BCC2SAT;

  const std::string error = "ERROR[" + proginfo.prg + "]: ";
  const std::string comment = "c ";

  const std::string default_B = "+0";

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg
         << " B algo-options format-options sb-rounds seeds [timeout]\n\n"
    " B              : " << "[+-]biclique-cover-size, default is \"" <<
      default_B << "\"\n"
    " algo-options   : " << Environment::WRP<SB>{} << "\n"
    "                : " << Environment::WRP<SS>{} << "\n"
    "                : " << Environment::WRP<PT>{} << "\n"
    " format-options : " << Environment::WRP<DC>{} << "\n"
    "                : " << Environment::WRP<DP>{} << "\n"
    "                : " << Environment::WRP<CS>{} << "\n"
    " sb-rounds      : " << "default is " << default_sb_rounds << "\n"
    " seeds          : " << "sequence, can contain \"t\" or \"r\"\n"
    " [timeout]      : " << "double, for redumis only; default=" <<
      GraphTools::default_redumis_timeout << "\n\n"
    " reads a graph from standard input, and prints the SAT-translation"
    " to standard output:\n\n"
    "  - Arguments \"\" (the empty string) yield the default-values.\n"
    "  - \"+\" for B means the increment from the lower bound,\n"
    "    while \"-\" means the decrement from the upper bound.\n"
    "  - If sb-rounds=1, then seeds are taken as given, otherwise a round-index"
    " is appended.\n"
    "  - sb-rounds does not currently apply to redumis-symmetry-breaking.\n"
    "  - Default-values for the options are the first possibilities given.\n\n"
;
    return true;
  }

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  if (argc != 6 and argc != 7) {
    std::cerr << error <<
      "Exactly five or six arguments (B, algo-opt, form-opt, rounds, seeds [,timeout])"
      " needed, but " << argc-1 << " provided.\n";
    return int(Error::faulty_parameters);
  }

  const std::string B_str(std::string(argv[1]).empty() ?
                          default_B : argv[1]);
  const alg3_options_t algopt =
    Environment::translate<alg3_options_t>()(argv[2], sep);
  const auto [sbo, sso, ubo, pto] = algopt;
  const format_options_t formopt =
    Environment::translate<format_options_t>()(argv[3], sep);
  const auto [dco, dpo, cso] = formopt;
  const var_t sb_rounds =
    read_var_t(argv[4], default_sb_rounds);
  const RandGen::vec_eseed_t seeds = RandGen::extract_seeds(argv[5]);
  const std::string timeout_str = argc==7 ? std::string(argv[6])
    : std::string();
  const double redumis_timeout = timeout_str.empty() ?
    GraphTools::default_redumis_timeout : std::stold(timeout_str);

  if (sbo != SB::none and sbo != SB::redumis and sb_rounds == 0) {
    std::cerr << error <<
      "Symmetry-breaking on, but number of rounds is zero.\n";
    return int(Error::bad_sb);
  }
  commandline_output(formopt, comment, std::cout, argc, argv);

  const auto G = Graphs::make_AdjVecUInt(std::cin, Graphs::GT::und);
  const var_t upper_bound = determine_ub(G, ubo, error);
  if (dco != DC::without)
    std::cout << Environment::DWW{"upper-bound"} << upper_bound << "\n";
  const Bounds B = read_B(B_str, upper_bound);

  BC2SAT T(G, B);
  T.sat_translate(std::cout, extract_alg_options(algopt), formopt,
                  sb_rounds, seeds, redumis_timeout);

}

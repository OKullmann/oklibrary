// Oliver Kullmann, 6.3.2022 (Swansea)
/* Copyright 2022, 2023 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Solving biclique-covering problems by SAT-solving, computing bcc(G)
  for an input-graph G

  (BCC2SAT computes one single SAT-translation, while this program
   attempts to run a complete chain of SAT-solving, starting with a given
   upper bound B on the number of bicliqes, and finishing with outputting
   an optimal solution.)


EXAMPLES:

Bicliques> ./GraphGen clique 16 | ./BCCbySAT 5 "" "" "" "" "" ST ""
# "./BCCbySAT" "5" "" "" "" "" "" "" ""
# ** Parameters **
# B                                     downwards 0 5
# sb-options                            basic-sb with-ssb
# pt-option                             cover
# di-option                             downwards
# so-option                             default-solver
# comments-option                       with-comments
# bicliques-option                      with-bicliques
# solver-timeout(s)                     2147483647
# sb-rounds                             100
# num_e-seeds                           0
# statistics-output                     ST
# log-output                            null
# ** Results **
# sb-stats                              100 : 1 1 1; 0
# result-type                           exact
# bcc                                   = 4
1 3 4 7 8 9 13 15 | 2 5 6 10 11 12 14 16
1 3 6 8 10 11 13 14 | 2 4 5 7 9 12 15 16
1 2 7 8 10 11 15 16 | 3 4 5 6 9 12 13 14
2 3 5 6 7 8 9 11 | 1 4 10 12 13 14 15 16
Bicliques> cat ST
B sat maxn    c ptime stime elimc rts  cfs  cfsps  dec decpr  decps     r1        r1ps   cfl cflpd mem        t
4   1  608 3067     0     0  0.01   6  608  47463 1368     0 106792  32376  2.5274e+06 13102  7.53  25  0.01281
3   0  456 2330     0     0  0.01  22 4076 245011 5514     0 331450 121658 7.31294e+06 31223 19.92  25 0.016636

Without specifying an upper bound, and moving upwards:
Bicliques> ./GraphGen clique 16 | ./BCCbySAT "" up "" "" "" "" ST ""
# "./BCCbySAT" "" "up" "" "" "" "" "ST" ""
# ** Parameters **
# B                                     upwards +0 18446744073709551615
# sb-options                            basic-sb with-ssb
# pt-option                             cover
# di-option                             upwards
# so-option                             default-solver
# comments-option                       with-comments
# bicliques-option                      with-bicliques
# solver-timeout(s)                     2147483647
# sb-rounds                             100
# num_e-seeds                           0
# statistics-output                     ST
# log-output                            null
# ** Results **
# sb-stats                              100 : 1 1 1; 0
# result-type                           exact
# bcc                                   = 4
1 3 4 7 8 9 13 15 | 2 5 6 10 11 12 14 16
1 3 6 8 10 11 13 14 | 2 4 5 7 9 12 15 16
1 2 7 8 10 11 15 16 | 3 4 5 6 9 12 13 14
2 3 5 6 7 8 9 11 | 1 4 10 12 13 14 15 16

We note that the bounds shown are before adjustment.

Bicliques> cat ST
B sat maxn    c ptime stime elimc rts  cfs  cfsps  dec decpr  decps     r1        r1ps   cfl cflpd mem        t
1   0  152  736     0     0     0   0    0      0    0   nan      0    138       77138     0   nan  25 0.001789
2   0  304 1593     0     0  0.01   1   25  12893   55     0  28365    402      207323    64  9.86  25 0.001939
3   0  456 2330     0     0  0.01  22 4076 202554 5514     0 274015 121658 6.04572e+06 31223 19.92  25 0.020123
4   1  608 3067     0     0  0.01   6  608  94498 1368     0 212620  32376 5.03202e+06 13102  7.53  25 0.006434

Using "-ssb" we get
Bicliques> ./GraphGen clique 16 | ./BCCbySAT "" up,-ssb "" "" "" "" ST ""
1 2 8 10 11 13 14 15 | 3 4 5 6 7 9 12 16
1 2 3 4 6 7 8 10 | 5 9 11 12 13 14 15 16
2 4 7 9 10 11 14 16 | 1 3 5 6 8 12 13 15
1 4 6 10 12 13 14 16 | 2 3 5 7 8 9 11 15
Bicliques> cat ST
B sat maxn    c ptime stime elimc rts  cfs  cfsps  dec decpr  decps    r1        r1ps   cfl cflpd mem        t
1   0  152  736     0     0     0   0    0    nan    0   nan    nan   138         inf     0   nan  25        0
2   0  304 1592     0     0  0.01   1   24   6778   58     0  16380   413      116634    64  8.57  25 0.003541
3   0  456 2328     0     0  0.01  15 2460 214081 3590     0 312418 61756 5.37429e+06 16967 18.46  25 0.011491
4   1  608 3064     0     0  0.01   4  427 188771 1062     0 469496 21953 9.70513e+06  8667  6.89  25 0.002262

Using the simple upper bound V-1, and using a different seed:
Bicliques> ./GraphGen clique 16 | ./BCCbySAT +0,15 binsearch "" "" "" 0 ST ""
# "./BCCbySAT" "+0,15" "binsearch" "" "" "" "0" "ST" ""
# ** Parameters **
# B                                     binsearch +0 15
# sb-options                            basic-sb with-ssb
# pt-option                             cover
# di-option                             binsearch
# so-option                             default-solver
# comments-option                       with-comments
# bicliques-option                      with-bicliques
# solver-timeout(s)                     2147483647
# sb-rounds                             100
# num_e-seeds                           1
#  e-seeds                              0
# statistics-output                     ST
# log-output                            null
# ** Results **
# sb-stats                              100 : 1 1 1; 0
# result-type                           exact
# bcc                                   = 4
2 3 4 7 9 11 13 15 | 1 5 6 8 10 12 14 16
1 2 3 9 12 13 14 16 | 4 5 6 7 8 10 11 15
1 3 4 6 8 9 11 14 | 2 5 7 10 12 13 15 16
2 3 7 8 10 11 12 14 | 1 4 5 6 9 13 15 16
B sat maxn    c ptime stime elimc rts  cfs  cfsps  dec decpr  decps     r1        r1ps   cfl cflpd mem        t
8   1 1216 6015     0     0  0.01   2  114  19057 1795     0 300067   8441 1.41107e+06  5213  0.19  26 0.005982
4   1  608 3067     0     0  0.01   5  510  96009 1177     0 221574  25407 4.78294e+06 10276  8.95  25 0.005312
2   0  304 1593     0     0  0.01   1   14   7487   54     0  28877    162       86631    26 25.71  25  0.00187
3   0  456 2330     0     0  0.01  19 3648 216923 5023     0 298686 102400 6.08908e+06 27258 22.08  25 0.016817

Bicliques> ./GraphGen clique 16 | ./BCCbySAT +0,15 binsearch,-ssb "" "" "" 0 ST ""
1 3 5 9 10 12 13 15 | 2 4 6 7 8 11 14 16
1 3 4 7 11 13 15 16 | 2 5 6 8 9 10 12 14
1 2 4 6 7 9 10 15 | 3 5 8 11 12 13 14 16
1 2 4 9 12 13 14 16 | 3 5 6 7 8 10 11 15
Bicliques> cat ST
B sat maxn    c ptime stime elimc rts  cfs  cfsps  dec decpr  decps     r1        r1ps   cfl cflpd mem        t
8   1 1216 6008     0     0  0.01   2  118  22771 1637     0 315901   7748 1.49518e+06  4624  0.47  26 0.005182
4   1  608 3064     0     0  0.01   5  511 184543 1219     0 440231  26071 9.41531e+06 10873  8.17  25 0.002769
2   0  304 1592     0     0  0.01   1   33  17188   81     0  42188    467      243229    95 11.21  25  0.00192
3   0  456 2328     0     0  0.01  31 6458 224384 8788     0 305340 177216  6.1574e+06 50432 19.38  25 0.028781

Upper and lower bound will be automatically adjusted to the bounds given by symmetry-breaking resp.
the simple upper bound, and thus we get the same result by
Bicliques> ./GraphGen clique 16 | ./BCCbySAT 0,1000 binsearch "" "" "" 0 ST ""
# "./BCCbySAT" "0,1000" "binsearch" "" "" "" "0" "ST" ""
# ** Parameters **
# B                                     binsearch 0 1000
Bicliques> cat ST
B sat maxn    c ptime stime elimc rts  cfs  cfsps  dec decpr  decps     r1        r1ps   cfl cflpd mem        t
8   1 1216 6015     0     0  0.01   2  114  21889 1795     0 344662   8441 1.62078e+06  5213  0.19  26 0.005208
4   1  608 3067     0     0  0.01   5  510  84689 1177     0 195450  25407 4.21903e+06 10276  8.95  25 0.006022
2   0  304 1593     0     0  0.01   1   14   7584   54     0  29252    162       87757    26 25.71  25 0.001846
3   0  456 2330     0     0  0.01  19 3648 214361 5023     0 295158 102400 6.01716e+06 27258 22.08  25 0.017018


One can investigate the symmetry-breaking as preprocessing:
Bicliques> time ./GraphGen grid 10 11 | ./BCC2SAT 0 "" -cs 3000000 ""
c sb-stats                              3000000 : 33 41.0195 50; 1.7225
c sb-seed                               2128577
c restricted-edges                      0
c ** Statistics **
c V                                     110
c E                                     199
c B                                     50
c total-lit-occurrences                 1359400
p cnf 20950 645149
real	3m3.991s
user	3m3.879s
sys	0m0.036s

Then one can provide this sb-sequence:
Bicliques> time ./GraphGen grid 10 11 | ./BCCbySAT 55 "" "" "" 1 2128577 ST ""
# "./BCCbySAT" "55" "" "" "" "1" "2128577" "ST" ""
# ** Parameters **
# B                                     downwards 0 55
# sb-option                             basic-sb
# pt-option                             cover
# di-option                             downwards
# so-option                             default-solver
# comments-option                       with-comments
# bicliques-option                      with-bicliques
# solver-timeout(s)                     2147483647
# sb-rounds                             1
# num_e-seeds                           1
#  e-seeds                              2128577
# statistics-output                     ST
# log-output                            null
# ** Results **
# sb-stats                              1 : 50 50 50; 0
# result-type                           exact
# bcc                                   = 55
real	0m0.755s
user	0m0.742s
sys	0m0.029s


As the solution-statistics shows:
Bicliques> cat ST
 B sat  maxn      c ptime stime elimc rts   cfs cfsps    dec decpr  decps          r1        r1ps    cfl cflpd mem        t
54   0 22626 696587  0.06  0.26  0.02  68 19772 31407 112721     0 179051 4.62649e+06 7.34892e+06 475498 31.84  61 0.629547

no solution was produced (since not needed).
In order to produce a solution:
Bicliques> time ./GraphGen grid 10 11 | ./BCCbySAT 56 "" "" "" 1 2128577 ST ""
...
5,5 | 4,5 5,4 5,6 6,5
1,4 | 1,3 1,5 2,4
4,1 5,2 6,1 | 5,1
9,7 10,8 | 9,8 10,7

real	0m1.211s
user	0m1.192s
sys	0m0.047s


Even with sb=49 the unsat-result takes much longer.


The above were biclique-cover-problems; now a partition-problem:
Bicliques> ./GraphGen clique 6 | ./BCCbySAT 6 partition2 "" "" "" "" ST ""
# "./BCCbySAT" "6" "partition2" "" "" "" "" "ST" ""
# ** Parameters **
# B                                     downwards 0 6
# sb-option                             basic-sb
# pt-option                             partition-quadratic
# di-option                             downwards
# so-option                             default-solver
# comments-option                       with-comments
# bicliques-option                      with-bicliques
# solver-timeout(s)                     2147483647
# sb-rounds                             100
# num_e-seeds                           0
# statistics-output                     ST
# log-output                            null
# ** Results **
# sb-stats                              100 : 1 1 1; 0
# result-type                           exact
# bcp                                   = 5

No output here, since the automatic upper-bound V-1=5 is never tried:
Bicliques> cat ST
B sat maxn   c ptime stime elimc rts   cfs  cfsps   dec decpr  decps     r1        r1ps    cfl cflpd mem        t
4   0  108 492     0     0     0  30 6401 230219 7681     0 276255 142368 5.12041e+06 56090  27.6  25 0.027804

In order to overcome this, an additional isolated vertex can be added to the input-graph (which increases
that bound, but otherwise has not much influence):
Bicliques> echo "new_vertex" > FILE
Bicliques> ./GraphGen clique 6 | cat - FILE | ./BCCbySAT 6 partition2 "" "" "" "" ST ""
# "./BCCbySAT" "6" "partition2" "" "" "" "" "ST" ""
# ** Parameters **
# B                                     downwards 0 6
# sb-options                            basic-sb with-ssb
# pt-option                             partition-quadratic
# di-option                             downwards
# so-option                             default-solver
# comments-option                       with-comments
# bicliques-option                      with-bicliques
# solver-timeout(s)                     2147483647
# sb-rounds                             100
# num_e-seeds                           0
# statistics-output                     ST
# log-output                            null
# ** Results **
# sb-stats                              100 : 1 1 1; 0
# result-type                           exact
# bcp                                   = 5
1 6 | 2 5
2 5 6 | 3 4
1 | 3 4 6
3 | 4
2 | 5
Bicliques> cat ST
B sat maxn   c ptime stime elimc rts   cfs  cfsps   dec decpr  decps     r1        r1ps    cfl cflpd mem        t
5   1  145 714     0     0     0   1   18   7692   81     0  34615    553      236325   159  2.45  25  0.00234
4   0  116 544     0     0     0  29 5393 288349 6584     0 352029 120946 6.46666e+06 44579    28  25 0.018703


Remarks on logging:

If a log-file is given (or /dev/stdout used), then the (complete) minisat-
output is only shown after a completed run (which may have been aborted);
in order to see the progress of the current run, in the calling directory
the file SystemCalls_Popen_out_* can be inspected.


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
        "0.10.1",
        "6.5.2023",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Bicliques/BCCbySAT.cpp",
        "GPL v3"};

  using namespace Bicliques2SAT;
  using namespace BCC2SAT;

  const std::string error = "ERROR[" + proginfo.prg + "]: ";
  const std::string comment = "# ";

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg
         << " B algo-options format-options"
            " sb-rounds timeout seeds stats log\n\n"
    " B              : " << "[+]biclique-cover-size, default is \"+0\"\n"
    " algo-options   : " << Environment::WRP<SB>{} << "\n"
    "                : " << Environment::WRP<SS>{} << "\n"
    "                : " << Environment::WRP<PT>{} << "\n"
    "                : " << Environment::WRP<DI>{} << "\n"
    "                : " << Environment::WRP<SO>{} << "\n"
    " format-options : " << Environment::WRP<DC>{} << "\n"
    "                : " << Environment::WRP<BC>{} << "\n"
    " timeout        : " << "in s, default is " << default_sec << "\n"
    " sb-rounds      : " << "default is " << default_sb_rounds << "\n"
    " seeds          : " << "sequence, can contain \"t\" or \"r\"" << "\n"
    " stats          : " << "filename for solving-stats, default is null\n"
    " log            : " << "filename for solving-log, default is null\n\n"
    " reads a graph from standard input, and attempts to compute its"
    " bcc/bcp-number:\n\n"
    "  - Arguments \"\" (the empty string) yield the default-values.\n"
    "  - Using \"+\" for B means the increment added to the lower-bound.\n"
    "  - Default-values for the options are the first possibilities given.\n"
    "  - For stats the special value \"t\" creates an automatic name (with timestamp).\n"
    "  - By using \"/dev/stdout\" for stats/log the output goes to standard output.\n\n"
;
    return true;
  }

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  if (argc != 9) {
    std::cerr << error <<
      "Exactly eight arguments (B, algo-opt, form-opt, sb-rounds, timeout, seeds, stats, log)"
      " needed, but " << argc-1 << " provided.\n";
    return int(Error::missing_parameters);
  }

  const auto bounds0 = read_vecvalorinc(argv[1]);
  if (bounds0.size() > 2) {
    std::cerr << error <<
      "Bounds-argument has " << bounds0.size() << " > 2 components.\n";
    return int(Error::faulty_parameters);
  }
  const alg2_options_t algopt =
    Environment::translate<alg2_options_t>()(argv[2], sep);
  const DI di = std::get<DI>(algopt);
  const Bounds bounds = extract_bounds(di, bounds0);
  const format2_options_t formopt =
    Environment::translate<format2_options_t>()(argv[3], sep);
  const auto sec = read_uint_t(argv[4], default_sec);
  const var_t sb_rounds = read_var_t(argv[5], default_sb_rounds);
  const RandGen::vec_eseed_t seeds = RandGen::extract_seeds(argv[6]);
  const auto [stats, statsname] = read_stats(argv[7], proginfo.prg, error);
  const std::string logname = argv[8];
  const auto log = read_log(logname, error);

  if (std::get<SB>(algopt) != SB::none and sb_rounds == 0) {
    std::cerr << error <<
      "Symmetry-breaking on, but number of rounds is zero.\n";
    return int(Error::bad_sb);
  }

  const DC dc = std::get<DC>(formopt);
  const BC bc = std::get<BC>(formopt);
  if (dc == DC::with) {
    commandline_output(std::make_tuple(DC::with), comment, std::cout,
                       argc, argv);
    Environment::DWW::prefix = comment;
    using Environment::DWW; using Environment::DHW;
    std::cout <<
      DHW{"Parameters"} <<
      DWW{"B"} << bounds << "\n" <<
      DWW{"sb-options"} << std::get<SB>(algopt) << " " <<
                           std::get<SS>(algopt) << "\n" <<
      DWW{"pt-option"} << std::get<PT>(algopt) << "\n" <<
      DWW{"di-option"} << di << "\n" <<
      DWW{"so-option"} << std::get<SO>(algopt) << "\n" <<
      DWW{"comments-option"} << dc << "\n" <<
      DWW{"bicliques-option"} << bc << "\n" <<
      DWW{"solver-timeout(s)"} << sec << "\n";
    if (std::get<SB>(algopt) != SB::none) {
      std::cout <<
        DWW{"sb-rounds"} << sb_rounds << "\n" <<
        DWW{"num_e-seeds"} << seeds.size() << "\n";
      if (not seeds.empty())
        std::cout <<
          DWW{" e-seeds"} << RandGen::ESW{seeds} << "\n";
    }
    std::cout <<
      DWW{"statistics-output"};
    print(std::cout, {stats,statsname});
    std::cout <<
      DWW{"log-output"};
    print(std::cout, {log,logname});
    std::cout <<
      DHW{"Results"};
    std::cout.flush();
  }

  const auto G = Graphs::make_AdjVecUInt(std::cin, Graphs::GT::und);
  BC2SAT T(G, bounds);
  const auto res = T.sat_solve(log.pointer(), algopt, sb_rounds, sec, seeds);
  log.close();

  res.output(dc == DC::with ? &std::cout : nullptr, bc, G, stats.pointer());
  stats.close();

}

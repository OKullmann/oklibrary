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


A bigger example:

Bicliques> QBRG "a140 e100 a100 100" "100*4,4;50*2,4;100*3,2,1/3;100*4,10,1/4" "" 0 > Test.qdimacs
# p cnf 428 350
Bicliques> time cat Test.qdimacs | ./GSM +sba "" "" "" > R.qcnf
real	0m19.077s
user	0m18.912s
sys	0m0.615s
Bicliques> cat Test.qdimacs R.qcnf | ./GCGeq
# 0: O = p cnf 428 350
# 0: G = p cnf 137 350
# 0: n_g = 137
# 1: O = p cnf 428 350
# 1: G = p cnf 126 350
# 1: n_g = 91
Bicliques> time cat Test.qdimacs | ./GSM +sba,nopre "" "" "" > R.qcnf
real	0m5.410s
user	0m5.431s
sys	0m0.437s


Analysis of the global conflict graph:
Bicliques> cat Test.qdimacs | ./QCNF2gCNF | ./CNFstats
Dimacs: 137 350; L = 598

Clauses L1 350 : 0 1.70857 7; 1.32453 1
0:59 1:131 2:60 3:65 4:27 5:5 6:2 7:1
L2 8 : 1 43.75 131; 44.583 43

Variables 137 137 137
  trivial-pure-singular-1singular-non1singular-nonsingular 0 39 51 4 47 47
degree L1 137 : 1 4.36496 11; 2.09999 4
1:8 2:17 3:25 4:30 5:27 6:8 7:8 8:7 9:5 10:1 11:1
L2 11 : 1 12.4545 30; 10.5106 8
muldegree L1 137 : 0 4.42336 30; 5.28695 3
0:39 1:4 2:18 3:16 4:16 5:3 6:17 7:1 8:2 9:2 12:8 15:4 16:1 18:3 20:2 30:1
L2 16 : 1 8.5625 39; 10.3342 3.5
maxdegree L1 137 : 1 3.14599 8; 1.44273 3
1:12 2:40 3:40 4:20 5:14 6:9 7:1 8:1
L2 8 : 1 17.125 40; 15.4775 13
mindegree L1 137 : 0 1.21898 5; 1.06905 1
0:39 1:51 2:29 3:15 4:2 5:1
L2 6 : 1 22.8333 51; 20.3216 22
litdegree L1 274 : 0 2.18248 8; 1.59311 2
0:39 1:63 2:69 3:55 4:22 5:15 6:9 7:1 8:1
L2 9 : 1 30.4444 69; 26.754 22

Conflict graph 350 600
Degrees L1 350 : 0 3.42857 21; 3.21249 3
0:80 1:36 2:46 3:42 4:32 5:30 6:30 7:19 8:11 9:8 10:4 11:5 12:2 13:2 14:1 15:1 21:1
L2 17 : 1 20.5882 80; 22.0682 11
Sizes of components L1 81 : 1 4.32099 270; 29.8889 1
1:80 270:1
L2 2 : 1 40.5 80; 55.8614 40.5
non-trivial:
L1 1 : 270 270 270; 0 270
270:1
L2 1 : 1 1 1; 0 1

Since we have only one non-trivial component (with 270 clauses; plus
80 trivial components), we just solve the whole thing:

Bicliques> time cat Test.qdimacs | ./QCNF2gCNF | ./CNF2cg | ./BCCbySAT 137 +sb -bcl "" "" "" ST ""
# "./BCCbySAT" "137" "+sb" "-bcl" "" "" "" "ST" ""
# ** Parameters **
# B                                     downwards 0 137
# sb-options                            basic-sb with-ssb
# pt-option                             cover
# di-option                             downwards
# so-option                             default-solver
# comments-option                       with-comments
# bicliques-option                      without-bicliques
# solver-timeout(s)                     2147483647
# sb-rounds                             100
# num_e-seeds                           0
# statistics-output                     ST
# log-output                            null
# ** Results **
# sb-stats                              100 : 75 79.4 85; 2.47411
# result-type                           exact
# bcc                                   = 91
real	4m16.536s
user	4m15.357s
sys	0m5.193s
Bicliques> cat ST
  B sat   maxn           c ptime stime elimc rts  cfs cfsps    dec decpr decps          r1   r1ps   cfl cflpd  mem       t
136   1 176800 1.69871e+07  1.54 16.25  0.41   4  436    24 288842     0 16198      780645  43777 31682  0.11 1166 17.8322
122   1 158600 1.52384e+07  1.41  14.4   0.3   4  423    27 182013     0 11486      607654  38348 24454  0.18 1036 15.8459
114   1 148200 1.42392e+07   1.3 13.43  0.24   2  173    12 110596     0  7497      326829  22154 10938   0.1 1008 14.7523
105   1 136500 1.31151e+07   1.2 12.14  0.17   3  200    15  60055     0  4497      318642  23859  8013  0.22  810 13.3553
104   1 135200 1.29902e+07  1.23 12.02  0.18   2  171    13  61082     0  4606      289872  21858  6236   0.1  806 13.2613
102   1 132600 1.27404e+07  1.11 11.97  0.29   3  213    16  12039     0   920      276188  21104  8175  0.22  799 13.0871
101   1 131300 1.26155e+07  1.16 11.79  0.28   2  138    11  11228     0   866      191397  14767  4211  0.09  795 12.9615
100   1 130000 1.24906e+07  1.15 11.79  0.26   2  166    13  11083     0   856      194788  15050  4081  0.24  773  12.943
 99   1 128700 1.23657e+07  1.08 11.62  0.25   3  241    19  14426     0  1136      255096  20081  8062   0.4  770 12.7032
 98   1 127400 1.22408e+07  1.13 11.53  0.23   2  142    11   8449     0   667      182973  14443  4545  0.07  769 12.6684
 97   1 126100 1.21159e+07  1.09 11.36  0.22   3  210    17   8586     0   689      224276  17999  3997   0.3  773 12.4605
 96   1 124800  1.1991e+07  1.11 11.29   0.2   4  419    34   8514     0   686      326086  26280  7984  0.76  770 12.4083
 95   1 123500 1.18661e+07  1.06 11.25  0.19   3  206    17   7035     0   571      204298  16595  5218   0.4  732 12.3106
 94   1 122200 1.17412e+07  1.02 11.07  0.17   4  414    34   7011     0   580      276370  22848  9189  0.95  712 12.0961
 93   1 120900 1.16163e+07  1.02 10.94  0.16   3  212    18   6553     0   548      189806  15861  4923  0.79  709 11.9669
 92   1 119600 1.14914e+07  1.07 10.84  0.14   4  425    36   5648     0   474      270325  22687  6502  1.13  705 11.9152
 90   0 117000 1.12416e+07  1.02  10.8  0.11  31 6759   569  16951     0  1428 1.32751e+06 111808 69935 26.76  691 11.8731
Fastest with +sb is binsearch:
Bicliques> time cat Test.qdimacs | ./QCNF2gCNF | ./CNF2cg | ./BCCbySAT 137 +sb,binsearch -bcl "" "" "" ST ""
real	1m9.880s
user	1m9.616s
sys	0m1.355s
Bicliques> cat ST
  B sat   maxn           c ptime stime elimc rts  cfs cfsps   dec decpr decps          r1   r1ps   cfl cflpd mem       t
111   1 144300 1.38645e+07  1.25 12.93  0.22   2  185    13 99731     0  7029      332929  23466  7321  0.18 997 14.1876
 94   1 122200 1.17412e+07  1.05 10.97  0.17   4  414    34  7011     0   583      276370  22971  9189  0.95 712  12.031
 89   0 115700 1.11167e+07  0.99 10.42   0.1   7 1170   102  4892     0   428      421674  36908  7689 15.66 687 11.4249
 92   1 119600 1.14914e+07  1.03 10.82  0.14   4  425    36  5648     0   476      270325  22793  6502  1.13 705   11.86
 90   0 117000 1.12416e+07  1.03 10.61  0.11  31 6759   577 16951     0  1448 1.32751e+06 113402 69935 26.76 691 11.7062

As usual here, using "-no-pre" is much faster:
Bicliques> time cat Test.qdimacs | ./QCNF2gCNF | ./CNF2cg | ./BCCbySAT 137 +sb,nopre -bcl "" "" "" ST ""
Bicliques> cat ST
  B sat   maxn           c ptime stime elimc rts  cfs cfsps    dec decpr  decps          r1        r1ps   cfl cflpd mem        t
136   1 176800 1.69871e+07  1.08  0.26     0   2  135    99 345579     0 253593      592066      434470 15383  0.03 727  1.36273
 97   1 126100 1.21159e+07  0.79  0.16     0   2  138   143  44916     0  46690      289664      301104  2911  0.27 545 0.962005
 96   1 124800  1.1991e+07  0.77  0.16     0   2  124   131  40145     0  42562      277192      293882  2420  0.17 542 0.943207
 95   1 123500 1.18661e+07  0.79  0.17     0   2  169   174  32714     0  33611      336886      346122  3440   0.2 540 0.973315
 94   1 122200 1.17412e+07  0.83  0.15     0   2  147   149  19362     0  19586      255358      258309  2454  0.28 536 0.988577
 93   1 120900 1.16163e+07   0.8  0.16     0   2  144   149  16019     0  16564      239089      247230  1888  0.37 533 0.967072
 92   1 119600 1.14914e+07  0.78  0.16     0   2  113   118  15915     0  16659      231126      241931  2483   0.2 524 0.955337
 91   1 118300 1.13665e+07  0.77  0.16     0   3  207   220  12353     0  13125      352184      374191  2193  1.31 522 0.941187
 90   0 117000 1.12416e+07  0.76  0.14     0  28 5073  5114  20820     0  20989 1.96552e+06 1.98151e+06 57048 21.61 519  0.99193

Fastest +sba with up:
Bicliques> time cat Test.qdimacs | ./QCNF2gCNF | ./CNF2cg | ./BCCbySAT +0 +sba,nopre,up -bcl "" "" "" ST ""
 ** Results **
# sb-stats                              100 : 90 90 90; 0
# result-type                           exact
# bcc                                   = 91
real	0m4.872s
user	0m4.920s
sys	0m0.375s
Bicliques> cat ST
 B sat   maxn           c ptime stime elimc rts cfs cfsps dec decpr decps     r1   r1ps cfl cflpd mem        t
90   0 117000 1.12416e+07  0.74     0     0   0   0     0   0   nan     0 116111 157467   0   nan 507 0.737368
91   1 118300 1.13665e+07  0.75  0.15     0   1   6     7 767     0   852 123622 137278  10  9.09 513 0.900526


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
        "0.7.0",
        "7.5.2023",
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
    "                : " << Environment::WRP<SS>{} << "\n"
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

  const auto F = DimacsTools::read_strict_GslicedCNF(std::cin);
  const GlobRepl GR(F);
  const auto R = GR.solve(nullptr, algopt, sb_rounds, sec, seeds);
  const DimacsTools::GslicedCNF Fnew(F.O(), R, F.other);
  std::cout << Fnew;
}

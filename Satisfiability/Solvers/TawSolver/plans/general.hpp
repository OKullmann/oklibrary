// Oliver Kullmann, 8.6.2013 (Swansea)
/* Copyright 2013, 2015, 2016 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Solvers/TawSolver/plans/general.hpp
  \brief Plans regarding updating and improving the TawSolver


  \todo Create milestones


  \todo Create tests (JPY)
  <ul>
   <li> Only applications tests. </li>
   <li> As many variations of the tawSolver as possible should be tested;
   for the beginning at least the four "official versions". </li>
   <li> As usual, in directory app_tests one finds the bash-script,
   which runs the solver on test-examples found in that directory (see
   Satisfiability/Solvers/OKsolver/SAT2002 for an example). </li>
  </ul>


  \todo Run benchmarks (JPY)
  <ul>
   <li> An overview is needed how the tawSolver (in various configurations)
   performs. </li>
   <li> The natural starting point is the SAT competition
   http://www.satcompetition.org/ , and all instances should be run, with
   a time-out of say 3600s (or shorter, if this takes too long). </li>
   <li> Currently tawSolver does not allow a timeout, but it can be achieved
   as follows:
   \verbatim
> timeout --signal=SIGINT 3600 tawSolver File
   \endverbatim
   (interrupt here after 3600s).
   </li>
   <li> Running through the crafted instances from SAT2014, with timeout of
   600sec; data/History (sc14_craftet_600). Solved instances:
    <ol>
     <li> Satisfiable: "289-*", "Chvatal-*", "Composite-*", "mod2-*",
     "prime-*", "Q3inK11-*", "rbsat-*", "rnd-*", "sgen3-*", "sgen4-*",
     "toughsat-*" , "VanDerWaerden-*". </li>
     <li> Unsatisfiable: "edges-*", "sgen6-*". </li>
    </ol>
   </li>
   <li> Now running the same with ttawSolver. XXX
   </li>
   <li> Running through the applications-instances from SAT2014, with timeout
   of 600 sec (sc14_app_600). Only satisfiable instances solved:
   "complete-*", "stable-*". </li>
   <li> Now running the same with ttawSolver. XXX </li>
  </ul>


  \todo Positive / negative occurrences
  <ul>
   <li> A compile-time switch POLARITY (default: 0) is to be created, which
   when active extends the clause data-structure by not just determining
   the current total length, but by having, for active clauses, the number
   of remaining positive and negative literals in it. </li>
   <li> And there are then global counters for the total counter of positive
   and negative clauses. If one of them is zero, then the clause-set is
   satisfiable.
    <ol>
     <li> This event should be indicated in the output. </li>
     <li> Perhaps then the assignment is not extended, since this information
     is enough. </li>
     <li> Furthermore information on number of definite Horn (precisely one
     positive literal) and/or definite dual Horn (precisely one negative
     literal) are interesting, with the goal to determine the "distance" from
     a Horn resp. dual Horn clause-set. For this a further compile-time
     switch, call it BIHORN, is needed. </li>
    <ol>
   </li>
   <li> Change of class Clause:
    <ol>
     <li> Instead of data-members length_, old_length we then have length_,
     npos_, nneg_ (old_length would no longer be needed due to
     old_length = npos_ + nneg_). </li>
     <li> Member functions increment, decrement would then be split into two
     versions each. And there would be the additional member functions npos,
     nneg. </li>
    </ol>
   </li>
   <li> Apparently the only other changes to the algorithm would be member
   functions reactivate_0/1 in class ChangeManagement, which for "0" (i.e.,
     re-activating a literal) needed to distinguish the two polarities.
    <ol>
     <li> Where to get this information (to distinguish between the two cases)?
     </li>
    </ol>
   </li>
  </ul>


  \todo Provide information
  <ul>
   <li> On the ExternalSources page the data needs to be entered. </li>
   <li> http://sourceforge.net/projects/tawsolver/ </li>
  </ul>


  \todo Timing
  <ul>
   <li> Test instances created by
   \verbatim
> VanderWaerdenCNF-O3-DNDEBUG 3 12 134
> VanderWaerdenCNF-O3-DNDEBUG 3 12 135
   \endverbatim
   (now in app_tests/test_cases/full).
   </li>
   <li> On cswsok (3GHz):
   \verbatim
# ID 237cbfc4d9b772a29e125928959af14cb4495d3e
# Version 2.6.6

s SATISFIABLE
c number_of_variables                   134
c number_of_clauses                     5172
c maximal_clause_length                 12
c number_of_literal_occurrences         22266
c running_time(sec)                     8.82
c number_of_nodes                       808309
c number_of_binary_nodes                404147
c number_of_1-reductions                9437027
c reading-and-set-up_time(sec)          0.002
c file_name                             VanDerWaerden_2-3-12_134.cnf
c options                               ""

s UNSATISFIABLE
c number_of_variables                   135
c number_of_clauses                     5251
c maximal_clause_length                 12
c number_of_literal_occurrences         22611
c running_time(sec)                     10.78
c number_of_nodes                       961949
c number_of_binary_nodes                480974
c number_of_1-reductions                11312180
c reading-and-set-up_time(sec)          0.003
c file_name                             VanDerWaerden_2-3-12_135.cnf
c options                               ""

s SATISFIABLE
c number_of_variables                   134
c number_of_clauses                     5172
c maximal_clause_length                 12
c number_of_literal_occurrences         22266
c running_time(sec)                     18.72
c number_of_nodes                       936151
c number_of_binary_nodes                468068
c number_of_1-reductions                11031117
c number_of_pure_literals               1492
c reading-and-set-up_time(sec)          0.002
c file_name                             VanDerWaerden_2-3-12_134.cnf
c options                               "PT5"

s UNSATISFIABLE
c number_of_variables                   135
c number_of_clauses                     5251
c maximal_clause_length                 12
c number_of_literal_occurrences         22611
c running_time(sec)                     19.37
c number_of_nodes                       953179
c number_of_binary_nodes                476589
c number_of_1-reductions                11285634
c number_of_pure_literals               1317
c reading-and-set-up_time(sec)          0.001
c file_name                             VanDerWaerden_2-3-12_135.cnf
c options                               "PT5"

s SATISFIABLE
c number_of_variables                   134
c number_of_clauses                     5172
c maximal_clause_length                 12
c number_of_literal_occurrences         22266
c running_time(sec)                     10.61
c number_of_nodes                       968509
c number_of_binary_nodes                484254
c number_of_1-reductions                11308431
c number_of_solutions                   1
c reading-and-set-up_time(sec)          0.001
c file_name                             VanDerWaerden_2-3-12_134.cnf
c options                               "A19"

c number_of_clauses                     5251
c maximal_clause_length                 12
c number_of_literal_occurrences         22611
c running_time(sec)                     10.70
c number_of_nodes                       961949
c number_of_binary_nodes                480974
c number_of_1-reductions                11312180
c number_of_solutions                   0
c reading-and-set-up_time(sec)          0.001
c file_name                             VanDerWaerden_2-3-12_135.cnf
c options                               "A19"

s SATISFIABLE
c number_of_variables                   134
c number_of_clauses                     5172
c maximal_clause_length                 12
c number_of_literal_occurrences         22266
c running_time(sec)                     19.20
c number_of_nodes                       969297
c number_of_binary_nodes                484648
c number_of_1-reductions                11427083
c number_of_solutions                   1
c reading-and-set-up_time(sec)          0.002
c file_name                             VanDerWaerden_2-3-12_134.cnf
c options                               "T5A19"

c number_of_variables                   135
c number_of_clauses                     5251
c maximal_clause_length                 12
c number_of_literal_occurrences         22611
c running_time(sec)                     19.23
c number_of_nodes                       953175
c number_of_binary_nodes                476587
c number_of_1-reductions                11286127
c number_of_solutions                   0
c reading-and-set-up_time(sec)          0.001
c file_name                             VanDerWaerden_2-3-12_135.cnf
c options                               "T5A19"

> oklib timing
# cswsok (no other process running)

tawSolver VanDerWaerden_2-3-12_134.cnf
8.81 8.88 8.81 8.87 8.93
tawSolver VanDerWaerden_2-3-12_135.cnf
10.78 10.65 10.70 10.65 10.73
ttawSolver VanDerWaerden_2-3-12_134.cnf
18.72 18.68 18.63 18.74 18.72
ttawSolver VanDerWaerden_2-3-12_135.cnf
19.36 19.35 19.39 19.45 19.23
ctawSolver VanDerWaerden_2-3-12_134.cnf
10.61 10.45 10.68 10.54 10.48
ctawSolver VanDerWaerden_2-3-12_135.cnf
10.70 10.67 10.64 10.64 10.63
cttawSolver VanDerWaerden_2-3-12_134.cnf
19.20 19.16 19.17 19.26 19.14
cttawSolver VanDerWaerden_2-3-12_135.cnf
19.23 19.16 19.17 19.15 19.23


# ID 4a74a73e720d378ba92dac09e95d9b76b46878db
# Version 2.6.7
> make timing
# (system-gcc 4.7.1) versus
> oklib timing (one directory up, with oklib-gcc 4.7.3).
# cswsok (no other process running)

tawSolver VanDerWaerden_2-3-12_134.cnf
8.71 8.67 8.70 8.75 8.76
8.87 8.77 8.81 8.89 8.91
tawSolver VanDerWaerden_2-3-12_135.cnf
10.64 10.56 10.60 10.65 10.57
10.90 10.74 10.76 10.70 10.81
ttawSolver VanDerWaerden_2-3-12_134.cnf
18.74 18.65 18.66 18.67 18.65
18.86 18.81 18.82 18.74 18.82
ttawSolver VanDerWaerden_2-3-12_135.cnf
19.23 19.25 19.22 19.25 19.25
19.45 19.41 19.35 19.38 19.45
ctawSolver VanDerWaerden_2-3-12_134.cnf
10.76 10.76 10.65 10.71 10.71
10.63 10.65 10.53 10.54 10.56
ctawSolver VanDerWaerden_2-3-12_135.cnf
10.84 10.92 10.98 10.84 10.85
10.72 10.72 10.69 10.71 10.69
cttawSolver VanDerWaerden_2-3-12_134.cnf
19.53 19.57 19.53 19.55 19.53
19.20 19.26 19.39 19.41 19.29
cttawSolver VanDerWaerden_2-3-12_135.cnf
19.47 19.54 19.47 19.43 19.47
19.20 19.18 19.30 19.25 19.26
   \endverbatim
   </li>
  </ul>


  \todo Provide Doxygen documentation


  \todo Improving the clause-bitsets
  <ul>
   <li> The following should be transferred to a the general library.
   Otherwise this todo is completed (from the solver these tools were
   removed after version 1.3.7). </li>
   <li> http://graphics.stanford.edu/~seander/bithacks.html
   contains various bit-level operations. </li>
   </li>
   <li> At
   http://graphics.stanford.edu/~seander/bithacks.html#IntegerLogObvious
   we find (slightly adapted):
   \verbatim
// IF YOU KNOW v IS A POWER OF 2:
unsigned int v;  // 32-bit value to find the log2 of
const unsigned int b[] = {0xAAAAAAAA, 0xCCCCCCCC, 0xF0F0F0F0, 0xFF00FF00, 0xFFFF0000};
unsigned int r = (v & b[0]) != 0;
for (i = 4; i > 0; --i) r |= ((v & b[i]) != 0) << i;

# The values b[0], ..., b[4]:
1010 1010 1010 1010 1010 1010 1010 1010
1100 1100 1100 1100 1100 1100 1100 1100
1111 0000 1111 0000 1111 0000 1111 0000
1111 1111 0000 0000 1111 1111 0000 0000
1111 1111 1111 1111 0000 0000 0000 0000
   \endverbatim
   </li>
   <li> The result computed is
     (bool(v&b[0]) << 0) | ... | (bool(v&b[4]) << 4)
   Note that the order doesn't matter. </li>
   <li> A proper C++ implementation of "log2s" ("s" for "special"):
   \verbatim
#incluce <limits>
#include <cassert>
#include <cmath>

constexpr int B {std::numeric_limits<unsigned int>::digits};
static_assert(B == 32, "Type \"unsigned int\" has not 32 bits.");
constexpr int N = 5;

constexpr int log2s(const unsigned int v) {
  assert((unsigned int) std::exp2(std::log2(v)) == v);
  constexpr unsigned int b[N] = {0xAAAAAAAA,0xCCCCCCCC,0xF0F0F0F0,0xFF00FF00,0xFFFF0000};
  unsigned int r = (v & b[0]) != 0;
  for (int i = 1; i < N; ++i) r |= ((v & b[i]) != 0) << i;
  return r;
}
   \endverbatim
   </li>
   <li> For B=64 (N=6) we have:
   \verbatim
const unsigned int b[N] = {0xAAAAAAAAAAAAAAAA, 0xCCCCCCCCCCCCCCCC, 0xF0F0F0F0F0F0F0F0, 0xFF00FF00FF00FF00, 0xFFFF00000FFFF0000, 0xFFFFFFFF00000000};
   \endverbatim
   </li>
   <li> A recursive computation of the array b of length N, via the values
   bp(N,0), ..., bp(N,N-1) ("bp" for bit pattern"), is:
   \verbatim
constexpr unsigned int pow2(const unsigned e) { return (e==0)?1:2*pow2(e-1); }
constexpr unsigned int pow22(const unsigned e) {return pow2(pow2(e));}

constexpr unsigned int bp(const unsigned N, const unsigned i) {
  // assert(N >= 0);
  // assert(i >= 0);
  // assert(i < N);
  return (i < N-1) ? bp(N-1,i) * (1 + pow22(N-1)) : pow22(N) - pow22(N-1);
}
   \endverbatim
   </li>
   <li> Then the general computation is:
   \verbatim
#include <limits>
#include <cassert>
#include <cmath>

constexpr int log2(const unsigned int n) { return (n <= 1) ? 0 : 1+log2(n/2); }

constexpr int B {std::numeric_limits<unsigned int>::digits};
constexpr unsigned int N = log2((unsigned int) B);
static_assert(pow2(N) == (unsigned int) B, "Number of bits in \"unsigned int\" not a power of 2.");

static_assert(N==5 or N==6, "Unexpected size of type \"unsigned int\".");
const unsigned int b[6] {bp(N,0),bp(N,1),bp(N,2),bp(N,3),bp(N,4), (N==6) ? bp(N,5) : 0};
// Unfortunately there is no reasonable way in C++ to just define b[N].

inline int log2s(const unsigned int v) {
  assert((unsigned int) std::exp2(std::log2(v)) == v);
  unsigned int r = (v & b[0]) != 0;
  for (unsigned int i = 1; i < N; ++i) r |= ((v & b[i]) != 0) << i;
  return r;
}
   \endverbatim
   </li>
   <li> As constexpr-functions:
   \verbatim
constexpr int log2sr(const unsigned v, const unsigned i) {
  // assert(i < N);
  return (i == 0) ? ((v & bp(N,0)) != 0)  : log2sr(v, i-1) | (((v & bp(N,i)) != 0) << i);
}
constexpr unsigned int log2sc(const unsigned int v) {
  return log2sr(v, N-1);
}
   \endverbatim
   (Remark: In OKs repository under "B149Bitposition.cpp" a more general,
   templatised version can be found.)
   </li>
  </ul>


  \todo Optimising weights (JPY)
  <ul>
   <li> First considering VanDerWaerden_2-3-12_135.cnf. </li>
   <li> Start with current weights:
   \verbatim
TawSolver> ./RunWeights WEIGHT_2_CLAUSES 2 10 0.1 VanDerWaerden_2-3-12_135.cnf ""
> E1=read_satstat("Result_tawSolver")
> plot(E1$x,E1$nds)
> min(E1$nds)
[1] 975045
> E1[seq(29,37),]
     x  rn   rc mcl     t sat    nds   bnds       r1 pls ptime
29 4.8 135 5251  12 10.85   0 977343 488671 11497281   0 0.005
30 4.9 135 5251  12 10.81   0 975045 487522 11472643   0 0.004
31 5.0 135 5251  12 10.85   0 975603 487801 11477414   0 0.005
32 5.1 135 5251  12 10.89   0 975313 487656 11478357   0 0.004
33 5.2 135 5251  12 10.80   0 976191 488095 11490982   0 0.005
34 5.3 135 5251  12 10.76   0 976097 488048 11494725   0 0.004
35 5.4 135 5251  12 10.94   0 977215 488607 11510241   0 0.005
36 5.5 135 5251  12 10.79   0 977449 488724 11512264   0 0.004
37 5.6 135 5251  12 10.86   0 977495 488747 11515979   0 0.005

> plot(E2$x,E2$nds)
> min(E2$nds)
[1] 928059
> E2[seq(29,37),]
     x  rn   rc mcl     t sat    nds   bnds       r1  pls ptime
29 4.8 135 5251  12 18.66   0 929761 464880 11025046 1108 0.001
30 4.9 135 5251  12 18.76   0 929603 464801 11024841 1112 0.003
31 5.0 135 5251  12 18.82   0 928323 464161 11014210 1143 0.002
32 5.1 135 5251  12 18.75   0 928407 464203 11012720 1166 0.004
33 5.2 135 5251  12 18.66   0 929763 464881 11024475 1166 0.002
34 5.3 135 5251  12 18.73   0 929225 464612 11017499 1179 0.002
35 5.4 135 5251  12 18.69   0 928273 464136 11010516 1196 0.004
36 5.5 135 5251  12 18.77   0 928059 464029 11008118 1205 0.003
37 5.6 135 5251  12 18.87   0 929681 464840 11027249 1208 0.003

# x= 5.0 seems a reasonable compromise.

TawSolver> ./RunWeights WEIGHT_BASIS_OPEN 1.1 2.0 0.02 VanDerWaerden_2-3-12_135.cnf "-DWEIGHT_2_CLAUSES=5"

> E1=read_satstat("Result_tawSolver")
> plot(E1$x,E1$nds)
> min(E1$nds)
[1] 966469
> E1[seq(16,26),]
      x  rn   rc mcl     t sat    nds   bnds       r1 pls ptime
16 1.40 135 5251  12 10.83   0 982193 491096 11580933   0 0.003
17 1.42 135 5251  12 10.86   0 977437 488718 11521053   0 0.001
18 1.44 135 5251  12 10.82   0 972857 486428 11460593   0 0.004
19 1.46 135 5251  12 10.67   0 967827 483913 11397547   0 0.004
20 1.48 135 5251  12 10.72   0 969641 484820 11417859   0 0.005
21 1.50 135 5251  12 10.75   0 966469 483234 11376615   0 0.001
22 1.52 135 5251  12 10.92   0 979757 489878 11546452   0 0.004
23 1.54 135 5251  12 10.85   0 980043 490021 11555949   0 0.005
24 1.56 135 5251  12 10.84   0 980257 490128 11544503   0 0.005
25 1.58 135 5251  12 10.91   0 976889 488444 11501144   0 0.004
26 1.60 135 5251  12 10.82   0 975603 487801 11477414   0 0.005

> E2=read_satstat("Result_ttawSolver")
> plot(E2$x,E2$nds)
> min(E2$nds)
[1] 928323

> E2[seq(20,30),]
      x  rn   rc mcl     t sat    nds   bnds       r1  pls ptime
20 1.48 135 5251  12 19.04   0 939901 469950 11163329 1165 0.004
21 1.50 135 5251  12 18.99   0 937969 468984 11134761 1163 0.003
22 1.52 135 5251  12 18.98   0 936247 468123 11110328 1163 0.002
23 1.54 135 5251  12 18.92   0 933985 466992 11085816 1147 0.003
24 1.56 135 5251  12 18.79   0 931153 465576 11051506 1139 0.002
25 1.58 135 5251  12 18.71   0 929917 464958 11035018 1150 0.003
26 1.60 135 5251  12 18.76   0 928323 464161 11014210 1143 0.002
27 1.62 135 5251  12 18.69   0 929299 464649 11024378 1149 0.002
28 1.64 135 5251  12 18.78   0 930451 465225 11035708 1154 0.002
29 1.66 135 5251  12 18.64   0 930383 465191 11031771 1170 0.003
30 1.68 135 5251  12 18.84   0 932059 466029 11046036 1167 0.002

# Here we have a clear difference: 1.5 for tawSolver, 1.6 for ttawSolver.
   \endverbatim
   </li>
   <li> With updated WEIGHT_2_CLAUSES, WEIGHT_BASIS_OPEN:
   \verbatim
TawSolver> ./RunWeights WEIGHT_5_CLAUSES 0.01 0.30 0.005 VanDerWaerden_2-3-12_135.cnf ""
> E1[seq(16,24),]
       x  rn   rc mcl     t sat    nds   bnds       r1 pls ptime
16 0.085 135 5251  12 10.68   0 967127 483563 11361843   0 0.005
17 0.090 135 5251  12 10.69   0 964125 482062 11333077   0 0.005
18 0.095 135 5251  12 10.60   0 963757 481878 11332793   0 0.004
19 0.100 135 5251  12 10.60   0 963365 481682 11330147   0 0.004
20 0.105 135 5251  12 10.65   0 963307 481653 11329159   0 0.002
21 0.110 135 5251  12 10.68   0 963193 481596 11334130   0 0.005
22 0.115 135 5251  12 10.63   0 963755 481877 11339048   0 0.005
23 0.120 135 5251  12 10.65   0 964259 482129 11348183   0 0.004
24 0.125 135 5251  12 10.67   0 965907 482953 11369838   0 0.004

> E2[seq(20,30),]
       x  rn   rc mcl     t sat    nds   bnds       r1  pls ptime
20 0.105 135 5251  12 18.83   0 932671 466335 11052740 1120 0.005
21 0.110 135 5251  12 18.79   0 931927 465963 11048695 1141 0.004
22 0.115 135 5251  12 18.63   0 930391 465195 11022002 1167 0.003
23 0.120 135 5251  12 18.72   0 929233 464616 11018405 1166 0.001
24 0.125 135 5251  12 18.87   0 928329 464164 11008388 1160 0.001
25 0.130 135 5251  12 18.81   0 928323 464161 11014210 1143 0.002
26 0.135 135 5251  12 18.72   0 929153 464576 11024301 1146 0.003
27 0.140 135 5251  12 18.75   0 929465 464732 11032722 1120 0.003
28 0.145 135 5251  12 18.87   0 930241 465120 11044985 1122 0.002
29 0.150 135 5251  12 18.91   0 931741 465870 11061705 1129 0.002
30 0.155 135 5251  12 18.86   0 931613 465806 11064423 1135 0.002

# tawsolver: x=0.11, ttawsolver: x=0.127
   \endverbatim
   </li>
   <li> With updated WEIGHT_2, WEIGHT_BASIS_OPEN, WEIGHT_5:
   \verbatim
TawSolver> ./RunWeights WEIGHT_4_CLAUSES 0.15 0.45 0.01 VanDerWaerden_2-3-12_135.cnf ""
# Multiple optima; rerunning with higher resolution
TawSolver> ./RunWeights WEIGHT_4_CLAUSES 0.25 0.40 0.001 VanDerWaerden_2-3-12_135.cnf ""
> E1[E1$nds==min(E1$nds),]
       x  rn   rc mcl     t sat    nds   bnds       r1 pls ptime
92 0.341 135 5251  12 10.78   0 962757 481378 11325664   0 0.005
> E2[E2$nds==min(E2$nds),]
       x  rn   rc mcl    t sat    nds   bnds       r1  pls ptime
46 0.295 135 5251  12 18.7   0 927705 463852 11007465 1179 0.002
   \endverbatim
   </li>
   <li> With updated WEIGHT_BASIS_OPEN, WEIGHT_5, WEIGHT_4:
   \verbatim
TawSolver> ./RunWeights WEIGHT_2_CLAUSES 4 6 0.05 VanDerWaerden_2-3-12_135.cnf ""
> E1[E1$nds==min(E1$nds),]
      x  rn   rc mcl    t sat    nds   bnds       r1 pls ptime
18 4.85 135 5251  12 10.7   0 962609 481304 11322162   0 0.005
> E2[E2$nds==min(E2$nds),]
   x  rn   rc mcl    t sat    nds   bnds       r1  pls ptime
21 5 135 5251  12 18.7   0 927705 463852 11007465 1179 0.003
   \endverbatim
   </li>
   <li> The minima for the tau-version are always much sharper and much less
   noisy. </li>
   <li> Re-examining WEIGHT_4:
   \verbatim
TawSolver> ./RunWeights WEIGHT_4_CLAUSES 0.27 0.37 0.001 VanDerWaerden_2-3-12_135.cnf ""
> E1[E1$nds==min(E1$nds),]
       x  rn   rc mcl     t sat    nds   bnds       r1 pls ptime
85 0.354 135 5251  12 10.64   0 962181 481090 11319517   0 0.003
> E2[E2$nds==min(E2$nds),]
       x  rn   rc mcl     t sat    nds   bnds       r1  pls ptime
26 0.295 135 5251  12 18.79   0 927705 463852 11007465 1179 0.002

# Value for tawSolver very unclear.
   \endverbatim
   </li>
   <li> Re-examining WEIGHT_5:
   \verbatim
TawSolver> ./RunWeights WEIGHT_5_CLAUSES 0.09 0.15 0.001 VanDerWaerden_2-3-12_135.cnf ""
> E1[E1$nds==min(E1$nds),]
      x  rn   rc mcl    t sat    nds   bnds       r1 pls ptime
21 0.11 135 5251  12 10.6   0 962181 481090 11319517   0 0.005
> E2[E2$nds==min(E2$nds),]
       x  rn   rc mcl    t sat    nds   bnds       r1  pls ptime
33 0.122 135 5251  12 18.9   0 927587 463793 11002176 1184 0.004
   \endverbatim
   </li>
   <li> Determining WEIGHT_6:
   \verbatim
TawSolver> ./RunWeights WEIGHT_6 0.02 0.12 0.001 VanDerWaerden_2-3-12_135.cnf ""
> E1[E1$nds==min(E1$nds),]
       x  rn   rc mcl     t sat    nds   bnds       r1 pls ptime
50 0.069 135 5251  12 10.69   0 962185 481092 11306038   0 0.004
> E2[E2$nds==min(E2$nds),]
       x  rn   rc mcl     t sat    nds   bnds       r1  pls ptime
58 0.077 135 5251  12 18.75   0 927685 463842 11004258 1183 0.004

TawSolver> ./RunWeights WEIGHT_6 0.067 0.08 0.0001 VanDerWaerden_2-3-12_135.cnf ""
> E1[E1$nds==min(E1$nds),]
        x  rn   rc mcl     t sat    nds   bnds       r1 pls ptime
25 0.0694 135 5251  12 10.65   0 962115 481057 11306120   0 0.005
> E2[E2$nds==min(E2$nds),]
        x  rn   rc mcl     t sat    nds   bnds       r1  pls ptime
87 0.0756 135 5251  12 18.84   0 927419 463709 10999237 1174 0.002
   \endverbatim
   <li>
   <li> Determine WEIGHT_BASIS_OPEN:
   \verbatim
TawSolver> ./RunWeights WEIGHT_BASIS_OPEN 1.3 1.8 0.01 VanDerWaerden_2-3-12_135.cnf ""
> E1[E1$nds==min(E1$nds),]
      x  rn   rc mcl     t sat    nds   bnds       r1 pls ptime
17 1.46 135 5251  12 10.54   0 961949 480974 11312180   0 0.005
> E2[E2$nds==min(E2$nds),]
     x  rn   rc mcl     t sat    nds   bnds       r1  pls ptime
31 1.6 135 5251  12 18.76   0 927419 463709 10999237 1174 0.002
   \endverbatim
   </li>
   <li> The weights we got are:
   \verbatim
tawSolver:
4.85 0.354 0.11 0.0694; 1.46
ttawSolver:
5 0.295 0.122 0.0756; 1.6
   \endverbatim
   Apparently these new settings do not improve node-counts for k >= 13. </li>
   <li> Now considering VanDerWaerden_2-3-13_160.cnf. Currently we have
   \verbatim
c number_of_variables                   160
c number_of_clauses                     7308
c maximal_clause_length                 13
c number_of_literal_occurrences         31804
c running_time(sec)                     83.33
c number_of_nodes                       5638667
c number_of_binary_nodes                2819333
c number_of_1-reductions                71942645
c reading-and-set-up_time(sec)          0.006

c running_time(sec)                     148.86
c number_of_nodes                       5824621
c number_of_binary_nodes                2912310
c number_of_1-reductions                74713798
c number_of_pure_literals               7371
c reading-and-set-up_time(sec)          0.002
   \endverbatim
   </li>
   <li> Optimising WEIGHT_2 (alone):
   \verbatim
TawSolver> ./RunWeights WEIGHT_2 4 6 0.1 VanDerWaerden_2-3-13_160.cnf ""
> E1[E1$nds==min(E1$nds),]
     x  rn   rc mcl     t sat     nds    bnds       r1 pls ptime
18 5.7 160 7308  13 87.11   0 5633957 2816978 71929506   0 0.002
> E2[E2$nds==min(E2$nds),]
     x  rn   rc mcl      t sat     nds    bnds       r1  pls ptime
12 5.1 160 7308  13 144.86   0 5819401 2909700 74659091 7424 0.003
   \endverbatim
   </li>
   <li> Optimising WEIGHT_4 (alone):
   \verbatim
TawSolver> ./RunWeights WEIGHT_4 0.2 0.5 0.01 VanDerWaerden_2-3-13_160.cnf ""
> E1[E1$nds==min(E1$nds),]
      x  rn   rc mcl     t sat     nds    bnds       r1 pls ptime
13 0.32 160 7308  13 90.13   0 5637911 2818955 71955323   0 0.003
> E2[E2$nds==min(E2$nds),]
      x  rn   rc mcl      t sat     nds    bnds       r1  pls ptime
14 0.33 160 7308  13 154.98   0 5822929 2911464 74662535 7316 0.003
   \endverbatim
   </li>
   <li> Optimising WEIGHT_5 (alone):
   \verbatim
TawSolver> ./RunWeights WEIGHT_5 0.08 0.15 0.002 VanDerWaerden_2-3-13_160.cnf ""
> E1[E1$nds==min(E1$nds),]
       x  rn   rc mcl     t sat     nds    bnds       r1 pls ptime
34 0.146 160 7308  13 90.03   0 5619049 2809524 71690988   0 0.002
> E2[E2$nds==min(E2$nds),]
       x  rn   rc mcl      t sat     nds    bnds       r1  pls ptime
14 0.106 160 7308  13 157.76   0 5821811 2910905 74693913 7438 0.002
TawSolver> ./RunWeights WEIGHT_5 0.08 0.19 0.001 VanDerWaerden_2-3-13_160.cnf ""
XXX
   \endverbatim
   </li>
   <li> Now considering VanDerWaerden_pd_2-3-21_405.cnf Currently we have
   \verbatim
s UNSATISFIABLE
c number_of_variables                   203
c number_of_clauses                     21950
c maximal_clause_length                 21
c number_of_literal_occurrences         96305
c running_time(sec)                     101.82
c number_of_nodes                       2239371
c number_of_binary_nodes                1119685
c number_of_1-reductions                32017061
c reading-and-set-up_time(sec)          0.009
c file_name                             VanDerWaerden_pd_2-3-21_405.cnf

c running_time(sec)                     136.52
c number_of_nodes                       2124123
c number_of_binary_nodes                1062061
c number_of_1-reductions                31939538
c number_of_pure_literals               127
c reading-and-set-up_time(sec)          0.009
   \endverbatim
   </li>
   <li> Optimising WEIGHTs for ttawSolver (basically ignoring E1):
   \verbatim
TawSolver> ./RunWeights WEIGHT_2 4 6 0.1 VanDerWaerden_pd_2-3-21_405.cnf ""
> E1[E1$nds==min(E1$nds),]
     x  rn    rc mcl     t sat     nds    bnds       r1 pls ptime
19 5.8 203 21950  21 96.35   0 2204753 1102376 31621828   0 0.009
> E2[E2$nds==min(E2$nds),]
     x  rn    rc mcl      t sat     nds    bnds       r1 pls ptime
16 5.5 203 21950  21 122.86   0 2109955 1054977 31725127 119 0.009

TawSolver> ./RunWeights WEIGHT_4 0.2 0.4 0.01 VanDerWaerden_pd_2-3-21_405.cnf "-DWEIGHT_2=5.5"
> E1[E1$nds==min(E1$nds),]
     x  rn    rc mcl     t sat     nds    bnds       r1 pls ptime
2 0.21 203 21950  21 96.09   0 2204367 1102183 31825416   0 0.011
> E2[E2$nds==min(E2$nds),]
      x  rn    rc mcl      t sat     nds    bnds       r1 pls ptime
12 0.31 203 21950  21 121.91   0 2109419 1054709 31684244 121  0.01

TawSolver> ./RunWeights WEIGHT_5 0.1 0.15 0.002 VanDerWaerden_pd_2-3-21_405.cnf "-DWEIGHT_2=5.5 -DWEIGHT_4=0.31"
> E1[E1$nds==min(E1$nds),]
      x  rn    rc mcl     t sat     nds    bnds       r1 pls ptime
5 0.108 203 21950  21 92.94   0 2206953 1103476 31704707   0 0.009
> E2[E2$nds==min(E2$nds),]
       x  rn    rc mcl     t sat     nds    bnds       r1 pls ptime
12 0.122 203 21950  21 117.3   0 2109419 1054709 31684244 121  0.01

TawSolver> ./RunWeights WEIGHT_6 0.05 0.09 0.001 VanDerWaerden_pd_2-3-21_405.cnf "-DWEIGHT_2=5.5 -DWEIGHT_4=0.31 -DWEIGHT_5=0.122"
> E1[E1$nds==min(E1$nds),]
       x  rn    rc mcl     t sat     nds    bnds       r1 pls ptime
15 0.064 203 21950  21 89.23   0 2200497 1100248 31525253   0 0.015
> E2[E2$nds==min(E2$nds),]
      x  rn    rc mcl      t sat     nds    bnds       r1 pls ptime
7 0.056 203 21950  21 111.81   0 2108175 1054087 31297261 139  0.01
TawSolver> ./RunWeights WEIGHT_6 0.03 0.06 0.0005 VanDerWaerden_pd_2-3-21_405.cnf "-DWEIGHT_2=5.5 -DWEIGHT_4=0.31 -DWEIGHT_5=0.122"
> E1[E1$nds==min(E1$nds),]
       x  rn    rc mcl     t sat     nds    bnds       r1 pls ptime
55 0.057 203 21950  21 89.83   0 2206717 1103358 31532243   0 0.013
> E2[E2$nds==min(E2$nds),]
        x  rn    rc mcl      t sat     nds    bnds       r1 pls ptime
32 0.0455 203 21950  21 111.06   0 2093661 1046830 30867447 154 0.011

TawSolver> ./RunWeights WEIGHT_BASIS_OPEN 1.2 2.0 0.01 VanDerWaerden_pd_2-3-21_405.cnf "-DWEIGHT_2=5.5 -DWEIGHT_4=0.31 -DWEIGHT_5=0.122 -DWEIGHT_6=0.0455"
> E1[E1$nds==min(E1$nds),]
      x  rn    rc mcl     t sat     nds    bnds       r1 pls ptime
23 1.42 203 21950  21 87.84   0 2154305 1077152 30949526   0 0.015
                              file
23 VanDerWaerden_pd_2-3-21_405.cnf
> E2[E2$nds==min(E2$nds),]
     x  rn    rc mcl      t sat     nds    bnds       r1 pls ptime
41 1.6 203 21950  21 110.35   0 2093661 1046830 30867447 154  0.01
   \endverbatim
   So the result of this first run is 5.5, 0.31, 0.122, 0.0455, 1.6.
   </li>
   <li> Let's use that as a first attempt at improved weights for the
   tau-heuristics. </li>
   <li> Second round of optimisations:
   \verbatim
TawSolver> ./RunWeights TWEIGHT_2 5 6 0.01 VanDerWaerden_pd_2-3-21_405.cnf "" ttawSolver
> E[E$nds==min(E$nds),]
     x  rn    rc mcl      t sat     nds    bnds       r1 pls ptime
91 5.9 203 21950  21 117.73   0 2091279 1045639 30791742 134  0.01
   \endverbatim
   No clear minimum.
   </li>
   <li> Perhaps the palindromic instances are not good for optimising the
   general constants, since they are already "too special" ? </li>
   <li> Thus now optimising the tau-weights for VanDerWaerden_2-3-13_160.cnf.
   Currently we have:
   \verbatim
s UNSATISFIABLE
c number_of_variables                   160
c number_of_clauses                     7308
c maximal_clause_length                 13
c number_of_literal_occurrences         31804
c running_time(sec)                     146.12
c number_of_nodes                       5862905
c number_of_binary_nodes                2931452
c number_of_1-reductions                75076427
c number_of_pure_literals               7717
c reading-and-set-up_time(sec)          0.003
c file_name                             VanDerWaerden_2-3-13_160.cnf
   \endverbatim
   </li>
   <li> Complete round of optimisations:
   \verbatim
TawSolver> ./RunWeights TWEIGHT_2 4 7 0.05 VanDerWaerden_2-3-13_160.cnf "" ttawSolver
> E[E$nds==min(E$nds),]
      x  rn   rc mcl      t sat     nds    bnds       r1  pls ptime
24 5.15 160 7308  13 144.51   0 5851899 2925949 74933775 7568 0.001

TawSolver> ./RunWeights TWEIGHT_4 0.2 0.4 0.005 VanDerWaerden_2-3-13_160.cnf "-DTWEIGHT_2=5.15" ttawSolver
> E[E$nds==min(E$nds),]
       x  rn   rc mcl      t sat     nds    bnds       r1  pls ptime
22 0.305 160 7308  13 144.65   0 5851469 2925734 74929246 7574 0.003

TawSolver> ./RunWeights TWEIGHT_5 0.1 0.15 0.001 VanDerWaerden_2-3-13_160.cnf "-DTWEIGHT_2=5.15 -DTWEIGHT_4=0.305" ttawSolver
> E[E$nds==min(E$nds),]
       x  rn   rc mcl      t sat     nds    bnds       r1  pls ptime
18 0.117 160 7308  13 144.74   0 5851209 2925604 74917730 7585 0.003

TawSolver> ./RunWeights TWEIGHT_6 0.03 0.08 0.001 VanDerWaerden_2-3-13_160.cnf "-DTWEIGHT_2=5.15 -DTWEIGHT_4=0.305 -DTWEIGHT_5=0.117" ttawSolver
> E[E$nds==min(E$nds),]
       x  rn   rc mcl      t sat     nds    bnds       r1  pls ptime
27 0.056 160 7308  13 139.33   0 5776755 2888377 73878997 7414 0.007

TawSolver> ./RunWeights TWEIGHT_BASIS_OPEN 1.3 1.9 0.01 VanDerWaerden_2-3-13_160.cnf "-DTWEIGHT_2=5.15 -DTWEIGHT_4=0.305 -DTWEIGHT_5=0.117 -DTWEIGHT_6=0.056" ttawSolver
> E[E$nds==min(E$nds),]
     x  rn   rc mcl      t sat     nds    bnds       r1  pls ptime
31 1.6 160 7308  13 138.69   0 5776755 2888377 73878997 7414 0.006
XXX
   \endverbatim
   </li>
  </ul>


  \todo Considering XOR-instances
  <ul>
   <li> On the instances TwoXORclauses-20.cnf etc. (see
   Experimentation/Investigations/BooleanFunctions/plans/Hardness/data/TwoXORclauses/Tawsolver )
   the optimisation-options behave differently, and also UCP-strategy 0 is now
   faster. </li>
  </ul>


  \todo DONE Very slow file-reading for inputs with large number of variables
  <ul>
   <li> It seems the problem here is
   \verbatim
  literal_table.assign(n_vars+1,0_l);
   \endverbatim
   in function read_a_clause_from_file. </li>
   <li> Instead one should use the counter n_clauses for the number of clauses,
   s.t. for a literal x +n_clauses means the literal was encountered
   positively, -n_clauses the literal was encountered negatively, and
   everything else means the literal was not yet encountered. </li>
   <li> For that to work, the value n_clauses=0 needs to be avoided; best
   done by using n_clauses+1. </li>
   <li> So a signed integral type is needed, which can include Count_clauses.
   This might be problematic, since Count_clauses is already a 64-bit-type.
   </li>
   <li> However in reality, a signed 64-bit-type is definitely enough!
   So we just use one, and perform a cast. </li>
   <li> Indeed we can not use n_clauses, since it counts only registered
   clauses, but we need a round-counter for every encountered clause. </li>
  </ul>


  \todo Using the tau-function : DONE
  <ul>
   <li> It is interesting to replace the product as projection by the
   tau-function. </li>
   <li> For the OKsolver this replacement did not reduce the node-count, but
   here it might be different? </li>
   <li> A simple implementation (returning 1/tau(a,b), so that maximisation can
   still be used):
   \verbatim
inline Weight_t tau(const Weight_t a, const Weight_t b) {
  if (a == 0 or b == 0) return 0;
  assert(a > 0); assert(b > 0);
  constexpr Weight_t eps = 1e-12;
  Weight_t x = std::pow(4,1/(a+b));
  while (true) {
    const Weight_t pa = std::pow(x,-a), pb = std::pow(x, -b);
    const Weight_t dx = x * (pa + pb - 1) / (a*pa + b*pb);
    if (dx <= eps) return 1 / x;
    x += dx;
  }
}
   \endverbatim
   resp.
   \verbatim
inline Weight_t tau(const Weight_t a, const Weight_t b) {
  if (a == 0 or b == 0) return 0;
  assert(a > 0); assert(b > 0);
  constexpr int iterations = 5;
  Weight_t x = std::pow(4,1/(a+b));
  for (int i = 0; i < iterations; ++i) {
    const Weight_t pa = std::pow(x,-a), pb = std::pow(x,-b);
    x += x * (pa + pb - 1) / (a*pa + b*pb);
  }
  return 1/x;
}
   \endverbatim
   </li>
   <li> On csltok (2.7 GHz) we get:
   \verbatim
# With termination-condition "dx < 10^-12":

> ./tawSolver VanDerWaerden_2-3-12_135.cnf
s UNSATISFIABLE
c number_of_variables                   135
c number_of_clauses                     5251
c maximal_clause_length                 12
c number_of_literal_occurrences         22611
c running_time(sec)                     57.14
c number_of_nodes                       953175
c number_of_binary_nodes                476587
c number_of_1-reductions                11286082
c file_name                             VanDerWaerden_2-3-12_135.cnf

> ./tawSolver VanDerWaerden_2-3-13_160.cnf
s UNSATISFIABLE
c number_of_variables                   160
c number_of_clauses                     7308
c maximal_clause_length                 13
c number_of_literal_occurrences         31804
c running_time(sec)                     414.03
c number_of_nodes                       5868909
c number_of_binary_nodes                2934454
c number_of_1-reductions                75157685
c file_name                             VanDerWaerden_2-3-13_160.cnf

> ./tawSolver VanDerWaerden_pd_2-3-21_405.cnf
s UNSATISFIABLE
c number_of_variables                   203
c number_of_clauses                     21950
c maximal_clause_length                 21
c number_of_literal_occurrences         96305
c running_time(sec)                     290.98
c number_of_nodes                       2178341
c number_of_binary_nodes                1089170
c number_of_1-reductions                32404324
c file_name                             VanDerWaerden_pd_2-3-21_405.cnf

# With 5 iterations:

s UNSATISFIABLE
c number_of_variables                   135
c number_of_clauses                     5251
c maximal_clause_length                 12
c number_of_literal_occurrences         22611
c running_time(sec)                     49.13
c number_of_nodes                       953175
c number_of_binary_nodes                476587
c number_of_1-reductions                11286082
c file_name                             VanDerWaerden_2-3-12_135.cnf

> ./tawSolver VanDerWaerden_2-3-13_160.cnf
0.618034
s UNSATISFIABLE
c number_of_variables                   160
c number_of_clauses                     7308
c maximal_clause_length                 13
c number_of_literal_occurrences         31804
c running_time(sec)                     354.67
c number_of_nodes                       5868893
c number_of_binary_nodes                2934446
c number_of_1-reductions                75158028
c file_name                             VanDerWaerden_2-3-13_160.cnf

> ./tawSolver VanDerWaerden_pd_2-3-21_405.cnf
s UNSATISFIABLE
c number_of_variables                   203
c number_of_clauses                     21950
c maximal_clause_length                 21
c number_of_literal_occurrences         96305
c running_time(sec)                     251.19
c number_of_nodes                       2229093
c number_of_binary_nodes                1114546
c number_of_1-reductions                33402234
c file_name                             VanDerWaerden_pd_2-3-21_405.cnf
   \endverbatim
   </li>
   <li> 5 Iterations might be sufficient; interestingly, the more iterations
   the smaller the node-count (with diminishing return). </li>
   <li> A reduction in node-count (10% for ordinary, and 20% for the
   palindromic problem) is achieved, but, as expected, a big increase in time. </li>
   <li> One needed to re-run the optimisation of the weights. </li>
  </ul>


  \todo Handle signals : DONE
  <ul>
   <li> As the OKsolver, when interrupted, then the current statistics should
   be printed (before exit). </li>
   <li> And SIGUSR1 should result in just printing the statistics (while
   continuing the computation). </li>
  </ul>


  \todo Introduce macros : DONE
  <ul>
   <li> DONE (no longer needed)
   MAX_NUMBER_CLAUSES, MAX_NUMBER_VARIABLES. </li>
   <li> And GIT_ID. </li>
   <li> DONE (different design)
   And CLAUSE_CONTENT (default "unsigned int"). </li>
   <li> DONE (not needed)
   Then a static_assert is needed, that the type for CLAUSE_CONTENT
   is unsigned integral. </li>
  </ul>


  \todo DONE
  Check number of variables and clauses
  <ul>
   <li> Errors must happen if there are too many variables or clauses. </li>
  </ul>


  \todo Version information : DONE
  <ul>
   <li> DONE
   Like the OKsolver, when using command-line argument "-v", the solver
   should output version information and date of compilation. </li>
   <li> DONE
   Let's also handle "--version". </li>
   <li> Also the value of MAX_CLAUSE_LENGTH, LIT_TYPE and GIT_ID. </li>
   <li> DONE
   And the git-ID (this needs to be added for the OKsolver as well). </li>
   <li> There should be an official version of the tawSolver:
    <ol>
     <li> The one on the solver's homepage is 1.0.0. </li>
     <li> DONE (using 1.3 now)
     Let's call the current version 1.1 (improved handling of maximal
     clause-length, improved output, code improvements). </li>
    </ol>
   </li>
  </ul>

*/

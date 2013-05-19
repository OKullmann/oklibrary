// Oliver Kullmann, 10.4.2009 (Swansea)
/* Copyright 2009, 2010, 2011, 2013 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/3-k/general.hpp
  \brief On investigations into vdW-numbers vdw_2(3,k)

  Instances created by output_vanderwaerden2nd_stdname(3,k,n) at Maxima-level,
  or by "VanderWaerden-O3-DNDEBUG 3 k n" at C++ level (instances differ by
  the order of negated literals in clauses).
  An ubcsat-experiment is run by "RunVdW3k k n0 alg runs cutoff".

  The palindromic versions are created by
  output_pd_vanderwaerden2nd_stdname(3,k,n) at Maxima-level, or by
  "PdVanderWaerdenCNF-O3-DNDEBUG 3 k n" at C++ level (instances differ by
  the order of negated literals in clauses).
  An ubcsat-experiment is run by "RunPdVdWk1k2 3 k n0 alg runs cutoff".
  And running a complete solver by "CRunPdVdWk1k2 3 k solver".


  \todo Connections
  <ul>
   <li> This is sequence A007783 at OEIS https://oeis.org/A007783 . </li>
  </ul>


  \todo Predictions of vdw_2(3,k)
  <ul>
   <li> A conjecture is vdw_2(3,k) <= k^2 for k >= 3.
    <ol>
     <li> The first counterexample is vdw_2(3,24) > 592. </li>
     <li> It seems that this is not just an outlier, but that growth is
     more than quadratic. </li>
    </ol>
   </li>
   <li> The known values with k=1, ..., 19 are available via
   vanderwaerden3k(k):
   \verbatim
create_list(vanderwaerden3k(k),k,1,19);
 [3,6,9,18,22,32,46,58,77,97,114,135,160,186,218,238,279,312,349]
   \endverbatim
   </li>
   <li> Log-log regression (in R) does not yield a good model, as can be
   seen inspecting the plot:
   \verbatim
d0 = c(3,6,9,18,22,32,46,58,77,97,114,135,160,186,218,238,279,312,349)
plot(d0)
lines(d0)
x0 = log((1:length(d0)))
y0 = log(d0)
plot(x0,y0)
L0 = lm(y0 ~ x0)
summary(L0)

Coefficients:
            Estimate Std. Error t value Pr(>|t|)
(Intercept)  0.58155    0.12319   4.721 0.000197 ***
x0           1.73669    0.05563  31.220  < 2e-16 ***
Residual standard error: 0.1906 on 17 degrees of freedom
Multiple R-squared: 0.9829,     Adjusted R-squared: 0.9818

lines(x0,predict(L0))
   \endverbatim
   </li>
   <li> Using the conjectured values 389,416,464,516,593,656,727,770,827,
   868 (see below):
   \verbatim
d = append(d0, c(389,416,464,516,593,656,727,770,827,868))
plot(d)
lines(d)
lines((1:length(d))^2)
x = log((1:length(d)))
y = log(d)
L = lm(y ~ x)
plot(x,y)
lines(x,predict(L))
   \endverbatim
   Again this doesn't seem to be a good model (larger k-values are not taken
   enough into account).
   </li>
   <li> Non-linear regression yields a better model:
   \verbatim
X = (1:length(d))
Y = d
plot(X,Y)
NL = nls(Y ~ a*(X^b), start = c(a = 0.8, b = 2))
lines(predict(NL))
summary(NL)

Parameters:
  Estimate Std. Error t value Pr(>|t|)
a  0.58471    0.05241   11.16 1.29e-11 ***
b  2.17488    0.02799   77.70  < 2e-16 ***
Residual standard error: 11.28 on 27 degrees of freedom

Cnl = coefficients(NL)
Cnl
        a         b
0.5847057 2.1748818
fnl = function(k){Cnl[1] * k^Cnl[2]}
round(fnl(1:35))
 1 3 6 12 19 29 40 54 70 87
 108 130 155 182 211 243 277 314 353 395
 439 486 535 587 642 699 759 821 886 954
 1024 1098 1174 1252 1334

d - round(fnl(1:29))
 2 3 3 6 3 3 6 4 7 10
 6 5 5 4 7 -5 2 -2 -4 -6
 -23 -22 -19 6 14 28 11 6 -18
   \endverbatim
   </li>
   <li> Finally using a quadratic model with non-linear regression and
   all data:
   \verbatim
NLq = nls(d ~ a + b*X + c * X^2, start = c(a=0, b = 0, c = 1))
lines(predict(NLq))
summary(NLq)

Parameters:
  Estimate Std. Error t value Pr(>|t|)
a 17.38095    7.21230   2.410   0.0233 *
b -5.09519    1.10817  -4.598 9.72e-05 ***
c  1.20367    0.03584  33.580  < 2e-16 ***
Residual standard error: 12.06 on 26 degrees of freedom

Cnlq = coefficients(NLq)
Cnlq
        a         b         c
17.380952 -5.095185  1.203665
fnlq = function(k){Cnlq[1] + Cnlq[2]*k +Cnlq[3] * k^2}
round(fnlq(1:35))
 13 12 13 16 22 30 41 54 69 87
 107 130 155 182 212 244 279 316 355 397
 441 488 537 588 642 699 757 818 882 948
 1016 1087 1160 1236 1314

d - round(fnlq(X))
 -10 -6 -4 2 0 2 5 4 8 10
 7 5 5 4 6 -6 0 -4 -6 -8
 -25 -24 -21 5 14 28 13 9 -14
   \endverbatim
   Hard to distinguish from the above model. </li>
   <li> Refined models:
    <ol>
     <li> It seems that vdw_2(3,30) is quite below the predicted value. </li>
     <li> Looking at the plots, there seems to be an oscillation around some
     core curve, and with k=29 it seems a down-phase (with negative
     differences in the above "d - predicted" tables) is entered. </li>
     <li> How to model that? </li>
    </ol>
   </li>
  </ul>


  \todo Analysing certificates
  <ul>
   <li> See "Analysing certificates" in
   ComputerAlgebra/RamseyTheory/Lisp/VanderWaerden/plans/Certificates.hpp.
   </li>
   <li> It seems that solutions at least for the harder problems come in
   small clusters, where the clusters have large Hamming distances, while
   within the clusters we have rather small Hamming distances. </li>
  </ul>


  \todo Performance of OKsolver-2002
  <ul>
   <li> k=4, n=17,18: 7 resp. 9 nodes. </li>
   <li> k=5, n=21,22: 6 resp. 19 nodes. </li>
   <li> k=6, n=31,32: 24 resp. 51 nodes. </li>
   <li> k=7, n=45,46: 11 resp. 139 nodes. </li>
   <li> k=8, n=57,58: 109 resp. 479 nodes. </li>
   <li> k=9, n=76,77: 702 resp. 1591 nodes. </li>
   <li> k=10, n=96,97: 129 resp. 10545 nodes. </li>
   <li> k=11, n=113,114:
    <ol>
     <li> 37374 resp. 41563 nodes. </li>
     <li> With minisat2-preprocessor: identical results (so apparently
     the preprocessor does nothing). </li>
    </ol>
   </li>
   <li> k=12, n=134, 135 (cswsok, 3GHz):
   \verbatim
> OKsolver_2002-O3-DNDEBUG VanDerWaerden_2-3-12_134.cnf
s SATISFIABLE
c sat_status                            1
c initial_maximal_clause_length         12
c initial_number_of_variables           134
c initial_number_of_clauses             5172
c initial_number_of_literal_occurrences 22266
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   0
c running_time(sec)                     222.1
c number_of_nodes                       283568
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                2060055
c number_of_pure_literals               35
c number_of_autarkies                   1
c number_of_missed_single_nodes         0
c max_tree_depth                        36
c proportion_searched                   9.996262e-01
c proportion_single                     0.000000e+00
c total_proportion                      0.9996261596679688
c number_of_table_enlargements          0
c number_of_1-autarkies                 516
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             VanDerWaerden_2-3-12_134.cnf

# without tree-pruning: precisely the same.

> OKsolver_2002-O3-DNDEBUG VanDerWaerden_2-3-12_135.cnf
s UNSATISFIABLE
c sat_status                            0
c initial_maximal_clause_length         12
c initial_number_of_variables           135
c initial_number_of_clauses             5251
c initial_number_of_literal_occurrences 22611
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   0
c running_time(sec)                     220.8
c number_of_nodes                       281381
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                2049274
c number_of_pure_literals               29
c number_of_autarkies                   0
c number_of_missed_single_nodes         0
c max_tree_depth                        36
c proportion_searched                   1.000000e+00
c proportion_single                     0.000000e+00
c total_proportion                      1
c number_of_table_enlargements          0
c number_of_1-autarkies                 490
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             VanDerWaerden_2-3-12_135.cnf

same machine csltok (Intel i5, 2.4GHz), but only with 1-2 processes altogether:
306.0s

   \endverbatim
   </li>
   <li> k=13, n=159, 160:
   \verbatim
On new csltok (64bit):
> OKsolver_2002-O3-DNDEBUG VanDerWaerden_2-3-13_159.cnf
s SATISFIABLE
c sat_status                            1
c initial_maximal_clause_length         13
c initial_number_of_variables           159
c initial_number_of_clauses             7216
c initial_number_of_literal_occurrences 31398
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   0
c running_time(sec)                     6835.3
c number_of_nodes                       1590139
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                12785699
c number_of_pure_literals               455
c number_of_autarkies                   0
c number_of_missed_single_nodes         0
c max_tree_depth                        43
c number_of_table_enlargements          0
c number_of_1-autarkies                 12836
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             VanDerWaerden_2-3-13_159.cnf

same machine csltok (Intel i5, 2.4GHz), but only with 1-2 processes altogether:
3224.7s

> OKsolver_2002-O3-DNDEBUG VanDerWaerden_2-3-13_160.cnf
s UNSATISFIABLE
c sat_status                            0
c initial_maximal_clause_length         13
c initial_number_of_variables           160
c initial_number_of_clauses             7308
c initial_number_of_literal_occurrences 31804
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   0
c running_time(sec)                     14095.2
c number_of_nodes                       2970723
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                24138237
c number_of_pure_literals               706
c number_of_autarkies                   0
c number_of_missed_single_nodes         0
c max_tree_depth                        43
c number_of_table_enlargements          0
c number_of_1-autarkies                 24075
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             VanDerWaerden_2-3-13_160.cnf

same machine csltok (Intel i5, 2.4GHz), but only with 1-2 processes altogether:
5372s

On old csltok (32bit):
s SATISFIABLE
c sat_status=1 initial_maximal_clause_length=13 initial_number_of_variables=159 initial_number_of_clauses=7216 initial_number_of_literal_occurrences=31398 running_time(s)=4187.7 number_of_nodes=1590137 number_of_single_nodes=0 number_of_quasi_single_nodes=0 number_of_2-reductions=13477703 number_of_pure_literals=455 number_of_autarkies=0 number_of_missed_single_nodes=1 max_tree_depth=43 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=0 reduced_number_of_clauses=0 reduced_number_of_literal_occurrences=0 number_of_1-autarkies=12836 number_of_initial_unit-eliminations=0 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=0 file_name=VanDerWaerden_2-3-13_159.cnf
s UNSATISFIABLE
c sat_status=0 initial_maximal_clause_length=13 initial_number_of_variables=160 initial_number_of_clauses=7308 initial_number_of_literal_occurrences=31804 running_time(s)=7514.8 number_of_nodes=2970721 number_of_single_nodes=0 number_of_quasi_single_nodes=0 number_of_2-reductions=25434723 number_of_pure_literals=706 number_of_autarkies=0 number_of_missed_single_nodes=0 max_tree_depth=43 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=0 reduced_number_of_clauses=0 reduced_number_of_literal_occurrences=0 number_of_1-autarkies=24074 number_of_initial_unit-eliminations=0 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=0 file_name=VanDerWaerden_2-3-13_160.cnf
   \endverbatim
   </li>
  </ul>


  \todo Performance of march_pl
  <ul>
   <li> k=12:
    <ol>
     <li> k=12, n=134: 73s (5567 nodes, csltok) </li>
     <li> k=12, n=135: 550s (47963 nodes, csltok) </li>
    </ol>
   </li>
   <li> k=13, n=160:
   \verbatim
c main():: nodeCount: 279674
c main():: dead ends in main: 531
c main():: lookAheadCount: 71792373
c main():: unitResolveCount: 1830566
c main():: time=5111.450195
   \endverbatim
   </li>
  </ul>


  \todo Performance of satz215
  <ul>
   <li> k=12:
    <ol>
     <li> k=12, n=134: 73s (NB_BRANCHE= 240158, cswsok (3 GHz)) </li>
     <li> k=12, n=135: 79s (NB_BRANCHE= 262304, cswsok (3 GHz)) </li>
    </ol>
   </li>
   <li> k=13, n=160 (cswsok, 3GHz):
   \verbatim
> satz215 VanDerWaerden_2-3-13_160.cnf
**** The instance is unsatisfiable. *****
NB_MONO= 363, NB_UNIT= 39344343, NB_BRANCHE= 1698185, NB_BACK= 863252
Program terminated in 711.470 seconds.
satz215 VanDerWaerden_2-3-13_160.cnf 711.470 1698185 863252 219047856 8567070 0 160 7308 0 4263617 1867228
   \endverbatim
   </li>
   <li> k=14, n=186 (cswsok, 3GHz):
   \verbatim
> satz215 VanDerWaerden_2-3-14_186.cnf
NB_MONO= 1435, NB_UNIT= 277867725, NB_BRANCHE= 10822316, NB_BACK= 5500793
Program terminated in 6233.370 seconds.
satz215 VanDerWaerden_2-3-14_186.cnf 6233.370 10822316 5500793 1573930613 58462301 0 186 9795 0 29371201 12582731
   \endverbatim
   </li>
   <li> k=15, n=218 (cswsok, 3GHz):
   \verbatim
> satz215 VanDerWaerden_2-3-15_218.cnf
NB_MONO= 3848, NB_UNIT= 1902278418, NB_BRANCHE= 66595028, NB_BACK= 33775013
Program terminated in 54913.090 seconds.
satz215 VanDerWaerden_2-3-15_218.cnf 54913.090 66595028 33775013 11030537203 387523942 0 218 13362 0 314567493 92988051
   \endverbatim
   (that's 15.3 hours).
   </li>
   <li> k=16, n=238 (cswsok, 3GHz):
   \verbatim
> satz215 VanDerWaerden_2-3-16_238.cnf
XXX
   \endverbatim
   (that's XXX hours).
   </li>

   <li> SplittingViaOKsolver:
   \verbatim
> solver="satz215" ProcessSplitViaOKsolver SplitViaOKsolver_D30VanDerWaerden_2315_218cnf_2013-05-17-015710
# aborted after 2071 from 12922 sub-instances:
> E=read_processsplit_satz()
2071: 21.955h, sum-nds=1.363216e+08, mean-t=38.165s, mean-nds=65824, sat: 0
$t:
    Min.  1st Qu.   Median     Mean  3rd Qu.     Max.
   0.120    0.890    1.420   38.160    2.305 6503.000
sd= 339.4337
     95%      96%      97%      98%      99%     100%
   4.845    5.630    6.979   39.830 1316.095 6503.010
sum= 79038.94
$nds:
    Min.  1st Qu.   Median     Mean  3rd Qu.     Max.
      34     1438     2465    65820     4323 11620000
sd= 586093.9
       95%        96%        97%        98%        99%       100%
    9509.0    11161.4    14417.8    83698.2  2248985.2 11621345.0
sum= 136321627
$t ~ $nds:
              Estimate Std. Error   t value Pr(>|t|)
(Intercept) 6.2299e-02 2.3939e-01    0.2602   0.7947
E$nds       5.7885e-04 4.0600e-07 1425.7567   <2e-16 ***
R-squared: 0.999
   \endverbatim
   That is cleary worse than just plain satz215. </li>
  </ul>


  \todo Performance of conflict-driven solvers
  <ul>
   <li> minisat2:
    <ol>
     <li> k=12, n=134: 1387192 conflicts, 22 restarts (csltok; 598s). </li>
     <li> k=12, n=135: 1454696 conflicts, 22 restarts (csltok; 608s). </li>
     <li> k=13, n=159: 525039 conflicts,  20 restarts (csltok; 263s). </li>
     <li> k=13, n=160: 9298288 conflicts, 27 restarts (csltok; 7019s). </li>
    </ol>
   </li>
   <li> minisat-2.2.0:
    <ol>
     <li> k=12, n=134: 61s (3605914 conflicts; cswsok (3GHz)) </li>
     <li> k=12, n=135: 107s (5963349 conflicts; cswsok (3GHz)) </li>
     <li> k=13, n=159: 13s (701558 conflicts; cswsok (3GHz)) </li>
     <li> k=13, n=160: 1716s (63901998 conflicts; cswsok (3GHz)) </li>
     <li> k=14, n=185: 147s (5619881 conflicts; cswsok (3GHz)) </li>
     <li> k=14, n=186: 16836s (463984635 conflicts; cswsok (3GHz)) </li>
     <li> k=15, n=217: 26319s (500756444 conflicts; cswsok (3GHz)) </li>
     <li> k=15, n=218: 190211s (3205639994 conflicts; cswsok (3GHz)) </li>
     <li> SplittingViaOKsolver (cswsok, 3 GHz):
     \verbatim
> SplittingViaOKsolver -D20 VanDerWaerden_2-3-13_160.cnf
> cd SplitViaOKsolver_D20VanDerWaerden_2313_160cnf_2013-05-17-001649
> more Md5sum
16c1083271af4be87d875298d869a6c7
> more Statistics
> summary(E$n)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  20.00   20.00   23.00   23.23   25.00   37.00
> table(E$n)
 20  21  22  23  24  25  26  27  28  29  30  31  32  34  37
409 111 125 196 233 201 151  96  52  12   2   2   4   2   3
> summary(E$d)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
   5.00   12.00   13.00   13.21   15.00   18.00
> table(E$d)
  5   6   7   8   9  10  11  12  13  14  15  16  17  18
  3  10  28  40  61  86 138 193 264 240 217 195 112  12
> more Result
s UNKNOWN
c sat_status                            2
c initial_maximal_clause_length         13
c initial_number_of_variables           160
c initial_number_of_clauses             7308
c initial_number_of_literal_occurrences 31804
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   0
c running_time(sec)                     9.7
c number_of_nodes                       3197
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                110
c number_of_pure_literals               0
c number_of_autarkies                   0
c number_of_missed_single_nodes         0
c max_tree_depth                        18
c proportion_searched                   0.000000e+00
c proportion_single                     0.000000e+00
c total_proportion                      0
c number_of_table_enlargements          0
c number_of_1-autarkies                 0
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             VanDerWaerden_2-3-13_160.cnf
c splitting_directory                   SplitViaOKsolver_D20VanDerWaerden_2313_160cnf_2013-05-17-001649/Insta
nces
c splitting_cases                       1599

> cd ..
> ProcessSplitViaOKsolver SplitViaOKsolver_D20VanDerWaerden_2313_160cnf_2013-05-17-001649
> cat Process_SplitViaOKsolver_D20VanDerWaerden_2313_160cnf_2013-05-17-001649_2013-05-17-002115/Result
  2:35

> SplittingViaOKsolver -D30 VanDerWaerden_2-3-14_186.cnf
c running_time(sec)                     145.7
c splitting_cases                       13527
> ProcessSplitViaOKsolver SplitViaOKsolver_D30VanDerWaerden_2314_186cnf_2013-05-17-002903/
> cat Process_SplitViaOKsolver_D30VanDerWaerden_2314_186cnf_2013-05-17-002903_2013-05-17-003312/Result
  22:08

> SplittingViaOKsolver -D30 VanDerWaerden_2-3-15_218.cnf
c running_time(sec)                     231.8
c splitting_cases                       12922
> ProcessSplitViaOKsolver SplitViaOKsolver_D30VanDerWaerden_2315_218cnf_2013-05-17-015710
> cat Process_SplitViaOKsolver_D30VanDerWaerden_2315_218cnf_2013-05-17-015710_2013-05-17-020436/Result
  141:27
> E=read_processsplit_minisat()
12922: 2.226h, sum-cfs=3.806417e+08, mean-t=0.620s, mean-cfs=29457, sat: 0
$t:
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
 0.0150  0.2320  0.3969  0.6203  0.7576 15.5700
sd= 0.6646329
      95%       96%       97%       98%       99%      100%
 1.866720  2.026010  2.252030  2.599404  3.274080 15.569600
sum= 8015.144
$cfs:
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
    408   11500   19900   29460   36550  636800
sd= 29454.45
      95%       96%       97%       98%       99%      100%
 85033.00  92782.64 102557.00 117306.90 145427.27 636785.00
sum= 380641711
$t ~ $cfs:
               Estimate  Std. Error  t value  Pr(>|t|)
(Intercept) -4.3157e-02  5.0876e-04  -84.827 < 2.2e-16 ***
E$cfs        2.2522e-05  1.2214e-08 1844.020 < 2.2e-16 ***
R-squared: 0.9962

> SplittingViaOKsolver -D35 VanDerWaerden_2-3-15_218.cnf
c running_time(sec)                     821.3
c splitting_cases                       32331
> ProcessSplitViaOKsolver SplitViaOKsolver_D35VanDerWaerden_2315_218cnf_2013-05-17-090420
> cat Process_SplitViaOKsolver_D35VanDerWaerden_2315_218cnf_2013-05-17-090420_2013-05-17-092127/Result
  134:31
> E=read_processsplit_minisat()
32331: 1.913h, sum-cfs=3.615115e+08, mean-t=0.213s, mean-cfs=11182, sat: 0
$t:
    Min.  1st Qu.   Median     Mean  3rd Qu.     Max.
0.005999 0.089990 0.158000 0.213100 0.274000 3.906000
sd= 0.1911241
     95%      96%      97%      98%      99%     100%
0.574912 0.619905 0.676897 0.752885 0.910861 3.906410
sum= 6888.165
$cfs:
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
     41    4755    8441   11180   14400  180100
sd= 9784.68
     95%      96%      97%      98%      99%     100%
 29808.0  32137.8  34737.2  38804.8  46902.0 180053.0
sum= 361511501
$t ~ $cfs:
               Estimate  Std. Error  t value  Pr(>|t|)
(Intercept) -4.8847e-03  1.0623e-04  -45.985 < 2.2e-16 ***
E$cfs        1.9491e-05  7.1493e-09 2726.231 < 2.2e-16 ***
R-squared: 0.9957

> SplittingViaOKsolver -D40 VanDerWaerden_2-3-16_238.cnf
c running_time(sec)                     3247.6
c splitting_cases                       104797
> ProcessSplitViaOKsolver SplitViaOKsolver_D40VanDerWaerden_2316_238cnf_2013-05-17-140911
> E=read_processsplit_minisat()
104797: 22.238h, sum-cfs=3.723995e+09, mean-t=0.764s, mean-cfs=35535, sat: 0
$t:
     Min.   1st Qu.    Median      Mean   3rd Qu.      Max.
 0.009998  0.266000  0.459900  0.763900  0.853900 29.560000
sd= 0.9925851
      95%       96%       97%       98%       99%      100%
 2.414630  2.746580  3.184759  3.804420  4.983320 29.558500
sum= 80056.43
$cfs:
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
     71   12830   22290   35540   40960 1128000
sd= 42953.37
      95%       96%       97%       98%       99%      100%
 110594.8  124136.6  142799.4  167161.1  218275.0 1127634.0
sum= 3723995162
$t ~ $cfs:
               Estimate  Std. Error t value  Pr(>|t|)
(Intercept) -5.5472e-02  2.6144e-04 -212.18 < 2.2e-16 ***
E$cfs        2.3059e-05  4.6897e-09 4916.80 < 2.2e-16 ***
R-squared: 0.9957

> SplittingViaOKsolver -D50 VanDerWaerden_2-3-17_279.cnf
c running_time(sec)                     23546.1
c number_of_nodes                       1399505
c number_of_2-reductions                82906
c max_tree_depth                        41
c splitting_cases                       699751
> ProcessSplitViaOKsolver SplitViaOKsolver_D50VanDerWaerden_2317_279cnf_2013-05-18-154809
XXX
     \endverbatim
     A big speed-up! </li>
    </ol>
   </li>
   <li> cryptominisat-2.9.0:
    <ol>
     <li> k=12, n=134: 91s (619145 conflicts; csltok) </li>
     <li> k=12, n=135: 763s (2815643 conflicts; csltok) </li>
    </ol>
   </li>
   <li> cryptominisat-2.9.6:
    <ol>
     <li> k=12, n=134: 155s (1693268 conflicts; cswsok (3 GHz)) </li>
     <li> k=12, n=135: 212s (2109106 conflicts; cswsok (3 GHz)) </li>
    </ol>
   </li>
   <li> picosat913:
    <ol>
     <li> k=12, n=134: 7s (368890 conflicts; cswsok (3 GHz)) </li>
     <li> k=12, n=135: 259s (9643671 conflicts; cswsok (3 GHz)) </li>
    </ol>
   </li>
   <li> precosat236:
    <ol>
     <li> k=12, n=134: 52s (1145491 conflicts; cswsok (3 GHz)) </li>
     <li> k=12, n=135: 205s (3583785 conflicts; cswsok (3 GHz)) </li>
    </ol>
   </li>
   <li> precosat-570.1:
    <ol>
     <li> k=12, n=134: 91s (1531799 conflicts; cswsok (3 GHz)) </li>
     <li> k=12, n=135: 211s (2425722 conflicts; cswsok (3 GHz)) </li>
    </ol>
   </li>
   <li> lingelingala-b02aa1a-121013:
    <ol>
     <li> k=12, n=134: 171s (1659607 conflicts; cswsok (3 GHz)) </li>
     <li> k=12, n=135: 476s (3435610 conflicts; cswsok (3 GHz)) </li>
    </ol>
   </li>
   <li> glucose-1.0:
    <ol>
     <li> k=12, n=134: 39s (425399 conflicts; csltok) </li>
     <li> k=12, n=135: 191s (1356325 conflicts; csltok) </li>
     <li> k=13, n=159: 139s (957255 conflicts; csltok) </li>
     <li> k=13, n=160: 3274s (9907932 conflicts; csltok) </li>
    </ol>
   </li>
   <li> glucose-2.0:
    <ol>
     <li> k=12, n=134: 5s (169420 conflicts; cswsok (3 GHz)) </li>
     <li> k=12, n=135: 58s (1263087 conflicts; cswsok (3 GHz)) </li>
     <li> k=13, n=159: 1s (50528 conflicts; cswsok (3 GHz)) </li>
     <li> k=13, n=160: 781s (8377487 conflicts; cswsok (3 GHz)) </li>
     <li> k=14, n=185: 5133s (31516583 conflicts; cswsok (3 GHz)) </li>
     <li> k=14, n=186: 84334s (163500051 conflicts; cswsok (3 GHz)) </li>
     <li> SplittingViaOKsolver (cswsok, 3 GHz):
     \verbatim
> solver="glucose-2.0" ProcessSplitViaOKsolver SplitViaOKsolver_D30VanDerWaerden_2315_218cnf_2013-05-17-015710
> cat Process_SplitViaOKsolver_D30VanDerWaerden_2315_218cnf_2013-05-17-015710_2013-05-17-180553/Result
  187:11
     \endverbatim
     Reasonable fast, but minisat-2.2.0 is faster.
     </li>
    </ol>
   </li>
   <li> glucose-2.2:
    <ol>
     <li> k=12, n=134: 84s (1350204 conflicts; cswsok (3 GHz)) </li>
     <li> k=12, n=135: 94s (1444017 conflicts; cswsok (3 GHz)) </li>
     <li> k=13, n=159: 87s (1332829 conflicts; cswsok (3 GHz)) </li>
     <li> k=13, n=160: 1412s (10447051 conflicts; cswsok (3 GHz)) </li>
     <li> SplittingViaOKsolver (cswsok, 3 GHz):
     \verbatim
> solver="glucose-2.2" ProcessSplitViaOKsolver SplitViaOKsolver_D30VanDerWaerden_2315_218cnf_2013-05-17-015710
XXX
     \endverbatim
     </li>
    </ol>
    Performance much worse -- apparently due to much more restarts!
   </li>
   <li> minisat-2.2.0 and glucose-2.0 seem best (for the conflict-driven
   solvers, while satz215 seems best overall); however SplittingViaOKsolver
   with minisat-2.2 is much better than satz215. </li>
  </ul>


  \todo SAT 2011 competition
  <ul>
   <li> See
   http://www.cril.univ-artois.fr/SAT11/results/globalbybench.php?idev=46&idcat=63
   </li>
   <li> clasp 2.0-R4092-crafted performed best on ordinary vdW-instances.
   </li>
   <li> Also on palindromic instances performance was sometimes best, but also
   MPhaseSAT 2011-02-15, claspmt 2.0-R4095-patched, Coach-SAT-Portfolio
   2011-03-18, ppfolio par, clasp 1.2.0-SAT09-32, sattime+ 2011-32-02,
   sathys 2011-04-01, and glucose 2 need to be considered. </li>
  </ul>


  \todo Local search for the satisfiable instances
  <ul>
   <li> The first impression is that ubcsat::samd is best. </li>
   <li> For k=16, n=237 a local minimum of 1 is easily found (even with just
   100000 steps), while to find a solution one should use, say, 10^7 steps
   (and then solutions seem to be found often, e.g., seed=370833091). </li>
   <li> But with vanderwaerden_2(3,17) we see that adaptnovelty+
   is better; perhaps, as with vdw_2(4,k), getting stronger with higher
   cutoff. </li>
   <li> However, for vanderwaerden_2(3,18) samd again seems clearly better,
   while for 19 <= k <= 23 gsat-tabu seems best, and for k >= 24 rots
   seems best --- this needs to be investigated more thoroughly. </li>
  </ul>

*/

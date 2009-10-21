// Oliver Kullmann, 10.4.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/VanderWaerden_2-3-k.hpp
  \brief On investigations into vdW-numbers vdw_2(3,k)

  Instances created by output_vanderwaerden2nd_stdname(3,k,n).
  
  
  \todo Literature overview
  <ul>
   <li> A conjecture seems to be that vdw_2(3,k) <= k^2 for k >= 3. </li>
   <li> The known values with k=1, ..., 16 are available via
   vanderwaerden3k(k):
   \verbatim
create_list(vanderwaerden3k(k),k,1,17);
 [3,6,9,18,22,32,46,58,77,97,114,135,160,186,218,238,unknown]
   \endverbatim
   </li>
   <li> Via R we get the prediction f(k) ~ 0.8132032 * k^2.0602760 when
   excluding the first 4 points (this is motivated by looking at the graph):
   \verbatim
d0 = c(3,6,9,18,22,32,46,58,77,97,114,135,160,186,218,238)
plot(d0)
x = log((1:length(d0))[-(1:4)])
y = log(d0[-(1:4)])
plot(x,y)
L = lm(y ~ x)
summary(L)
Coefficients:
            Estimate Std. Error t value Pr(>|t|)
(Intercept) -0.20677    0.04173  -4.955 0.000574 ***
x            2.06028    0.01799 114.495  < 2e-16 ***
lines(x,predict(L))
C = coefficients(L)
exp(C[1])
  0.8132032
C[2]
  2.060276
f = function(k){exp(C[1]) * k^C[2]}
plot(d0)
lines(f(1:length(d0)))
round(f(1:20))
  1   3   8  14  22  33  45  59  75  93 114 136 160 187 215 246 279 314 351
  390
d0 - round(f(1:16))
  2  3  1  4  0 -1  1 -1  2  4  0 -1  0 -1  3 -8
   \endverbatim
   </li>
   <li> Using the conjectures 279, 312, 349 (see below):
   \verbatim
d = c(3,6,9,18,22,32,46,58,77,97,114,135,160,186,218,238,279,312,349)
x = log((1:length(d))[-(1:4)])
y = log(d[-(1:4)])
L = lm(y ~ x)
C = coefficients(L)
exp(C[1])
  0.8164498
C[2]
  2.058377
f = function(k){exp(C[1]) * k^C[2]}
round(f(1:21))
 1   3   8  14  22  33  45  59  75  93 114 136 160 187 215 246 278 313 350
 389 430
d - round(f(1:19))
2  3  1  4  0 -1  1 -1  2  4  0 -1  0 -1  3 -8  1 -1 -1
   \endverbatim
   </li>
   <li> Now using a quadratic model (using the above d):
\verbatim
X = (1:length(d))[-(1:4)]
Y = d[-(1:4)]
Lq = lm(Y ~ poly(X,2))
summary(Lq)
Residual standard error: 2.632 on 12 degrees of freedom
Multiple R-squared: 0.9995,     Adjusted R-squared: 0.9994
F-statistic: 1.119e+04 on 2 and 12 DF,  p-value: < 2.2e-16
plot(X,Y)
lines(X,predict(Lq))

Lq = lm(Y ~ X + I(X^2))
summary(Lq)
Coefficients:
            Estimate Std. Error t value Pr(>|t|)
(Intercept)  1.49457    5.51319   0.271    0.791
X           -0.76469    0.99586  -0.768    0.457
I(X^2)       0.99927    0.04097  24.388 1.36e-11 ***
Residual standard error: 2.632 on 12 degrees of freedom
Multiple R-squared: 0.9995,     Adjusted R-squared: 0.9994
F-statistic: 1.119e+04 on 2 and 12 DF,  p-value: < 2.2e-16
Cq = coefficients(Lq)
fq = function(k){Cq[1] + Cq[2]*k + Cq[3]*k^2}
round(fq(1:21))
 2   4   8  14  23  33  45  59  76  94 114 136 160 187 215 245 277 311 348
 386 426
d - round(fq(1:19))
 1  2  1  4 -1 -1  1 -1  1  3  0 -1  0 -1  3 -7  2  1  1
   \endverbatim
   (using orthogonal polynomials via poly(x,2) seems to result in more
   meaningfull coefficients, however the regression results are the same).
   </li>
   <li> Non-linear regression (using the above X, Y):
   \verbatim
NL = nls(Y ~ a*(X^b), start = c(a = 0.8164498, b = 2.058377))
> summary(NL)
Parameters:
  Estimate Std. Error t value Pr(>|t|)
a  0.84917    0.04274   19.87 4.13e-11 ***
b  2.04306    0.01810  112.85  < 2e-16 ***
Residual standard error: 2.506 on 13 degrees of freedom
Number of iterations to convergence: 2
Achieved convergence tolerance: 5.458e-06
Cnl = coefficients(NL)
Cnl
        a         b
0.8491743 2.0430615
fnl = function(k){Cnl[1] * k^Cnl[2]}
round(fnl(1:21))
 1   3   8  14  23  33  45  59  76  94 114 136 160 186 215 245 277 312 348
 386 427
d - round(fnl(1:19))
 2  3  1  4 -1 -1  1 -1  1  3  0 -1  0  0  3 -7  2  0  1
   \endverbatim
   </li>
   <li> Finally using a quadratic model with non-linear regression and
   all data:
   \verbatim
i = 1:length(d)
NLq = nls(d ~ a + b*i + c * i^2, start = c(a=0, b = 0, c = 1))
summary(NLq)
Parameters:
  Estimate Std. Error t value Pr(>|t|)
a  3.74303    1.83844   2.036   0.0587 .
b -1.11280    0.42330  -2.629   0.0182 *
c  1.01187    0.02056  49.210   <2e-16 ***
Residual standard error: 2.395 on 16 degrees of freedom
Number of iterations to convergence: 1
Achieved convergence tolerance: 1.616e-07
Cnlq = coefficients(NLq)
Cnlq
        a         b         c
 3.743034 -1.112797  1.011868
fnlq = function(k){Cnlq[1] + Cnlq[2]*k +Cnlq[3] * k^2}
round(fnlq(1:21))
 4   6  10  15  23  33  46  60  76  94 114 136 160 186 215 245 277 312 348
 386 427
d - round(fnlq(i)
 -1  0 -1  3 -1 -1  0 -2  1  3  0 -1  0  0  3 -7  2  0  1
plot(d)
lines(i,f(i))
   \endverbatim
   Perhaps this is the best approach?
   </li>
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
   <li> k=12, n=134, 135: 283568 resp. 281381 nodes (~ 7 minutes). </li>
   <li> k=13, n=159,160:
   \verbatim
s SATISFIABLE
c sat_status=1 initial_maximal_clause_length=13 initial_number_of_variables=159 initial_number_of_clauses=7216 initial_number_of_literal_occurrences=31398 running_time(s)=4187.7 number_of_nodes=1590137 number_of_single_nodes=0 number_of_quasi_single_nodes=0 number_of_2-reductions=13477703 number_of_pure_literals=455 number_of_autarkies=0 number_of_missed_single_nodes=1 max_tree_depth=43 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=0 reduced_number_of_clauses=0 reduced_number_of_literal_occurrences=0 number_of_1-autarkies=12836 number_of_initial_unit-eliminations=0 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=0 file_name=VanDerWaerden_2-3-13_159.cnf
s UNSATISFIABLE
c sat_status=0 initial_maximal_clause_length=13 initial_number_of_variables=160 initial_number_of_clauses=7308 initial_number_of_literal_occurrences=31804 running_time(s)=7514.8 number_of_nodes=2970721 number_of_single_nodes=0 number_of_quasi_single_nodes=0 number_of_2-reductions=25434723 number_of_pure_literals=706 number_of_autarkies=0 number_of_missed_single_nodes=0 max_tree_depth=43 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=0 reduced_number_of_clauses=0 reduced_number_of_literal_occurrences=0 number_of_1-autarkies=24074 number_of_initial_unit-eliminations=0 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=0 file_name=VanDerWaerden_2-3-13_160.cnf
   \endverbatim
   </li>
   <li> k=14, n=185,186:
   \verbatim
> OKsolver_2002-O3-DNDEBUG -M -D21 -F VanDerWaerden_2-3-14_185.cnf
  1029:   1111   1896.56  3.98E+09     4.16s     8.55s     0y 207d  8h  0m 42s     0     0   52

s SATISFIABLE
c sat_status                            1
c initial_maximal_clause_length         14
c initial_number_of_variables           185
c initial_number_of_clauses             9689
c initial_number_of_literal_occurrences 42542
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   0
c running_time(sec)                     8812.1
c number_of_nodes                       1956023
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                17875997
c number_of_pure_literals               185
c number_of_autarkies                   0
c number_of_missed_single_nodes         0
c max_tree_depth                        52
c number_of_table_enlargements          0
c number_of_1-autarkies                 4074
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             VanDerWaerden_2-3-14_185.cnf

> OKsolver_2002-O3-DNDEBUG -M -D21 -F VanDerWaerden_2-3-14_186.cnf
s UNSATISFIABLE
c sat_status                            0
c initial_maximal_clause_length         14
c initial_number_of_variables           186
c initial_number_of_clauses             9795
c initial_number_of_literal_occurrences 43014
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   0
c running_time(sec)                     103496.6
c number_of_nodes                       22469535
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                210989012
c number_of_pure_literals               2150
c number_of_autarkies                   0
c number_of_missed_single_nodes         8
c max_tree_depth                        52
c number_of_table_enlargements          0
c number_of_1-autarkies                 60129
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             VanDerWaerden_2-3-14_186.cnf

> E = read_oksolver_mon("VanDerWaerden_2-3-14_186.cnf.mo")
450700
> plot_oksolver_mon_nodes(E)
ldstep= 18 step= 262144 left= 128 right= 2097152
obs/count= 4.654129 nodes-range= 1 70632 ave-nodes-range= 10.713 2631.656
> summary_oksolver(E)
Nodes:
    Min.  1st Qu.   Median     Mean  3rd Qu.     Max.
    1.00     2.00     3.00    49.85     7.00 70630.00
2-reductions:
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
   0.20    7.50    9.92   11.76   13.46   70.00
Single nodes:
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
      0       0       0       0       0       0
Autarkies:
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
      0       0       0       0       0       0
Time ~ nodes:
[1] 0.9735288
 (Intercept)      E$nodes
-0.011336484  0.004833474
> hist_oksolver_mon_nodes(E)
Median= 1.584963
Mean= 5.639659
> hist_oksolver_mon_nodes(E,breaks="st")
Median= 1.584963
Mean= 5.639659
   \endverbatim
   The node-average continously decreases, and the initially very hard-looking
   problem turned out to be relatively easy. Since it ran on csltok, perhaps
   performance is actually not so much worse than that of satz215.
   Here it seems very interesting to optimise the heuristics! </li>
   <li> k=15, n=217, 218:
   \verbatim
> OKsolver_2002-O3-DNDEBUG -M -D26 -F VanDerWaerden_2-3-15_217.cnf
  4597:  12803   1046.97  7.03E+10   225.36s    18.37s    39y  35d 13h  0m 53s     0     0   60
s UNKNOWN
c sat_status                            2
c initial_maximal_clause_length         15
c initial_number_of_variables           217
c initial_number_of_clauses             13239
c initial_number_of_literal_occurrences 58617
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   0
c running_time(sec)                     84676.4
c number_of_nodes                       4830513
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                47307023
c number_of_pure_literals               2275
c number_of_autarkies                   0
c number_of_missed_single_nodes         3
c max_tree_depth                        60
c number_of_table_enlargements          0
c number_of_1-autarkies                 40863
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             VanDerWaerden_2-3-15_217.cnf

> E = read_oksolver_mon("VanDerWaerden_2-3-15_217.cnf.mo")
4012
> plot_oksolver_mon_nodes(E)
ldstep= 9 step= 512 left= 128 right= 4597
obs/count= 1.150283 nodes-range= 1 64636 ave-nodes-range= 727.307 2436.24
> summary_oksolver(E)
Nodes:
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
      1       5      22    1200     167   64640
2-reductions:
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
   0.96    9.53   10.27   11.32   11.65   56.00
Single nodes:
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
      0       0       0       0       0       0
Autarkies:
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
      0       0       0       0       0       0
Time ~ nodes:
[1] 0.9664525
(Intercept)     E$nodes
 0.33440742  0.01727101
> hist_oksolver_mon_nodes(E,breaks="st")
Median= 4.459432
Mean= 10.22837
   \endverbatim
   </li>
   Node counts show typical quasi-periodic patterns, distribution with
   two peaks, one close to 1, the other at around 12. Here it seems satz215
   is definitely better, which should be either due to the heuristics (by
   chance?!), or the partial r_3-reduction, or both. </li>
   <li> k=16, n=238: Running it with monitoring level 20 appears hopeless.
   And using the m2pp-variant doesn't make a difference, since the
   preprocessor has no effect. </li>
  </ul>


  \todo Performance of march_pl
  <ul>
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
   <li> k=13, n=160:
   \verbatim
> satz215 VanDerWaerden_2-3-13_160.cnf
****the instance is unsatisfiable *****
NB_MONO= 316, NB_UNIT= 38903958, NB_BRANCHE= 1699870, NB_BACK= 864253
Program terminated in 1308.930 seconds.
satz215 VanDerWaerden_2-3-13_160.cnf 1308.930 1699870 864253 214884364 8337569 0 160 7308 0 3878431 1563013
   \endverbatim
   So satz215 seems best-performing here.
   </li>
   <li> k=14, n=186:
   \verbatim
****the instance is unsatisfiable *****
NB_MONO= 1011, NB_UNIT= 275097539, NB_BRANCHE= 10842443, NB_BACK= 5511751
Program terminated in 11507.900 seconds.
satz215 VanDerWaerden_2-3-14_186.cnf 11507.900 10842443 5511751 1544946578 56724610 0 186 9795 0 26796344 10549534
   \endverbatim
   </li>
   <li> k=15, n=218:
   \verbatim
****the instance is unsatisfiable *****
NB_MONO= 2544, NB_UNIT= 1879235364, NB_BRANCHE= 66642611, NB_BACK= 33796681
Program terminated in 99407.850 seconds.
satz215 VanDerWaerden_2-3-15_218.cnf 99407.850 66642611 33796681 10812633494 373672422 0 218 13362 0 305553394 82352337
   \endverbatim
   </li>
   <li> k=16, n=238:
   \verbatim
****the instance is unsatisfiable *****
NB_MONO= 62751, NB_UNIT= 17632939133, NB_BRANCHE= 600383827, NB_BACK= 304409259
Program terminated in 990144.910 seconds.
satz215 VanDerWaerden_2-3-16_238.cnf 990144.910 600383827 304409259 99369777769 3501709276 0 238 15812 0 3305729737 824297599
   \endverbatim
   Stably a factor of 10. So with some parallelisation k=17 should be feasible
   (the unsatisfiable case; around 3 months on a single processor). </li>
  </ul>


  \todo Performance of minisat2
  <ul>
   <li> k=13, n=160: 19681930 conflicts, 19 restarts (4500s). </li>
   <li> k=14 </li>
   <li> k=16, n=238: Apparently after 30 restarts and 36906145 conflicts
   no progress was made. </li>
  </ul>


  \todo Local search for the satisfiable instances
  <ul>
   <li> The first impression is that ubcsat::samd is best. </li>
   <li> For k=16, n=237 a local minimum of 1 is easily found (even with just
   100000 steps), while to find a solution one should use, say, 10^7 steps
   (and then solutions seem to be found often, e.g., seed=370833091). </li>
   <li> But with vanderwaerden_2(3,17) (below) we see that adaptnovelty+
   is better; perhaps, as with vdw_2(4,k), getting stronger with higher
   cutoff. </li>
   <li> However, for vanderwaerden_2(3,18) samd again seems clearly better
   --- this needs to be investigated more thoroughly. </li>
  </ul>


  \todo vanderwaerden_2(3,17) >= 279
  <ul>
   <li> The prediction (see above) is n=279. </li>
   <li> n=270 easily found satisfiable with samd (cutoff=10^6). </li>
   <li> n=275 found satisfiable by adaptnovelty+ (run 14, cutoff=10^6,
   seed=1567198554). </li>
   <li> n=276 found satisfiable by adaptnovelty+ (run 28, cutoff=10^6,
   seed=1440019878). </li>
   <li> n=277 found satisfiable by adaptnovelty+ (run 10, cutoff=2*10^6,
   seed=2732014045). </li>
   <li> n=278
    <ol>
     <li> cutoff=2*10^6:
     \verbatim
> ubcsat-okl -alg adaptnovelty+ -runs 100 -cutoff 2000000 -i VanDerWaerden_2-3-17_278.cnf -solve | tee VanDerWaerden_2-3-17_278.cnf_AUS
Clauses = 21460
Variables = 278
TotalLiterals = 96272
FlipsPerSecond = 69054
BestStep_Mean = 391212.470000
Steps_Mean = 2000000.000000
Steps_Max = 2000000.000000
PercentSuccess = 0.00
BestSolution_Mean = 1.820000
BestSolution_Median = 2.000000
BestSolution_Min = 1.000000
BestSolution_Max = 2.000000
 1  2
18 82
     \endverbatim
     </li>
     <li> cutoff=4*10^6:
     \verbatim
 1  2
11 33
44
     \endverbatim
     </li>
     <li> cutoff=16*10^6: In run 222 a solution was found by adaptnovelty+
     (seed=2225069948, osteps=12016774). </li>
    </ol>
   </li>
   <li> n=279
    <ol>
     <li> cutoff=16*10^6:
     \verbatim
> ubcsat-okl -alg adaptnovelty+ -runs 400 -cutoff 16000000 -i VanDerWaerden_2-3-17_279.cnf -solve | tee VanDerWaerden_2-3-17_279.cnf_AUS
Clauses = 21616
Variables = 279
TotalLiterals = 96978
FlipsPerSecond = 67558
BestStep_Mean = 2654188.382500
Steps_Mean = 16000000.000000
Steps_Max = 16000000.000000
PercentSuccess = 0.00
BestSolution_Mean = 1.682500
BestSolution_Median = 2.000000
BestSolution_Min = 1.000000
BestSolution_Max = 2.000000
  1   2
127 273
     \endverbatim
     </li>
     <li> cutoff=32*10^6:
     \verbatim
  1   2
202 198
     \endverbatim
     </li>
     <li> cutoff=64*10^6:
     \verbatim
325  75
400
     \endverbatim
     </li>
    </ol>
   </li>
   <li> n=280
   \verbatim
> ubcsat-okl -alg samd -runs 100 -cutoff 1000000 -i VanDerWaerden_2-3-17_280.cnf -solve | tee VanDerWaerden_2-3-17_280.cnf_AUS
Clauses = 21772
Variables = 280
TotalLiterals = 97684
FlipsPerSecond = 131553
BestStep_Mean = 335464.530000
Steps_Mean = 1000000.000000
Steps_Max = 1000000.000000
PercentSuccess = 0.00
BestSolution_Mean = 2.090000
BestSolution_Median = 2.000000
BestSolution_Min = 2.000000
BestSolution_Max = 3.000000
 2  3
91  9
> ubcsat-okl -alg samd -runs 100 -cutoff 2000000 -i VanDerWaerden_2-3-17_280.cnf -solve | tee VanDerWaerden_2-3-17_280.cnf_AUS2 
 2  3
97  3
> ubcsat-okl -alg samd -runs 100 -cutoff 4000000 -i VanDerWaerden_2-3-17_280.cnf -solve | tee VanDerWaerden_2-3-17_280.cnf_AUS3
 1  2
 1 99
> ubcsat-okl -alg samd -runs 100 -cutoff 8000000 -i VanDerWaerden_2-3-17_280.cnf -solve | tee VanDerWaerden_2-3-17_280.cnf_AUS3
 1  2
 1 99
> ubcsat-okl -alg samd -runs 100 -cutoff 16000000 -i VanDerWaerden_2-3-17_280.cnf -solve | tee VanDerWaerden_2-3-17_280.cnf_AUS4
 1  2
 4 96
> ubcsat-okl -alg adaptnovelty+ -runs 100 -cutoff 16000000 -i VanDerWaerden_2-3-17_280.cnf -solve | tee VanDerWaerden_2-3-17_280.cnf_AUS4
Clauses = 21772
Variables = 280
TotalLiterals = 97684
FlipsPerSecond = 66820
BestStep_Mean = 1808806.410000
Steps_Mean = 16000000.000000
Steps_Max = 16000000.000000
PercentSuccess = 0.00
BestSolution_Mean = 1.860000
BestSolution_Median = 2.000000
BestSolution_Min = 1.000000
BestSolution_Max = 2.000000
 1  2
14 86
   \endverbatim
   So actually also here adaptnovelty+ seems best. </li>
  </ul>


  \todo vanderwaerden_2(3,18) >= 312
  <ul>
   <li> The prediction (see above) is n=314. </li>
   <li> n=300 found satisfiable with adaptnovelty+ with cutoff=10^6. </li>
   <li> n=301 found satisfiable with adaptnovelty+ with cutoff=10^6
   (seed=3824107188, osteps=627657). </li>
   <li> n=302 : cutoff=10^6
   \verbatim
 1  2
71 29
100
   \endverbatim
   but found satisfiable with cutoff=2*10^6 (seed=1205573131, osteps=988309,
   adaptnovelty+). </li>
   <li> n=303 : cutoff=10^6
   \verbatim
 1  2
61 39
100
   \endverbatim
    but found satisfiable with cutoff=2*10^6 (seed=3612981472, osteps=1991145,
    adaptnovelty+). </li>
   <li> n=305:
    <ol>
     <li> cutoff=10^6
     \verbatim
 1  2
61 39
100
     \endverbatim
     </li>
     <li> cutoff=2*10^6 found a solution in run 99 (seed=2434783444,
     osteps=532233, adaptnovelty+). </li>
    </ol>
   </li>
   <li> n=310: cutoff=10^6
   \verbatim
 1  2  3
60 39  1
100
   \endverbatim
   but with cutoff=8*10^6 in run 112 a solution was found (seed=1883662893,
   osteps=3087470, adaptnovelty+). </li>
   <li> n=311:
    <ol>
     <li> cutoff=8*10^6 yields in 400 runs always min=1. </li>
     <li> Using cutoff=4*10^6, in round 944 a solution was found
     (seed=1836688823, osteps=1914277, adaptnovelty+). </li>
    </ol>
   </li>
   <li> n=312
    <ol>
     <li> cutoff=4*10^6 with adaptnovelty+:
     \verbatim
   1    2
1942   58
2000
     \endverbatim
     (the histogram of osteps is nearly strictly decreasing, only at about
     3*10^6 we seem to have a local minimum, followed by a slight increase,
     peaking at 3.5*10^6). </li>
     <li> cutoff=8*10^6 with adaptnovelty+:
     \verbatim
> ubcsat-okl -alg adaptnovelty+ -runs 2000 -cutoff 8000000 -i VanDerWaerden_2-3-18_312.cnf -solve | tee VanDerWaerden_2-3-18_312.cnf_AUS2
Clauses = 26889
Variables = 312
TotalLiterals = 121302
FlipsPerSecond = 56414
BestStep_Mean = 1106252.638500
Steps_Mean = 8000000.000000
Steps_Max = 8000000.000000
PercentSuccess = 0.00
BestSolution_Mean = 1.000500
BestSolution_Median = 1.000000
BestSolution_Min = 1.000000
BestSolution_Max = 2.000000
   1    2
1999    1
2000
     \endverbatim
     (now the histogram is strictly decreasing).
     </li>
     <li> cutoff=10^6 to compare samd and adaptnovelty+:
     \verbatim
> ubcsat-okl -alg samd -runs 8000 -cutoff 1000000 -i VanDerWaerden_2-3-18_312.cnf -solve | tee VanDerWaerden_2-3-18_312.cnf_AUS3
Clauses = 26889
Variables = 312
TotalLiterals = 121302
FlipsPerSecond = 108486
BestStep_Mean = 296836.281000
Steps_Mean = 1000000.000000
Steps_Max = 1000000.000000
PercentSuccess = 0.00
BestSolution_Mean = 1.140750
BestSolution_Median = 1.000000
BestSolution_Min = 1.000000
BestSolution_Max = 4.000000
   1    2    3    4
6885 1105    9    1
8000

> ubcsat-okl -alg adaptnovelty+ -runs 8000 -cutoff 1000000 -i VanDerWaerden_2-3-18_312.cnf -solve | tee VanDerWaerden_2-3-18_312.cnf_AUS4
Clauses = 26889
Variables = 312
TotalLiterals = 121302
FlipsPerSecond = 56657
BestStep_Mean = 364589.452625
Steps_Mean = 1000000.000000
Steps_Max = 1000000.000000
PercentSuccess = 0.00
BestSolution_Mean = 1.430375
BestSolution_Median = 1.000000
BestSolution_Min = 1.000000
BestSolution_Max = 3.000000
   1    2    3
4708 3141  151
8000
     \endverbatim
     So actually samd seems to clearly outperform adaptnovelty+. </li>
     <li> Now cutoff=8*10^6:
     \verbatim
> ubcsat-okl -alg samd -runs 1000 -cutoff 8000000 -i VanDerWaerden_2-3-18_312.cnf -solve | tee VanDerWaerden_2-3-18_312.cnf_AUS5
Clauses = 26889
Variables = 312
TotalLiterals = 121302
FlipsPerSecond = 122375
BestStep_Mean = 486193.876000
Steps_Mean = 8000000.000000
Steps_Max = 8000000.000000
PercentSuccess = 0.00
BestSolution_Mean = 1.013000
BestSolution_Median = 1.000000
BestSolution_Min = 1.000000
BestSolution_Max = 10.000000
  1   2  10
995   4   1
1000
     \endverbatim
     So perhaps with higher cutoff adaptnovelty+ gets better? Though samd
     appears to be faster. </li>
    </ol>
   </li>
  </ul>


  \todo vanderwaerden_2(3,19) >= 349
  <ul>
   <li> The predictions (see above) are n= 347, 350. </li>
   <li> Experience with k=18 is that samd is best, however we should
   have a look again --- this might change with different k's or with
   different cutoff's. </li>
   <li> Evaluating
   \verbatim
E = eval_ubcsat("VanDerWaerden_2-3-19_345.cnf", params=list(runs=100,cutoff=10000),monitor=TRUE)
   \endverbatim
   by plot(E$alg,E$best), the best algorithms seem gsat_tabu, rnovelty and
   rsaps (all three found solutions). </li>
   </li>
   <li> Evaluating
   \verbatim
E = eval_ubcsat("VanDerWaerden_2-3-19_347.cnf", params=list(runs=100,cutoff=10000),monitor=TRUE)
   \endverbatim
   by plot(E$alg,E$best), now the best algorithms seem gsat_tabu, rnovelty,
   rnoveltyp, saps, rsaps and samd. </li>
   <li> Using cutoff=100000:
   \verbatim
E = eval_ubcsat("VanDerWaerden_2-3-19_347.cnf", params=list(runs=100,cutoff=100000),monitor=TRUE)
   \endverbatim
   by plot(E$alg,E$best), now the best algorithms seem gsat_tabu,
   adaptnoveltyp, rnoveltyp, rsaps, rots and samd (all found a solution).
   gsat_tabu seems the best, followed by samd. </li>
   <li> Evaluating
   \verbatim
E = eval_ubcsat("VanDerWaerden_2-3-19_348.cnf", params=list(runs=100,cutoff=100000),monitor=TRUE)
   \endverbatim
   now only two algorithms found a solution, gsat_tabu and samd, and these
   two seem to be best (hard to distinguish between them). </li>
   <li> Evaluating
   \verbatim
> E = eval_ubcsat("VanDerWaerden_2-3-19_349.cnf", params=list(runs=100,cutoff=100000),monitor=TRUE)
   \endverbatim
   adaptnoveltyp, rnovelty, rnoveltyp hard to distinguish, gsat_tabu has higher
   variance, the bulk of rsaps' results are far worse, but outliers reach also
   min=1, rots seems a bit worse than the novelty's and gsat_tabu, and finally
   samd seems best. </li>
   <li> Using cutoff=10^6: Now gsat_tabu is best, followed by samd (all
   just reaching min=1). </li>
   <li> So let's assume that gsat-tabu is the best. </li>
   <li> n=349:
    <ol>
     <li> Using gsat-tabu, with cutoff=10^6:
     \verbatim
   1    2    3    4    9
1143  834   20    2    1
2000
     \endverbatim
     </li>
     <li> gsat-tabu with cutoff=4*10^6:
     \verbatim
   1    2    3
1909   88    3
2000
     \endverbatim
     </li>
    </ol>
   </li>
  </ul>


  \todo vanderwaerden_2(3,20) > 386
  <ul>
   <li> The predictions (see above) are n= 386, 389. </li>
   <li> Experience with k=19 is that gsat-tabu is best, however we should
   have a look again --- this might change with different k's or with
   different cutoff's. </li>
   <li> Evaluating
   \verbatim
> E = eval_ubcsat("VanDerWaerden_2-3-20_385.cnf", params=list(runs=100,cutoff=100000),monitor=TRUE)
   \endverbatim
   gsat-tabu, adaptnovelty+ and rnoveltyp seem best (all reaching min=2). </li>
   </li>
   <li> Now using cutoff=10^6: gsat-tabu seems best, and also finds as only
   solver a solution (this could be luck, and rnovelty as the second solver
   could be better). </li>
   <li> n=386:
    <ol>
     <li> cutoff=2*10^6 with gsat-tabu: found a solution in run 2 
     (seed=3063820134). </li>
    </ol>
   </li>
   <li> n=387:
    <ol>
     <li> cutoff=2*10^6 with gsat-tabu:
     \verbatim
 2  3  4
14 50 36
100
> summary(E$osteps)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  26010  189900  596200  702300 1104000 1967000
     \endverbatim
     </li>
     <li> cutoff=4*10^6 with gsat-tabu:
     \verbatim
 2  3  4
30 56 14
100
> summary(E$osteps)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  41100  614600 1393000 1479000 2188000 3910000
     \endverbatim
     </li>
     <li> cutoff=4*10^6 with rnovelty:
     \verbatim
     \endverbatim
     </li>
    </ol>
   </li>
  </ul>

*/


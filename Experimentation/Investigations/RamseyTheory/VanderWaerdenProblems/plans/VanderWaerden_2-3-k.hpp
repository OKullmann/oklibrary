// Oliver Kullmann, 10.4.2009 (Swansea)
/* Copyright 2009, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/VanderWaerden_2-3-k.hpp
  \brief On investigations into vdW-numbers vdw_2(3,k)

  Instances created by output_vanderwaerden2nd_stdname(3,k,n) at Maxima-level,
  or by "VanderWaerden-O3-DNDEBUG 3 k n" at C++ level (instances differ by
  the order of negated literals in clauses).
  An experiment is run by "RunVdW3k k n0 alg runs cutoff".

  
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
 [3,6,9,18,22,32,46,58,77,97,114,135,160,186,218,238,279,312,349,unknown]
   \endverbatim
   </li>
   <li> Log-log regression (in R) does not yield a good model, as can be
   seen inspecting the plot:
   \verbatim
d0 = c(3,6,9,18,22,32,46,58,77,97,114,135,160,186,218,238,279,312)
plot(d0)
lines(d0)
x0 = log((1:length(d0)))
y0 = log(d0)
plot(x0,y0)
L0 = lm(y0 ~ x0)
summary(L0)

Coefficients:
            Estimate Std. Error t value Pr(>|t|)
(Intercept)   0.6000     0.1257   4.773 0.000207 ***
x0            1.7232     0.0580  29.708    2e-15 ***
Residual standard error: 0.1918 on 16 degrees of freedom
Multiple R-squared: 0.9822,     Adjusted R-squared: 0.9811

lines(x0,predict(L0))
   \endverbatim
   </li>
   <li> Using the conjectured values 349, 389, 416, 464, 516, 593, 656, 727, 770, 827, 868 (see below):
   \verbatim
d = append(d0, c(349,389,416,464,516,593,656,727,770,827,868))
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
round(fnl(1:31))
 1 3 6 12 19 29 40 54 70 87
 108 130 155 182 211 243 277 314 353 395
 439 486 535 587 642 699 759 821 886 954
 1024

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
round(fnlq(1:31))
 13 12 13 16 22 30 41 54 69 87
 107 130 155 182 212 244 279 316 355 397
 441 488 537 588 642 699 757 818 882 948
 1016

d - round(fnlq(X))
 -10 -6 -4 2 0 2 5 4 8 10
 7 5 5 4 6 -6 0 -4 -6 -8
 -25 -24 -21 5 14 28 13 9 -14
   \endverbatim
   Hard to distinguish from the above model. </li>
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
   <li> This value has been verified by Tanbir Ahmed. </li>
   <li> Experience with k=18 is that samd is best, however we should
   have a look again --- this might change with different k's or with
   different cutoff's. </li>
   <li> Evaluating
   \verbatim
E = eval_ubcsat("VanDerWaerden_2-3-19_345.cnf", runs=100,cutoff=10000,monitor=TRUE)
   \endverbatim
   by plot(E$alg,E$best), the best algorithms seem gsat_tabu, rnovelty and
   rsaps (all three found solutions). </li>
   </li>
   <li> Evaluating
   \verbatim
E = eval_ubcsat("VanDerWaerden_2-3-19_347.cnf", runs=100,cutoff=10000,monitor=TRUE)
   \endverbatim
   by plot(E$alg,E$best), now the best algorithms seem gsat_tabu, rnovelty,
   rnoveltyp, saps, rsaps and samd. </li>
   <li> Using cutoff=100000:
   \verbatim
E = eval_ubcsat("VanDerWaerden_2-3-19_347.cnf", runs=100,cutoff=100000,monitor=TRUE)
   \endverbatim
   by plot(E$alg,E$best), now the best algorithms seem gsat_tabu,
   adaptnoveltyp, rnoveltyp, rsaps, rots and samd (all found a solution).
   gsat_tabu seems the best, followed by samd. </li>
   <li> Evaluating
   \verbatim
E = eval_ubcsat("VanDerWaerden_2-3-19_348.cnf", runs=100,cutoff=100000,monitor=TRUE)
   \endverbatim
   now only two algorithms found a solution, gsat_tabu and samd, and these
   two seem to be best (hard to distinguish between them). </li>
   <li> Evaluating
   \verbatim
> E = eval_ubcsat("VanDerWaerden_2-3-19_349.cnf", runs=100,cutoff=100000,monitor=TRUE)
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


  \todo vanderwaerden_2(3,20) >= 389
  <ul>
   <li> Experience with k=19 is that gsat-tabu is best, and that seems to be
   true here too. </li>
   <li> Evaluating
   \verbatim
> E = eval_ubcsat("VanDerWaerden_2-3-20_385.cnf", runs=100,cutoff=100000,monitor=TRUE)
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
   <li> n=386: gsat-tabu finds a solution (modified stepwise from n=379), with
   certificate:
   \verbatim
18,30,35,43,45,50,64,74,79,81,
96,97,101,102,104,114,133,143,148,150,
156,160,171,188,197,214,225,229,235,237,
242,252,271,281,283,284,288,289,304,306,
311,321,335,340,342,350,355,367
   \endverbatim
   Another run with gsat-tabu found also this solution, this time modified
   stepwise from n=383. </li>
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
     <li> cutoff=4*10^6 with rnovelty: found a solution in run 62
    (seed=1299962486, osteps=1466958). </li>
     <li> rnovelty might be better:
     \verbatim
 0  2  3  4
 1 21 29 11
62
> summary(E$osteps)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  14410  614000 1303000 1467000 2288000 3844000
     \endverbatim
     </li>
     <li> For comparison: adaptnovelty+:
     \verbatim
>  ubcsat-okl -alg adaptnovelty+ -runs 100 -cutoff 8000000 -i VanDerWaerden_2-3-20_387.cnf -solve | tee VanDerWaerden_2-3-20_387.cnf_AUS4
 1  2  3  4
 1 51 44  4
100
> summary(E$osteps)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
 193900 1162000 2486000 2976000 4207000 7925000
     \endverbatim
     Unclear; but it seems the cutoff has to be increased to 8*10^6. </li>
     <li> Using RunVdW3k with gsat-tabu, runs=100 and cutoff=8*10^6 finds a
     solution for n=386, but not for n=387:
     \verbatim
 1  2  3  4 
 1 41 55  3 
100 
     \endverbatim
     rnovelty with the same parameters has bigger problems, and finds only
     solutions up to n=381. </li>
    </ol>
   </li>
   <li> n=388
    <ol>
     <li> rnovelty with cutoff=8*10^6: found a solution in run 33
     (seed=2441787444, osteps=4412722). Reproduction does not succeed (new
     order of clauses etc.), but we just see whether we can find just new
     solutions. </li>
     <li> gsat-tabu with cutoff=8*10^6:
     \verbatim
 1  2  3  4
 3 32 62  3
100
> summary(E$osteps)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  25570  924300 2283000 2818000 4242000 7901000
     \endverbatim
     </li>
     <li> RunVdW3k with gsat-tabu, cut=2*10^7 finds a solution in run 1, with
     certificate
     \verbatim
20,32,37,45,47,52,66,76,81,83,
98,99,103,104,106,116,135,145,150,152,
158,162,173,190,199,216,227,231,237,239,
244,254,273,283,285,286,290,291,306,308,
313,323,337,342,344,352,357,369
     \endverbatim
     </li>
    </ol>
   </li>
   <li> n=389: 
    <ol>
     <li> cutoff=8*10^6:
     \verbatim
> ubcsat-okl -alg rnovelty -runs 100 -cutoff 8000000 -i VanDerWaerden_2-3-20_389.cnf -solve | tee VanDerWaerden_2-3-20_389.cnf_AUS
 2  3  4
42 53  5
> summary(E$osteps)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  31700 1449000 2589000 2981000 4027000 7955000
>  ubcsat-okl -alg gsat-tabu -runs 100 -cutoff 8000000 -i VanDerWaerden_2-3-20_389.cnf -solve | tee VanDerWaerden_2-3-20_389.cnf_AUS2
 1  2  3  4
 1 29 60 10
> summary(E$osteps)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  18300  631700 1531000 2300000 3661000 7883000
     \endverbatim
     It seems rnovelty is a bit better. </li>
     <li> cutoff = 10^8:
     \verbatim
> nohup ubcsat-okl -alg rnovelty -runs 1000 -cutoff 100000000 -i VanDerWaerden_2-3-20_389.cnf -solve > VanDerWaerden_2-3-20_389.cnf_AUS
> E=read_ubcsat("VanDerWaerden_2-3-20_389.cnf_AUS")
  1   2
 28 371
399
> summary(E$osteps)
    Min.  1st Qu.   Median     Mean  3rd Qu.     Max.
   67020  3267000  9072000 15800000 21300000 92970000
> summary(E$osteps[E$min==1])
    Min.  1st Qu.   Median     Mean  3rd Qu.     Max.
  551000 24360000 57580000 49740000 72950000 92970000

> ubcsat-okl -alg rnovelty -runs 1000 -cutoff 100000000 -i VanDerWaerden_2-3-20_389.cnf -solve | tee VanDerWaerden_2-3-20_389.cnf_AUS3
> E = read_ubcsat("VanDerWaerden_2-3-20_389.cnf_AUS3")
  1   2
 67 933
1000
> summary(E$osteps)
    Min.  1st Qu.   Median     Mean  3rd Qu.     Max.
   17140  3765000  9559000 15030000 18850000 98290000

> nohup ubcsat-okl -alg gsat-tabu -runs 1000 -cutoff 100000000 -i VanDerWaerden_2-3-20_389.cnf -solve > VanDerWaerden_2-3-20_389.cnf_AUS4 &
> E=read_ubcsat("VanDerWaerden_2-3-20_389.cnf_AUS4")
  1   2   3   4   5   6
165 750  71  10   3   1
1000
> summary(E$osteps)
    Min.  1st Qu.   Median     Mean  3rd Qu.     Max.
    7145  4473000 13520000 21760000 31660000 99550000

> nohup ubcsat-okl -alg adaptnovelty+ -runs 1000 -cutoff 100000000 -i VanDerWaerden_2-3-20_389.cnf -solve > VanDerWaerden_2-3-20_389.cnf_AUS5 &
> E = read_ubcsat("VanDerWaerden_2-3-20_389.cnf_AUS5");
  1   2   3
 64 520   6
590
> summary(E$osteps)
    Min.  1st Qu.   Median     Mean  3rd Qu.     Max.
   51760  5938000 15350000 22130000 34500000 99970000

     \endverbatim
     So actually gsat-tabu is better than rnovelty for higher cutoff, and
     adaptnovelty+ gets better, but still worse than gsat-tabu. </li>
    </ol>
   </li>
  </ul>


  \todo vanderwaerden_2(3,21) >= 416
  <ul>
   <li> The conjecture is vanderwaerden_2(3,21) = 416. </li>
   <li> Experience with k=20 is that gsat-tabu is best, however we should
   have a look again --- this might change with different k's or with
   different cutoff's. </li>
   <li> n=394, gsat-tabu, cutoff=10^8: in run 1 a solution was found. </li>
   <li> n=396, gsat-tabu, cutoff=10^8: in run 2 a solution was found. </li>
   <li> n=398, gsat-tabu, cutoff=10^8: in 100 runs 73 solutions were found.
    </li>
   <li> n=410, gsat-tabu, cutoff=10^8: in 100 runs 36 solutions were found.
   </li>
   <li> n=415
    <ol>
     <li> gsat-tabu, cutoff=10^8: in run 3 a solution was found
     (seed=174271267, osteps=91827849). Further statistics:
     \verbatim
 0   1  2 3 4 5
17 147 28 5 2 1
200
     \endverbatim
     </li>
     <li> RunVdWk3 with cutoff=10^7 and 100 runs finds a solution (run 69),
     with certificate
     \verbatim
9,27,29,36,38,48,61,62,82,101,
105,107,110,123,124,130,131,133,144,163,
167,169,170,179,192,213,232,236,238,246,
255,268,269,271,289,308,312,314,315,324,
330,337,338,351,358,374,376,381,383,395
     \endverbatim
     </li>
     <li> RunVdWk3 with cutoff=2*10^7 finds a solution (run 73), same as
     above, but with vertex 133 removed. </li>
     <li> Is this the only solution (up to small variations)? </li>
    </ol>
   </li>
   <li> n=416, gsat-tabu
    <ol>
     <li> cutoff=10^7:
     \verbatim
  1   2   3   4   5
405 572  20   2   1
1000
 1  2 
39 61 
100
     \endverbatim
     </li>
     <li> cutoff=2*10^7:
     \verbatim
  1   2   3   4 
123  70   5   2 
200 
     \endverbatim
     </li>
     <li> cutoff=10^8:
     \verbatim
  1  2 3 4
245 48 6 1
300
     \endverbatim
     </li>
    </ol>
   </li>
   <li> n=420, gsat-tabu, cutoff=10^8:
   \verbatim
  1  2  3 6
249 40 10 1
300
   \endverbatim
   </li>
   <li> n=423, gsat-tabu, cutoff=10^8:
   \verbatim
 1  2  3  4
74 19  5  2
100
   \endverbatim
   </li>
   <li> n=424, gsat-tabu, cutoff=10^8:
   \verbatim
  1  2  3  4 6
138 52  5  4 1
200
   \endverbatim
   </li>
   <li> n=425, gsat-tabu, cutoff=10^8:
   \verbatim
  1  2  3 6
117 41 13 1
172
   \endverbatim
   </li>
   <li> n=426, gsat-tabu, cutoff=10^8:
   \verbatim
  1  2 3 4
120 71 8 1
200
   \endverbatim
   </li>
  </ul>


  \todo vanderwaerden_2(3,22) >= 464
  <ul>
   <li> The conjecture is vanderwaerden_2(3,22) = 464. </li>
   <li> Experience with k=20 is that gsat-tabu is best, however we should
   have a look again --- this might change with different k's or with
   different cutoff's. </li>
   <li> n=430, gsat-tabu, cutoff=10^8: 12 runs each found a solution. </li>
   <li> n=435, gsat-tabu, cutoff=10^8: 5 runs each found a solution. </li>
   <li> n=438, gsat-tabu, cutoff=10^8: 6 runs each found a solution. </li>
   <li> n=439, gsat-tabu, cutoff=10^8: 5 runs each found a solution. </li>
   <li> n=440, gsat-tabu, cutoff=10^8: 3 runs each found a solution (while
   one run apparently didn't find a solution). </li>
   <li> n=445, gsat-tabu, cutoff=10^8: 5 runs each found a solution. </li>
   <li> n=450, gsat-tabu, cutoff=10^8: 3 runs each found a solution (while
   one run possibly didn't find a solution). </li>
   <li> n=455, gsat-tabu, cutoff=10^8: in 3 runs one solution was found
   (seed=638321166, osteps=8804984). </li>
   <li> n=460, gsat-tabu, cutoff=10^8: in 300 runs one solution was found
   (seed=3944359483, osteps=7057558):
   \verbatim
 0   1  2 3 4 5
 1 258 32 6 2 1
300
   \endverbatim
   </li>
   <li> n=461, gsat-tabu, cutoff=2*10^8: In 39 runs a solution was found
   (seed=696047484, osteps=20698025). It seems one should less steps. </li>
   <li> n=462
    <ol>
     <li> gsat-tabu, cutoff=5*10^7:
     \verbatim
 1  2  3
81 17  2
100
     \endverbatim
     In further 313 runs one solution was found (seed=270943421,
     osteps=11236258). </li>
     </li>
     <li> gsat-tabu, cutoff=5*10^7, finds a solution, adapted from n=458
     (run 31), with certificate:
     \verbatim
6,19,25,38,48,54,63,67,69,79,
92,98,106,111,122,125,141,154,156,169,
179,180,198,199,209,222,224,237,253,256,
267,272,280,286,299,309,311,315,324,328,
330,340,353,359,367,372,383,386,402,415,
417,430,440,459,460
     \endverbatim
     </li>
    </ol>
   </li>
   <li> n=463, gsat-tabu, cutoff=5*10^7: in 140 runs one solution was found
   (seed=552382418, osteps=1894212) (while 74 runs with cutoff=10^8 didn't
   find a solution). </li>
   <li> n=464, gsat-tabu:
    <ol>
     <li> cutoff=5*10^7:
     \verbatim
   1   2  3  4 5 6 7
2019 615 63 12 4 2 1
2716
     \endverbatim
     </li>
     <li> cutoff=10^8:
     \verbatim
 1  2  3  6
71 25  3  1
100
 1  2  3  5
79 18  2  1
100
     \endverbatim
     </li>
    </ol>
   </li>
  </ul>


  \todo vanderwaerden_2(3,23) >= 516
  <ul>
   <li> The conjecture is vanderwaerden_2(3,23) = 516. </li>
   <li> Experience with k=20 is that gsat-tabu is best, however we should
   have a look again --- this might change with different k's or with
   different cutoff's. </li>
   <li> n=500, gsat-tabu, cutoff=10^8: easily found a solution. </li>
   <li> n=510, gsat-tabu, cutoff=10^8: a solution was found in the first
   run (seed=1387406527, osteps=43902628). </li>
   <li> n=511, gsat-tabu, cutoff=10^8: in 31 runs two solutions were found
   (seed=2372840954, osteps=16960676). </li>
   <li> n=512:
    <ol>
     <li> gsat-tabu, cutoff=10^8: in 75 runs one solution was found
     (seed=216008612, osteps=49163297). </li>
     <li> cutoff=5*10^7, gsat-tabu finds a solution, taken over from n=511 
     (run 35), with certificate
     \verbatim
8,10,28,33,49,51,59,62,79,83,
95,97,105,106,118,120,142,144,149,152,
171,172,175,197,218,234,240,241,243,258,
266,281,286,303,312,327,346,347,349,355,
356,369,378,379,393,404,416,422,425,441,
443,448,450,468,487,491
     \endverbatim
     </li>
    </ol>
   </li>
   <li> n=513, gsat-tabu, cutoff=10^8: in 187 runs one solution was found
   (seed=1657670235,osteps=4640848). </li>
   <li> n=514, gsat-tabu
    <ol>
     <li> cutoff=10^8: in 116 runs one solution was found (seed=1685669740,
     osteps=9763052). </li>
     <li> cutoff=2*10^8: in 126 runs one solution was found (seed=1343283093,
     osteps=10794090. </li>
    </ol>
   </li>
   <li> n=515, gsat-tabu, cutoff=10^8: in 23 runs a solution was found
   (seed=4106190469, osteps=73878847). </li>
   <li> n=516, gsat-tabu
    <ol>
     <li> cutoff=5*10^7:
     \verbatim
  1   2   3   4   5   6   7
189 351 101  35  10   3   1
690
     \endverbatim
     </li>
     <li> cutoff=10^8: 
     \verbatim
  1   2  3  4  5 6 9 10
188 276 87 28 14 5 1  1
600
     \endverbatim
     </li>
     <li> cutoff=2*10^8: 
     \verbatim
 1  2  3  4  5  8
32 41 17  7  2  1
100
> summary(E$osteps)
    Min.  1st Qu.   Median     Mean  3rd Qu.     Max.
   27750  1668000  6598000 12460000 15130000 77790000
 1  2  3  4  5  7
30 51 14  2  2  1
100
> summary(E$osteps)
     Min.   1st Qu.    Median      Mean   3rd Qu.      Max.
   223800   2602000   7126000  13450000  16280000 107900000
     \endverbatim
     So the higher cutoff here seems completely uneffective. </li>
    </ol>
   </li>
   <li> n=520, gsat-tabu, cutoff=10^8: in 5 runs only min=2 was reached, so
   let's consider this for now as unsatisfiable. </li>
  </ul>


  \todo vanderwaerden_2(3,24) >= 593
  <ul>
   <li> The conjecture is vanderwaerden_2(3,24) = 593. </li>
   <li> Experience with k=20 is that gsat-tabu is best, however now it seems
   that rots is best (using half of the cutoff of gsat-tabu). </li>
   <li> n=530, gsat-tabu, cutoff=10^8: A solution was easily found. </li>
   <li> n=540, gsat-tabu, cutoff=10^8:
   \verbatim
> ubcsat-okl -alg gsat-tabu -runs 100 -cutoff 100000000 -i VanDerWaerden_2-3-24_540.cnf | tee VanDerWaerden_2-3-24_540.cnf_OUT
# -alg gsat-tabu -runs 100 -cutoff 100000000 -i VanDerWaerden_2-3-24_540.cnf
       sat  min     osteps     msteps       seed
      1 0     1   22497477  100000000 1352939123 
      2 0     2    1782874  100000000 1502390071 
      3 1     0   16984061   16984061 2901963531 
   \endverbatim
   </li>
   <li> n=560, gsat-tabu, cutoff=10^8:
   \verbatim
> ubcsat-okl -alg gsat-tabu -runs 100 -cutoff 100000000 -i VanDerWaerden_2-3-24_560.cnf | tee VanDerWaerden_2-3-24_560.cnf_OUT
# -alg gsat-tabu -runs 100 -cutoff 100000000 -i VanDerWaerden_2-3-24_560.cnf
       sat  min     osteps     msteps       seed
      1 0     1   17750621  100000000 1956429616 
      2 0     5    2138624  100000000  291337957 
      3 0     2     516286  100000000  890207100 
      4 0     2    6874501  100000000 1078823928 
      5 1     0   59397520   59397520 2277207974 
      6 1     0   21657900   21657900 2077294880 
      7 0     2    6943316  100000000 2830567231 
      8 0     1    2073902  100000000 3486000509 
   \endverbatim
   </li>
   <li> Running the search-tool:
   \verbatim
OKplatform> RunVdW3k 24 583 gsat-tabu 1000 10000000
   \endverbatim
   (first "OKplatform> RunVdW3k 24 561 gsat-tabu 100 100000000" was used,
   but the lower cutoff seems more efficient):
    <ol>
     <li> n=578 found satisfiable (seed=3813501165, osteps=1602760) in run 6.
     </li>
     <li> The elements of the k=3-partition of the solution are
     \verbatim
[
 3,22,43,44,61,72,80,89,111,118,
 126,130,140,152,155,177,181,189,196,218,
 227,235,239,246,254,263,264,268,276,283,
 285,292,300,304,305,322,333,341,350,372,
 379,387,391,401,413,416,438,442,450,457,
 479,488,496,500,503,507,515,524,537,546,
 553,561
]
     \endverbatim
     (62 elements). </li>
     <li> Checking:
     \verbatim
default_memory_ecl();
S : [
 3,22,43,44,61,72,80,89,111,118,
 126,130,140,152,155,177,181,189,196,218,
 227,235,239,246,254,263,264,268,276,283,
 285,292,300,304,305,322,333,341,350,372,
 379,387,391,401,413,416,438,442,450,457,
 479,488,496,500,503,507,515,524,537,546,
 553,561
]$
P : create_certificate2_vdw(S,578);
check_certificate_vdw([3,24],578,P);
  true
     \endverbatim
     </li>
     <li> n=579 found satisfiable (seed=1409510574 , osteps=5706287) in run 33.
     The elements of the k=3-partition of the solution are
     \verbatim
[
 15,34,51,56,64,73,88,92,93,102,
 110,114,117,138,139,143,151,167,175,179,
 180,201,204,208,216,226,245,247,254,276,
 284,295,303,325,332,334,353,363,371,375,
 378,399,400,404,412,428,436,440,441,462,
 469,477,486,487,491,506,508,515,523,528,
 545,564
]
     \endverbatim
     (62 elements). </li>
     <li> n=580 found satisfiable (seed=3723870221 , osteps=366990) in run 11.
     The elements of the k=3-partition of the solution are
     \verbatim
default_memory_ecl();
S : [
 10,29,46,51,59,66,68,83,87,88,
 97,105,112,133,134,138,146,162,170,174,
 175,196,199,203,211,221,240,242,249,271,
 279,290,298,320,327,329,348,358,366,370,
 373,394,395,399,407,423,431,435,436,457,
 460,464,472,481,482,486,501,503,510,518,
 523,540,559
]$
P : create_certificate2_vdw(S,580);
check_certificate_vdw([3,24],580,P);
  true
     \endverbatim
     (63 elements). </li>
     <li> n=586 found satisfiable (seed=3343037665, osteps=1598927, run=12),
     where the elements of the k=3-partition of the solution are
     \verbatim
18,37,54,59,67,74,76,91,95,96,
105,113,120,141,142,146,154,170,178,182,
183,204,207,211,219,229,248,250,257,279,
287,298,306,328,335,337,356,366,374,378,
381,402,403,407,415,431,439,443,444,465,
472,480,489,490,494,509,511,518,526,531,
548,567
     \endverbatim
     </li>
     <li> n=587 found satisfiable (seed=410357823, osteps=7862929, run=39),
     where the elements of the k=3-partition of the solution are
     \verbatim
18,37,54,59,67,74,76,91,95,96,
105,113,120,141,142,146,154,170,178,182,
183,204,207,211,219,229,248,250,257,279,
287,298,306,328,335,337,356,366,374,378,
381,402,403,407,415,431,439,443,444,465,
472,480,489,490,494,509,511,518,526,531,
548,567
     \endverbatim
     This is the same as for n=586 ! </li>
     <li> n=590 found satisfiable (seed=1242463204, osteps=2333499, run=48),
     with solution
     \verbatim
21,40,57,62,70,77,79,94,98,99,
108,116,120,123,144,145,149,157,173,181,
185,186,207,210,214,222,232,251,253,260,
282,290,301,309,331,338,340,359,369,377,
381,384,405,406,410,418,434,442,446,447,
468,471,475,483,492,493,497,512,514,521,
529,534,551,570
     \endverbatim
     </li>
    </ol>
   </li>
   <li> Evaluating
   \verbatim
E = eval_ubcsat("VanDerWaerden_2-3-24_561.cnf", runs=100,cutoff=1000000)
   \endverbatim
   by plot(E$alg,E$best) and eval_ubcsat_dataframe(E):
   \verbatim
gsat_tabu : 
 0  1  2  3  4  5  6  7  8  9 10 
 4  4 13  6 18 20 17 12  3  1  2 
samd : 
 0  1  2  3  4  5  6  7  8  9 10 
 3  4  8 10 17 12 22 12  7  4  1 
rots : 
 0  1  2  3  4  5  6  7  8  9 10 
 1  5  2  8  4 10 15 15 21 17  2 
   \endverbatim
   Now only these best three algorithms:
   \verbatim
E = eval_ubcsat("VanDerWaerden_2-3-24_575.cnf", algs=list(gsat_tabu="gsat-tabu",samd="samd",rots="rots"), runs=500,cutoff=10000000)
plot(E$alg,E$best)
eval_ubcsat_dataframe(E)
rots : 
  0   1   2   3   4   5   6   7   8 
 35  90  94 110  94  45  19   9   4
FlipsPerSecond = 67637
samd : 
  0   1   2   3   4   5   6   7   8  10  11  12  14  16  17 
 27  49  78 137  87  64  41   7   2   1   3   1   1   1   1
FlipsPerSecond = 118920
gsat_tabu : 
  0   1   2   3   4   5   6   7   8   9  15 
 23  62  98 131  98  41  34   7   4   1   1 
FlipsPerSecond = 121030
   \endverbatim
   It seems that rots with half of the cutoff of gsat-tabu is better than
   gsat-tabu. </li>
   <li> Running an search:
   \verbatim
OKplatform> RunVdW3k 24 591 rots 1000 5000000
   \endverbatim
   </li>
   <li> n=592
    <ol>
     <li> Found satisfiable (rots, seed=1337569052, osteps=2845761,
     run=398), with solution
     \verbatim
22,41,58,63,71,78,80,95,99,100,
109,117,121,124,145,146,150,158,174,182,
186,187,208,211,215,223,233,252,254,261,
283,291,302,310,332,339,341,360,370,378,
382,385,406,407,411,419,435,443,447,448,
469,472,476,484,493,494,498,513,515,522,
530,535,552,571
     \endverbatim
     </li>
     <li> 1000 runs with rots, cutoff=5*10^6 found only one solution:
     \verbatim
> E=read_ubcsat("VanDerWaerden_2-3-24_592.cnf_OUT",nrows=1000)
  0   1   2   3   4   5   6   7   8   9  10  11 
  1  25 167 265 267 132  60  35  20  17   8   3 
1000
> E[E$sat==1,]
    sat min  osteps  msteps       seed
168   1   0 2119635 2119635 1621341392
     \endverbatim
     where the solution is
     \verbatim
> ubcsat-okl -alg rots -seed 1621341392 -cutoff 2119635 -i Exp_VanderWaerden_2-3-24_2010-10-12-214502_591/VanDerWaerden_2-3-24_592.cnf -solve | tee VanDerWaerden_2-3-24_592.cnf_OUT2
>
> ExtractCertificate_vdW2 VanDerWaerden_2-3-24_592.cnf_OUT2
22,41,58,63,71,78,80,95,99,100,
109,117,124,145,146,150,158,174,182,186,
187,208,211,215,223,233,252,254,261,283,
291,302,310,332,339,341,360,370,378,382,
385,406,407,411,419,435,443,447,448,469,
472,476,484,493,494,498,513,515,522,530,
535,552,571
     \endverbatim
     This is the same solution as above, only vertex 121 here was left
     out. </li>
     <li> 500 runs with rots, cutoff=10^7 found only one solution. </li>
     <li> rots, cutoff=10^7, finds a solution in run 23 (osteps=1273935,
     seed=2153150186):
     \verbatim
22,41,58,63,71,78,80,95,99,100,
109,117,121,124,145,146,150,158,174,182,
186,187,208,211,215,223,233,252,254,261,
283,291,302,310,332,339,341,360,370,378,
382,385,406,407,411,419,435,443,447,448,
469,476,484,493,494,498,513,515,522,530,
535,552,571
     \endverbatim
     The same solution as above (last version), but with vertex 121 replacing
    vertex 472. </li>
     <li> rots, cutoff=10^7, finds a solution in run 215 (osteps=1898408,
     seed=4066830458), the same as the first one found. </li>
     <li> rots, cutoff=5*10^7, finds a solution in run 14 (osteps=19500171,
     seed=1877893773), which was already found. </li>
     <li> So one could conjecture that this solution with small variations is
     the only solution for n=592 (which would show that vdw_2(3,24)=593). </li>
    </ol>
   </li>
   <li> Restarting the search, now using the solution found for n=592
   as initial assignment:
   \verbatim
OKplatform> cat Exp_VanderWaerden_2-3-24_2010-10-12-214502_591/VanDerWaerden_2-3-24_592.cnf_OUT | extract_solution_ubcsat > Solution_n592
OKplatform> RunVdW3k 24 593 rots 1000 5000000 Solution_n592
   \endverbatim
   (this solution didn't help here). </li>
   <li> n=593:
    <ol>
     <li> Cutoff=5*10^6, rots:
     \verbatim
  1   2   3   4   5   6   7   8   9  10  11 
 19 165 260 275 142  64  35  23  11   5   1 
1000 
 1  2  3  4  5  6  7  8  9 10 
 4 45 73 84 35 16 11 12  6  4 
290 
    \endverbatim
     </li>
     <li> Cutoff=10^7, gsat-tabu:
     \verbatim
  1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17 
  9  65  73 202 232 216 126  42   7   7   4   3   3   8   1   1   1 
1000 
     \endverbatim
     (it seems clear that rots is better). </li>
     <li> Cutoff=10^7, rots:
     \verbatim
  1   2   3   4   5   6   7   8 
 13 148 181 124  29   3   1   1 
500
  1   2   3   4   5   6   7 
 14 155 163 129  29   7   3 
500 
     \endverbatim
     </li>
     <li> Cutoff=5*10^7, rots:
     \verbatim
 1  2  3 
16 67 17 
100 
 1  2  3 
14 73 13 
100 
     \endverbatim
     </li>
     <li> Let's consider this as unsatisfiable. </li>
    </ol>
   </li>
  </ul>


  \todo vanderwaerden_2(3,25) > 655
  <ul>
   <li> The (weak) conjecture is vanderwaerden_2(3,25) = 656. </li>
   <li> Search starting with n=590:
   \verbatim
OKplatform> RunVdW3k 25 590 gsat-tabu 200 50000000
   \endverbatim
   </li>
   <li> Perhaps also here the cutoff-value is too large:
   \verbatim
OKplatform> RunVdW3k 25 609 gsat-tabu 1000 10000000
   \endverbatim
   </li>
   <li> n=609 found satisfiable with certificate
   \verbatim
18,19,31,49,53,55,68,90,93,102,
106,123,129,132,138,143,160,164,175,176,
187,201,206,224,234,243,249,253,254,277,
278,286,287,290,315,317,324,328,335,351,
364,382,386,388,389,401,409,426,435,439,
456,462,465,471,476,493,497,508,509,520,
534,539,557,567,576,586,587
   \endverbatim
   </li>
   <li> n=622 found satisfiable with certificate
   \verbatim
7,12,18,27,37,60,74,82,85,86,
97,101,123,129,132,138,155,159,166,168,
185,196,205,206,208,212,229,230,243,259,
266,270,277,279,304,308,316,317,340,341,
345,351,360,370,393,407,415,418,419,430,
434,456,462,465,471,488,492,499,501,504,
518,539,541,545,562,575,576,599,603,610,
612
   \endverbatim
   </li>
   <li> Restarting the search, now using the solution found for n=622
   as initial assignment:
   \verbatim
OKplatform> cat Exp_VanderWaerden_2-3-25_2010-10-12-161927_609/VanDerWaerden_2-3-25_622.cnf_OUT | extract_solution_ubcsat > Solution_n622
OKplatform> RunVdW3k 25 623 rots 1000 5000000 Solution_n622
   \endverbatim
   </li>
   <li> n=654 found satisfiable with certificate
   \verbatim
1,16,19,39,48,56,76,85,90,92,
98,116,127,149,150,153,155,166,174,187,
192,196,203,211,223,227,241,246,264,266,
270,277,278,285,303,312,320,334,349,352,
357,371,372,381,389,409,418,423,425,433,
449,460,470,482,483,486,488,499,505,520,
525,529,536,544,556,560,574,579,597,599,
603,610,611,636,645
   \endverbatim
   A variation is
   \verbatim
16,19,38,39,48,56,76,85,90,92,
98,116,127,149,150,153,155,166,174,187,
192,196,203,211,223,227,241,246,264,266,
270,277,278,285,303,312,320,334,349,352,
357,371,372,381,389,409,418,423,425,431,
433,449,460,470,482,483,486,488,499,520,
525,529,536,544,556,560,574,579,597,599,
603,610,611,636
   \endverbatim
   </li>
   <li> n=655
    <ol>
     <li> Found satisfiable with certificate
     \verbatim
17,20,40,49,57,77,86,91,93,99,
117,128,150,151,154,156,167,175,188,193,
197,204,212,224,228,242,247,265,267,271,
278,279,286,304,313,321,335,350,353,358,
372,373,382,390,410,419,424,426,434,450,
461,471,483,484,487,489,500,506,521,526,
530,537,545,557,561,575,580,598,600,604,
611,612,637,646
     \endverbatim
     This is essentially different from the above solution. </li>
     <li> 1000 runs with rots and cutoff=5*10^6 don't find a solution:
     \verbatim
  1   2   3   4   5   6   7   8   9  10  11  12  13  14
216 309 149 112  17  21  32  43  40  34  18   6   1   2
1000
     \endverbatim
     </li>
     <li> Another 1000 runs with rots and cutoff=5*10^6 don't find a solution:
     \verbatim
  1   2   3   4   5   6   7   8   9  10  11  12
232 292 163 112  31  14  48  39  36  19   6   8
1000
     \endverbatim
     </li>
     <li> 500 runs with rots and cutoff=10^7 don't find a solution:
     \verbatim
  1   2   3   4   5   6   7   8  10  11 
191 183  62  35   5   5   6  10   2   1 
500 
     \endverbatim
     </li>
     <li> So n=655 seems to be quite difficult, and so unsatisfiability for
     n=656 is hard to judge. </li>
     <li> Rots with cutoff=20*10^7 found in run 3 a solution for n=654
     (seed=3127361270, osteps=7949973), and this solution worked also for
     n=655:
     \verbatim
2,17,20,40,49,57,77,86,91,93,
99,117,128,150,151,154,156,167,175,188,
193,197,204,212,224,228,242,247,265,267,
271,278,279,286,304,313,321,335,350,353,
358,372,373,382,390,410,419,424,426,434,
450,461,471,483,484,487,489,500,506,521,
526,530,537,545,557,561,575,580,598,600,
604,611,612,637,646
     \endverbatim
     </li>
     <li> A variation was found with rots, cutoff=10^7, as the solution for
     n=654:
     \verbatim
2,17,20,25,39,40,49,57,77,86,
91,93,99,117,128,150,151,154,156,167,
175,188,193,197,204,212,224,228,242,247,
265,267,271,278,279,286,304,313,321,335,
350,353,358,372,373,382,390,410,419,424,
426,434,450,461,471,483,484,487,489,500,
506,521,526,530,537,545,557,561,575,580,
598,600,604,611,612,637
     \endverbatim
     </li>
     <li> Are there essentially different solutions? </li>
    </ol>
   <li>
   <li> n=656
    <ol>
     <li> Cutoff=5*10^6, rots:
     \verbatim
  1   2   3   4   5   6   7   8   9  10  11  12
224 321 162  99  26  24  34  37  30  20  13  10
1000
     \endverbatim
     </li>
     <li> Cutoff=10^7 found in 500 runs no solution. Another 500 runs:
     \verbatim
  1   2   3   4   5   6   7   8   9
187 194  56  39   4   6   7   3   4
500
     \endverbatim
     </li>
    </ol>
   </li>
  </ul>


  \todo vanderwaerden_2(3,26) >= 727
  <ul>
   <li> The conjecture is vanderwaerden_2(3,26) = 727. </li>
   <li> Search starting with n=600:
   \verbatim
OKplatform> RunVdW3k 26 600 gsat-tabu 100 100000000
   \endverbatim
   </li>
   <li> Perhaps also here the cutoff-value is too large:
   \verbatim
OKplatform> RunVdW3k 26 655 gsat-tabu 1000 10000000
   \endverbatim
   </li>
   <li> n=659 found satisfiable with certificate
   \verbatim
13,26,27,46,51,64,80,91,113,116,
120,128,138,142,157,159,166,179,188,207,
215,224,237,244,246,265,275,283,290,311,
316,324,340,348,352,353,374,377,381,389,
399,418,420,427,440,449,457,468,476,485,
505,507,522,526,536,544,548,551,572,573,
585,607,614,627,651
   \endverbatim
   </li>
   <li> n=677 found satisfiable with certificate
   \verbatim
5,12,14,38,39,58,68,76,79,91,
105,117,123,125,128,132,150,162,163,179,
191,197,200,202,216,223,243,260,264,273,
274,280,301,302,308,311,334,339,345,347,
350,371,372,391,401,409,412,424,438,450,
456,458,461,465,483,486,495,496,512,520,
524,530,533,535,549,556,576,593,606,607,
613,634,635,641,644,667,672
   \endverbatim
   </li>
   <li> Restarting the experiment, now using the solution found for n=677
   as initial assignment:
   \verbatim
OKplatform> cat Exp_VanderWaerden_2-3-26_2010-10-12-161754_655/VanDerWaerden_2-3-26_677.cnf_OUT | extract_solution_ubcsat > Solution_n677
OKplatform> RunVdW3k 26 678 rots 1000 5000000 Solution_n677
   \endverbatim
   </li>
   <li> n=726:
    <ol>
     <li> Found satisfiable with certificate
     \verbatim
11,35,46,48,69,74,86,93,105,106,
109,115,122,128,152,157,159,167,185,202,
204,216,217,220,222,226,231,234,253,257,
263,278,291,308,313,333,342,344,345,350,
364,379,381,402,407,426,438,439,442,448,
455,461,485,490,492,500,516,518,535,537,
549,550,553,555,559,564,567,586,596,611,
624,641,646,666,675,678,697,701
     \endverbatim
     </li>
     <li> rots with cutoff=10^7 found another solution in run 27:
     \verbatim
26,31,49,51,68,82,86,99,120,125,
144,156,157,160,162,173,174,193,197,203,
208,211,218,231,253,268,277,282,284,285,
290,314,319,321,329,347,364,366,378,382,
384,388,393,401,415,419,432,453,458,470,
477,489,490,493,495,506,507,526,530,536,
541,551,564,581,586,601,610,615,617,618,
623,647,652,654,680,699,711,721,726
     \endverbatim
     </li>
     <li> rots with cutoff=10^7 found another solution, caried over from n=725,
     very close to the above first solution:
     \verbatim
17,35,46,48,69,74,86,93,106,109,
115,122,128,152,157,159,167,185,202,204,
216,217,220,222,226,231,234,253,257,263,
278,291,308,313,333,342,344,345,350,364,
379,381,402,407,419,426,438,439,442,448,
455,461,485,490,492,500,516,518,535,537,
549,550,553,555,559,564,567,586,596,611,
624,641,646,666,675,678,697,701
     \endverbatim
     </li>
     <li> A different solution was found by rots with cutoff=2*10^7, caried
     over from n=725:
     \verbatim
1,6,16,28,47,73,75,80,104,109,
110,112,117,126,141,146,163,176,186,191,
197,201,220,221,234,237,238,250,257,269,
274,295,308,312,326,334,339,343,345,349,
361,363,380,398,406,408,413,437,442,443,
445,450,459,474,496,509,516,519,524,530,
534,553,554,565,567,570,571,583,602,607,
628,641,645,659,676,678,696,701
     \endverbatim
     </li>
    </ol>
   </li>
   <li> n=727
    <ol>
     <li> rots, cutoff=5*10^6:
     \verbatim
  1   2   3   4   5   6   7   8   9  10  11  12  13  14  15
372 336  83   9   3   2   2  13  30  36  45  39  25   4   1
1000
     \endverbatim
     </li>
     <li> gsat-tabu, cutoff=10*10^6:
     \verbatim
  1   2   3   4   6   7   8   9  10  11  12  13  14  15  16  17  18  19  20  21
 13  25  21   6   2   5  36  92 208 184 124  67  43  49  32  19  25  15   8   4
 22  23  24  25  26  27  28  29  30  33
  8   3   2   2   1   1   1   1   1   2
1000
     \endverbatim
     (so it seems rots is definitely better). </li>
     <li> rots, cutoff=10^7:
     \verbatim
  1   2   3   4   8   9  10  11
284 171  18   2   4  15   3   3
500
  1   2   3   5   7   8   9  10  11  12
286 183  10   3   2   2   5   3   2   4
500
  1   2   3   7   8   9  10  11  12
281 176  19   1   3   6   5   4   5
500
     \endverbatim
     </li>
    </ol>
   </li>
  </ul>


  \todo vanderwaerden_2(3,27) >= 770
  <ul>
   <li> The conjecture is vanderwaerden_2(3,27) = 770. </li>
   <li> Search starting with n=610:
   \verbatim
OKplatform> RunVdW3k 27 610 gsat-tabu 100 100000000
   \endverbatim
   </li>
   <li> Perhaps also here the cutoff-value is too large:
   \verbatim
OKplatform> RunVdW3k 27 678 gsat-tabu 1000 10000000
   \endverbatim
   </li>
   <li> n=682 found satisfiable with certificate
    \verbatim
10,25,42,53,58,68,71,86,102,115,
116,129,139,145,158,182,184,197,198,202,
213,227,231,240,247,269,276,284,290,313,
314,319,327,334,361,363,372,377,387,405,
406,419,445,448,458,459,463,474,488,492,
501,506,508,530,537,545,551,574,588,595,
617,619,622,633,648,666
    \endverbatim
   </li>
   <li> n=725 found satisfiable with certificate
    \verbatim
18,23,36,55,57,75,88,98,110,117,
125,129,134,149,155,166,168,171,172,186,
191,192,194,199,205,231,236,258,282,283,
305,316,320,332,334,339,342,351,369,377,
388,408,413,414,421,431,443,450,458,462,
467,488,499,501,504,505,525,527,532,538,
556,564,569,591,598,612,615,616,638,649,
653,665,672,675,689,710
    \endverbatim
   </li>
   <li> Restarting the search, now using the solution found for n=725
   as initial assignment:
   \verbatim
OKplatform> cat Exp_VanderWaerden_2-3-27_2010-10-12-161406_678/VanDerWaerden_2-3-27_725.cnf_OUT | extract_solution_ubcsat > Solution_n725
OKplatform> RunVdW3k 27 726 rots 1000 5000000 Solution_n725
   \endverbatim
   </li>
   <li> n=769:
    <ol>
     <li> Found satisfiable with certificate
      \verbatim
25,29,35,54,72,74,77,99,103,111,
114,135,136,149,165,176,188,192,202,209,
223,246,250,251,260,266,287,294,296,313,
321,325,331,333,334,336,358,361,362,377,
387,405,407,410,414,432,436,442,444,447,
468,469,482,498,521,525,535,542,556,579,
583,584,593,599,620,629,646,654,658,664,
666,667,669,694,701,710,731,747,764
      \endverbatim
     </li>
     <li> 1000 runs with rots and cutoff=5000000 found 4 solutions, from which
     (only) one is reproducible (due to a bug in Ubcsat, now resolved):
     \verbatim
  0   1   2   3   4   5   6   7   9  11  12  13  14  15  16  17  18  19
  4 473 315  71   7   4   2   1   2   2   8  13  15  32  26  14   9   2
1000
> E[E$sat==1,]
    sat min  osteps  msteps       seed
107   1   0 2782015 2782015 2990676734
435   1   0 1430527 1430527 1209355894
467   1   0 3552707 3552707 3160209839
656   1   0 3273460 3273460  813836620

> ubcsat-okl -alg rots -cutoff 1430527 -seed 1209355894 -i Exp_VanderWaerden_2-3-27_2010-10-13-184057_726_rots-1000-5000000/VanDerWaerden_2-3-27_769.cnf -solve | tee VanDerWaerden_2-3-27_769.cnf_OUT3
> ExtractCertificate_vdW2 VanDerWaerden_2-3-27_769.cnf_OUT3
6,23,39,60,69,76,101,103,104,106,
112,116,124,141,150,171,177,186,187,191,
214,228,235,245,249,272,288,301,302,323,
326,328,334,338,339,356,360,363,365,383,
393,408,409,412,434,436,437,439,445,449,
457,474,476,483,504,510,519,520,524,547,
561,568,578,582,594,605,621,634,635,656,
659,667,671,693,696,698,716,741,745
     \endverbatim
     </li>
     <li> Cutoff=10^6, rots:
     \verbatim
 1  2  3  4  5  6  9 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 
26 58 51 12  6  6  1  2  3  9  9 21 27 38 50 57 30 35 32 18  6  3 
500 
     \endverbatim
     </li>
     <li> Cutoff=10^7, rots, finds a solution in run 209, the same as last one
     above, but without vertex 339. </li>
    </ol>
   </li>
   <li> n=770:
    <ol>
     <li> Cutoff=5*10^6, rots:
     \verbatim
  1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18  19  20
451 349  80   9   2   1   1   1   1   1   4   4  11  13  27  17  13   9   3   1
 21  22
  1   1
1000
  1   2   3   4   5   6   8   9  12  13  14  15  16  17  18  19
452 361  72   8   2   1   1   1   7   4  18  31  11  16  13   2
1000
     \endverbatim
     </li>
     <li> Cutoff=10^7, rots:
     \verbatim
  1   2   3  14  15  16
354 138   5   1   1   1
500
  1   2   3   4  10  13  14  15  17 
369 117   5   1   1   2   3   1   1 
500 
     \endverbatim
     </li>
    </ol>
   </li>
  </ul>


  \todo vanderwaerden_2(3,28) > 826
  <ul>
   <li> The (weak) conjecture is vanderwaerden_2(3,28) = 827. </li>
   <li> Search starting with n=620:
   \verbatim
OKplatform> RunVdW3k 28 620 gsat-tabu 1000 10000000
   \endverbatim
   </li>
   <li> n=733 found satisfiable (seed=1020588263, osteps=2293839, run=1),
   with solution
   \verbatim
22,40,49,59,72,75,77,81,88,89,
94,108,112,118,123,133,146,151,168,170,
182,183,200,205,219,234,236,257,271,274,
293,297,303,310,334,336,340,347,355,382,
385,390,392,405,408,414,421,422,427,441,
445,447,451,456,466,479,484,501,503,515,
516,533,538,552,569,590,604,607,626,630,
636,643,667,669,673,680,706,718,723
   \endverbatim
   </li>
   <li> n=742 found satisfiable (seed=1162005655, osteps=1152687, run=3),
   with solution
   \verbatim
12,22,26,41,65,74,75,78,103,111,
112,116,123,137,140,152,170,174,176,179,
185,186,189,190,213,214,216,222,223,244,
251,271,281,285,290,301,324,327,338,345,
355,382,398,408,411,436,444,448,449,456,
470,473,481,503,507,509,512,523,546,547,
549,555,556,559,577,584,596,614,618,623,
634,657,660,667,671,692,715
   \endverbatim
   </li>
   <li> n=750 found satisfiable (seed=2611554683, osteps=5411045, run=5),
   with certificate
   \verbatim
10,36,47,56,59,85,93,110,118,128,
133,137,159,165,170,191,192,205,211,221,
244,258,265,269,270,276,302,303,306,307,
315,322,340,350,359,369,376,380,390,417,
418,426,443,451,461,463,464,466,470,492,
501,503,524,525,544,554,577,591,598,603,
609,635,636,639,640,648,649,673,676,683,
686,692,709,720,722,725
   \endverbatim
   </li>
   <li> Now with the new version of RunVdW3k, which uses the old solution as
   initial assignment, and with rots:
   \verbatim
OKplatform> RunVdW3k 28 750 rots 1000 50000000
   \endverbatim
   </li>
   <li> n=763 found satisfiable with certificate
   \verbatim
4,24,29,41,42,56,61,62,79,99,
106,110,112,115,130,135,138,152,173,180,
184,186,210,217,223,239,249,264,278,297,
301,323,332,337,350,352,374,375,387,389,
395,402,406,412,431,439,443,448,463,468,
471,485,498,506,513,517,519,543,545,550,
556,572,582,597,611,616,634,654,656,665,
667,683,685,690,704,720,735,739
   \endverbatim
   </li>
   <li> n=800 found satisfiable with certificate
   \verbatim
10,34,47,68,72,92,94,99,109,121,
140,166,168,173,197,202,203,205,219,234,
239,256,269,284,290,294,313,321,325,327,
330,331,343,345,350,362,380,388,390,401,
419,427,432,436,438,442,454,473,499,501,
506,530,535,536,538,552,567,589,602,617,
623,627,646,654,658,660,663,664,678,683,
695,700,721,738,752,769,771,794
   \endverbatim
   </li>
   <li> n=826:
    <ol>
     <li> Found satisfiable with certificate
     \verbatim
28,39,62,64,81,95,112,133,138,155,
157,169,170,173,175,179,186,187,206,210,
216,231,244,266,281,295,297,298,303,327,
332,334,360,379,391,395,397,401,406,414,
432,443,445,453,471,483,488,502,503,506,
508,512,520,539,543,549,564,577,594,599,
614,628,630,631,650,660,665,667,693,712,
724,734,739,741,765,776,786,799,816
     \endverbatim
     </li>
     <li> 1000 runs with rots and cutoff=5*10^6 found 2 solutions:
     \verbatim
  0   1   2   3   4   5   6   7   8  10  11  13  14  15  16  17  18  20 
  2  31 530 272  36   7   1   1   2   1   1  16  18  32  32  15   2   1 
1000 
    sat min  osteps  msteps       seed
400   1   0 3527090 3527090  249146123
751   1   0 4364843 4364843 2199943925
     \endverbatim
     Again none of the solution could be reproduced (due to the Ubcsat bug).
     </li>
     <li> With small modifications from a solution for n=825 a solution for
     n=826 was produced, the same as above but with vertex 823 added. </li>
    </ol>
   </li>
   <li> n=827:
    <ol>
     <li> cutoff=5*10^6, rots:
     \verbatim
  1   2   3   4   5   6   7   9  10  12  13  14  15  16  17  18  19 
 29 502 313  44   7   3   2   1   1   5  10  24  26  18   7   7   1 
1000 
     \endverbatim
     </li>
     <li> cutoff=10^7, rots:
     \verbatim
  1   2   3   4  13  14  15 
 58 763 151  14   4   7   3 
1000 
     \endverbatim
     </li>
     <li> cutoff=5*10^7, rots:
     \verbatim
 1  2 
 7 14 
21 
     \endverbatim
     </li>
    </ol>
   </li>
  </ul>


  \todo vanderwaerden_2(3,29) > 867
  <ul>
   <li> The (weak) conjecture is vanderwaerden_2(3,29) = 868. </li>
   <li> Search starting with n=620:
   \verbatim
OKplatform> RunVdW3k 29 750 rots 1000 5000000
   \endverbatim
   </li>
   <li> n=808 found satisfiable with certificate
   \verbatim
7,30,50,61,87,103,104,123,126,135,
155,161,172,174,190,198,205,214,227,229,
242,248,271,285,288,301,311,316,338,345,
348,353,359,363,377,394,399,400,414,420,
422,431,456,457,474,488,493,505,507,511,
531,538,542,547,568,575,581,604,618,621,
634,636,644,649,658,671,678,681,686,692,
710,732,733,745,755,764,782,792
   \endverbatim
   </li>
   <li> n=852 found satisfiable with certificate
   \verbatim
18,37,59,64,75,92,98,119,136,144,
145,149,173,181,186,207,222,240,246,256,
259,260,277,281,286,292,304,318,320,323,
330,333,341,358,366,395,397,408,415,431,
434,441,452,477,478,482,499,506,514,519,
540,555,573,579,589,592,593,610,614,619,
625,637,651,653,656,663,666,674,691,699,
725,728,748,767,774,795,801,802,814,832
   \endverbatim
   </li>
   <li> n=854 found satisfiable with certificate
   \verbatim
26,39,61,66,77,94,100,121,131,138,
146,147,151,175,183,188,209,224,242,248,
258,261,262,279,283,288,294,306,320,322,
325,332,335,343,360,368,397,399,410,417,
433,436,443,454,471,479,480,484,501,508,
516,521,542,557,575,581,591,594,595,612,
616,621,627,639,653,655,658,665,668,676,
693,701,727,730,750,769,776,797,803,816,
834
   \endverbatim
   </li>
   <li> n=859 found satisfiable with certificate
   \verbatim
28,35,61,74,81,98,102,111,128,136,
139,165,183,186,188,209,214,222,229,239,
247,257,260,262,276,283,294,296,297,303,
321,324,325,340,361,368,371,373,394,399,
410,414,431,432,435,461,472,498,510,516,
519,521,542,547,555,572,590,593,595,609,
616,627,629,630,636,654,657,658,666,673,
691,704,706,732,740,743,765,784,802,821,
843
   \endverbatim
   </li>
   <li> n=867:
    <ol>
     <li> Found satisfiable with certificate
     \verbatim
16,38,57,75,94,116,119,127,153,155,
168,186,193,201,202,205,223,229,230,232,
243,250,264,266,269,287,304,312,317,338,
340,343,349,361,387,398,424,427,428,445,
449,460,465,486,488,491,498,519,534,535,
538,556,562,563,565,576,583,597,599,602,
612,620,630,637,645,650,671,673,676,694,
720,723,731,757,761,778,785,798,824,831,
859
     \endverbatim
     </li>
    </ol>
   </li>
   <li> n=868:
    <ol>
     <li> cutoff=5*10^6, rots:
     \verbatim
  1   2   3   4   5   6   7   8  10  11  12  13  14  15  16  17  18  19  20 
 21 336 294  67  13   7   2   2   3   9  18  27  50  46  55  37   9   2   2 
1000 
     \endverbatim
     </li>
     <li> cutoff=10^7, rots:
     \verbatim
  1   2   3   4   5   6   7   9  11  12  13  14  15  16  17  18 
 67 609 210  24   9   2   2   1   3  13  13  21  15   8   2   1 
1000 
     \endverbatim
     </li>
    </ol>
   </li>
  </ul>


  \todo vanderwaerden_2(3,30) > 901
  <ul>
   <li> The predictions are 948, 954. </li>
   <li> The conjecture is vanderwaerden_2(3,29) = ???. </li>
   <li> Search starting with n=852:
   \verbatim
OKplatform> RunVdW3k 30 852 rots 1000 5000000
   \endverbatim
   </li>
   <li> n=883 found satisfiable with certificate
   \verbatim
12,38,45,66,75,88,90,108,125,127,
140,149,154,157,182,186,193,219,228,230,
250,251,273,287,288,291,304,310,312,342,
349,361,365,367,371,379,398,402,404,408,
423,450,453,460,473,476,482,489,519,524,
526,547,561,583,584,600,606,620,624,635,
637,643,645,663,675,682,694,698,712,731,
732,735,737,754,769,774,783,786,809,823,
828,848,857
   \endverbatim
   </li>
   <li> n=889 found satisfiable with certificate
   \verbatim
30,31,45,71,84,85,92,108,110,127,
156,158,159,178,179,188,193,201,203,216,
240,262,263,270,289,290,293,295,299,304,
332,344,349,351,363,373,380,404,406,417,
423,438,443,460,474,491,511,521,526,528,
534,536,552,566,573,589,595,600,622,632,
647,663,665,677,682,684,706,711,714,734,
739,756,785,799,807,808,825,848,850,861
   \endverbatim
   </li>
   <li> n=891 found satisfiable with certificate
   \verbatim
2,9,27,47,58,78,91,113,120,128,
134,137,152,157,174,195,197,206,212,232,
248,249,261,268,271,280,282,300,313,317,
319,335,350,360,382,391,393,396,419,424,
434,446,453,454,483,485,490,507,528,541,
545,559,565,581,582,596,601,604,613,615,
639,646,650,675,678,683,692,715,726,729,
742,744,761,767,781,787,794,798,816,831,
855,863,874
   \endverbatim
   </li>
   <li> n=894 found satisfiable with certificate
   \verbatim
30,47,60,75,79,105,112,118,121,134,
158,159,173,178,190,210,216,227,245,252,
253,255,266,270,282,284,289,306,321,334,
338,340,344,363,364,371,377,393,403,412,
438,445,451,455,477,482,506,511,514,523,
528,543,549,560,585,586,588,602,603,617,
622,639,654,671,673,677,689,697,704,710,
736,741,763,787,788,800,810,821,824,825,
847,858,874
   \endverbatim
   </li>
   <li> n=897 found satisfiable with certificate
   \verbatim
23,25,33,42,62,75,97,104,112,121,
136,141,158,173,179,181,190,196,216,232,
233,252,255,264,266,290,297,301,303,319,
334,344,366,375,377,380,403,408,418,430,
437,438,467,469,474,491,512,525,529,543,
549,565,566,580,585,588,597,623,630,634,
636,659,662,667,676,677,689,710,713,726,
728,745,751,771,778,782,800,815,821,839,
847,858,884
   \endverbatim
   </li>
   <li> n=898:
    <ol>
     <li> rots with cutoff=5*10^6 didn't find a solution:
     \verbatim
  1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17
 37 222 170  58  13   8   4   8  16  40  92 120 121  61  24   5   1
1000
     \endverbatim
     </li>
     <li> cutoff=10^7 found a solution, with certificate
     \verbatim
9,34,54,65,85,91,98,120,127,135,
141,144,159,164,181,202,204,213,219,239,
255,256,268,275,278,287,289,307,320,324,
326,342,349,357,367,389,398,400,403,426,
431,441,453,460,461,490,492,497,514,535,
548,552,566,572,588,589,603,608,611,620,
622,646,653,657,682,685,690,699,722,733,
736,749,751,768,774,788,794,801,805,823,
838,862,870,881
     \endverbatim
     </li>
    </ol>
   </li>
   <li> n=901: rots with cutoff=10^7 found a solution (in run 316, for n=898,
   with small changes then) with certificate
   \verbatim
24,47,61,77,84,102,122,124,133,135,
158,170,175,176,190,195,225,233,240,264,
270,272,277,281,287,299,318,323,324,346,
351,360,381,383,388,412,417,434,447,455,
457,484,486,505,509,523,528,540,558,566,
579,583,597,602,610,614,616,619,620,647,
651,656,679,706,713,714,716,721,745,758,
764,767,768,780,805,819,827,836,838,841,
864,873
   \endverbatim
   </li>
   <li> n=902:
    <ol>
     <li> rots found no solution in 500 runs with cutoff=10^7:
     \verbatim
  1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18
 25 175 148  32  11   6   1   1   2   1   2  14  30  27  17   3   4   1
500
     \endverbatim
     </li>
    </ol>
   </li>
  </ul>

*/

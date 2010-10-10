// Oliver Kullmann, 10.4.2009 (Swansea)
/* Copyright 2009, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/VanderWaerden_2-3-k.hpp
  \brief On investigations into vdW-numbers vdw_2(3,k)

  Instances created by output_vanderwaerden2nd_stdname(3,k,n).
  An experiment is run by "RunVdW3k k n0 alg runs cutoff".

  
  \todo Literature overview
  <ul>
   <li> A conjecture is vdw_2(3,k) <= k^2 for k >= 3. </li>
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
   <li> Using the conjectured values 349, 389, 416, 464, 516 (see below):
   \verbatim
d = append(d0, c(349, 389, 416, 464, 516))
plot(d)
lines(d)
lines((1:length(d))^2)
x = log((1:length(d)))
y = log(d)
L = lm(y ~ x)
plot(x,y)
lines(x,predict(L))
   \endverbatim
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
a  0.87855    0.03864   22.74 2.84e-16 ***
b  2.03024    0.01483  136.89  < 2e-16 ***
Residual standard error: 3.362 on 21 degrees of freedom

Cnl = coefficients(NL)
Cnl
        a         b 
0.8785468 2.0302359 
fnl = function(k){Cnl[1] * k^Cnl[2]}
round(fnl(1:25))
 1 4 8 15 23 33 46 60 76 94
 114 136 160 186 215 245 277 311 347 385
 425 467 511 557 605

d - round(fnl(1:23))
 2 2 1 3 -1 -1 0 -2 1 3
 0 -1 0 0 3 -7 2 1 2 4
 -9 -3  5
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
a  3.23998    2.29550   1.411   0.1735    
b -0.92261    0.44066  -2.094   0.0492 *  
c  1.00003    0.01783  56.095   <2e-16 ***
Residual standard error: 3.355 on 20 degrees of freedom

Cnlq = coefficients(NLq)
Cnlq
         a          b          c 
 3.2399774 -0.9226143  1.0000282 
fnlq = function(k){Cnlq[1] + Cnlq[2]*k +Cnlq[3] * k^2}
round(fnlq(1:25))
 3 5 9 16 24 34 46 60 76 94
 114 136 160 186 214 244 277 311 347 385
 425 467 511 557 605

d - round(fnlq(X))
 0 1 0 2 -2 -2 0 -2 1 3
 0 -1 0 0 4 -6 2 1 2 4
 -9 -3  5
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


  \todo vanderwaerden_2(3,20) >= 389
  <ul>
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
    </ol>
   </li>
   <li> n=388
    <ol>
     <li> rnovelty with cutoff=8*10^6: found a solution in run 33
     (seed=2441787444, osteps=4412722). </li>
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
   <li> n=415, gsat-tabu, cutoff=10^8: in run 3 a solution was found
   (seed=174271267, osteps=91827849). Further statistics:
   \verbatim
 0   1  2 3 4 5
17 147 28 5 2 1
200
   \endverbatim
   </li>
   <li> n=416, gsat-tabu
    <ol>
     <li> cutoff=10^7:
     \verbatim
  1   2   3   4   5
405 572  20   2   1
1000
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
   <li> The prediction is vanderwaerden_2(3,22) = 465. </li>
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
   <li> n=462, gsat-tabu, cutoff=5*10^7:
   \verbatim
 1  2  3
81 17  2
100
   \endverbatim
   In further 313 runs one solution was found (seed=270943421,
   osteps=11236258). </li>
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
   <li> The predictions are vanderwaerden_2(3,23) in {508,509}. </li>
   <li> Experience with k=20 is that gsat-tabu is best, however we should
   have a look again --- this might change with different k's or with
   different cutoff's. </li>
   <li> n=500, gsat-tabu, cutoff=10^8: easily found a solution. </li>
   <li> n=510, gsat-tabu, cutoff=10^8: a solution was found in the first
   run (seed=1387406527, osteps=43902628). </li>
   <li> n=511, gsat-tabu, cutoff=10^8: in 31 runs two solutions were found
   (seed=2372840954, osteps=16960676). </li>
   <li> n=512, gsat-tabu, cutoff=10^8: in 75 runs one solution was found
   (seed=216008612, osteps=49163297). </li>
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


  \todo vanderwaerden_2(3,24) > 560
  <ul>
   <li> The conjecture is vanderwaerden_2(3,24) = ???. </li>
   <li> The predictions are vanderwaerden_2(3,24) = 557. </li>
   <li> Experience with k=20 is that gsat-tabu is best, however we should
   have a look again --- this might change with different k's or with
   different cutoff's. </li>
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
   <li> n=557, gsat-tabu, cutoff=10^8: found satisfiable (seed=1050785257).
   </li>
   <li> n=558, gsat-tabu, cutoff=10^8: found satisfiable (seed=3904367479).
   </li>
   <li> n=559, gsat-tabu, cutoff=10^8: found satisfiable (seed=1689825937).
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
   <li> Running experiment starting with n=561:
   \verbatim
OKplatform> RunVdW3k 24 561 gsat-tabu 100 100000000
   \endverbatim
   </li>
  </ul>


  \todo vanderwaerden_2(3,25)
  <ul>
   <li> The conjecture is vanderwaerden_2(3,25) = ???. </li>
   <li> The predictions are vanderwaerden_2(3,25) = 605. </li>
   <li> Running experiment starting with n=590:
   \verbatim
OKplatform> RunVdW3k 25 590 gsat-tabu 100 100000000
   \endverbatim
   </li>
  </ul>

*/


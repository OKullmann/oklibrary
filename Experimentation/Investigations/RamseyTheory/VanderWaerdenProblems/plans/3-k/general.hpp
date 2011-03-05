// Oliver Kullmann, 10.4.2009 (Swansea)
/* Copyright 2009, 2010, 2011 Oliver Kullmann
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
  An ubcsat-experiment is run by "RunPdVdW3k k n0 alg runs cutoff".
  And running a complete solver by "CRunPdVdW3k k solver".


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
   <li> k=12, n=134, 135:
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
c running_time(sec)                     593.2
c number_of_nodes                       283568
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                2060055
c number_of_pure_literals               35
c number_of_autarkies                   1
c number_of_missed_single_nodes         0
c max_tree_depth                        36
c number_of_table_enlargements          0
c number_of_1-autarkies                 516
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             VanDerWaerden_2-3-12_134.cnf

same machine csltok (Intel i5, 2.4GHz), but only with 2 processes altogether:
329.4s

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
c running_time(sec)                     636.0
c number_of_nodes                       281381
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                2049274
c number_of_pure_literals               29
c number_of_autarkies                   0
c number_of_missed_single_nodes         0
c max_tree_depth                        36
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
     <li> k=12, n=134: 214s (NB_BRANCHE= 240158, csltok) </li>
     <li> k=12, n=135: 236s (NB_BRANCHE= 262304, csltok) </li>
    </ol>
   </li>
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
     <li> k=12, n=134: 153s (3605914 conflicts; csltok) </li>
     <li> k=12, n=135: 266s (5963349 conflicts; csltok (higher load)) </li>
     <li> k=13, n=159: 33s (701558 conflicts; csltok) </li>
     <li> k=13, n=160: 4871s (63901998 conflicts; csltok) </li>
    </ol>
   </li>
   <li> cryptominisat-2.9.0:
    <ol>
     <li> k=12, n=134: 91s (619145 conflicts; csltok) </li>
     <li> k=12, n=135: 763s (2815643 conflicts; csltok) </li>
    </ol>
   </li>
   <li> precosat236:
    <ol>
     <li> k=12, n=134: 159s (1145491 conflicts; csltok) </li>
     <li> k=12, n=135: 744s (3583785 conflicts; csltok) </li>
    </ol>
   </li>
   <li> precosat-570:
    <ol>
     <li> k=12, n=134: 278s (1531799 conflicts; csltok) </li>
     <li> k=12, n=135: 526s (2425722 conflicts; csltok) </li>
    </ol>
   </li>
   <li> glucose:
    <ol>
     <li> k=12, n=134: 39s (425399 conflicts; csltok) </li>
     <li> k=12, n=135: 191s (1356325 conflicts; csltok) </li>
     <li> k=13, n=159: 139s (957255 conflicts; csltok) </li>
     <li> k=13, n=160: 3274s (9907932 conflicts; csltok) </li>
    </ol>
   </li>
   <li> minisat-2.2.0 and glucose seem best (for the conflict-driven solvers,
   while satz215 seems best overall). </li>
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

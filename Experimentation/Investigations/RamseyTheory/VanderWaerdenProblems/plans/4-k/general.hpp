// Oliver Kullmann, 5.3.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/4-k/general.hpp
  \brief On investigations into vdW-numbers vdw_2(4,k)

  Instances created by output_vanderwaerden2nd_stdname(4,k,n) at Maxima-level,
  or by "VanderWaerden-O3-DNDEBUG 4 k n" at C++ level (instances differ by
  the order of negated literals in clauses).
  An ubcsat-experiment is run by "RunVdWk1k2 4 k n0 alg runs cutoff".

  The palindromic versions are created by
  output_pd_vanderwaerden2nd_stdname(4,k,n) at Maxima-level, or by
  "PdVanderWaerdenCNF-O3-DNDEBUG 4 k n" at C++ level (instances differ by
  the order of negated literals in clauses).
  An ubcsat-experiment is run by "RunPdVdWk1k2 4 k n0 alg runs cutoff".
  And running a complete solver by "CRunPdVdWk1k2 4 k solver".


  \todo Literature overview
  <ul>
   <li> Apparently nothing is known about vdw_2(4,k) ? OK has conjectureed
   that it's polynomially bounded. </li>
   <li> The known values:
   \verbatim
create_list(vanderwaerden4k(k),k,1,9);
 [4,7,18,35,55,73,109,146,unknown]
   \endverbatim
   </li>
   <li> Via R we get the prediction f(k) ~ 1.846715 * k^2.09233 when
   excluding the first 2 points:
   \verbatim
d = c(4,7,18,35,55,73,109,146)
plot(d)
x = log((1:length(d))[-(1:2)])
y = log(d[-(1:2)])
plot(x,y)
L = lm(y ~ x)
summary(L)
Coefficients:
            Estimate Std. Error t value Pr(>|t|)
(Intercept)  0.61341    0.09491   6.463  0.00295 **
x            2.09233    0.05632  37.153 3.13e-06 ***
lines(x,predict(L))
C = coefficients(L)
f = function(k){exp(C[1]) * k^C[2]}
plot(d)
lines(f(1:length(d)))
f(1:10)
 [1]   1.846715   7.875051  18.394758  33.582025  53.564174  78.441799
 [7] 108.298437 143.205718 183.226470 228.416720
   \endverbatim
   </li>
   <li> Including the conjecture for k=9:
   \verbatim
d = c(4,7,18,35,55,73,109,146,309)
plot(d)
x = log((1:length(d))[-(1:2)])
y = log(d[-(1:2)])
plot(x,y)
L = lm(y ~ x)
summary(L)
Coefficients:
            Estimate Std. Error t value Pr(>|t|)
(Intercept)   0.2298     0.3471   0.662    0.537
x             2.3572     0.1964  12.002 7.08e-05 ***
Residual standard error: 0.1886 on 5 degrees of freedom
Multiple R-squared: 0.9665,     Adjusted R-squared: 0.9597
F-statistic:   144 on 1 and 5 DF,  p-value: 7.085e-05
lines(x,predict(L))
C = coefficients(L)
f = function(k){exp(C[1]) * k^C[2]}
plot(d)
lines(f(1:length(d)))
round(f(1:10))
  1   6  17  33  56  86 124 169 223 286
round(f(1:10)) - d
  -3  -1  -1  -2   1  13  15  23 -86
   \endverbatim
   This model is not very good (the outlier for k=9 is hard to handle).
   </li>
  </ul>


  \todo Why so hard?
  <ul>
   <li> The instances for vdw_2(4,k) seem to be really hard, for all solver
   types. Why is this? </li>
   <li> While the palindromic problems are rather easy (relatively). Why?
   </li>
   <li> Can this be exploited? Perhaps not, since the palindromic case is
   really very special, and adding a non-palindromic constraint wouldn't
   help much. </li>
  </ul>


  \todo Performance of OKsolver-2002
  <ul>
   <li> The minisat2-preprocessing has no effect here. </li>
   <li> vdw_2(4,4) = 35
   \verbatim
> OKsolver_2002-O3-DNDEBUG VanDerWaerden_2-4-4_34.cnf
s SATISFIABLE
c sat_status                            1
c initial_maximal_clause_length         4
c initial_number_of_variables           34
c initial_number_of_clauses             352
c initial_number_of_literal_occurrences 1408
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   0
c running_time(sec)                     0.0
c number_of_nodes                       7
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                4
c number_of_pure_literals               3
c number_of_autarkies                   0
c number_of_missed_single_nodes         0
c max_tree_depth                        6
c number_of_table_enlargements          0
c number_of_1-autarkies                 8
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             VanDerWaerden_2-4-4_34.cnf
> OKsolver_2002-O3-DNDEBUG VanDerWaerden_2-4-4_35.cnf
s UNSATISFIABLE
c sat_status                            0
c initial_maximal_clause_length         4
c initial_number_of_variables           35
c initial_number_of_clauses             374
c initial_number_of_literal_occurrences 1496
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   0
c running_time(sec)                     0.0
c number_of_nodes                       71
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                142
c number_of_pure_literals               0
c number_of_autarkies                   0
c number_of_missed_single_nodes         0
c max_tree_depth                        6
c number_of_table_enlargements          0
c number_of_1-autarkies                 0
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             VanDerWaerden_2-4-4_35.cnf
   \endverbatim
   </li>
   <li> vdw_2(4,5) = 55
   \verbatim
> OKsolver_2002-O3-DNDEBUG VanDerWaerden_2-4-5_54.cnf
s SATISFIABLE
c sat_status                            1
c initial_maximal_clause_length         5
c initial_number_of_variables           54
c initial_number_of_clauses             797
c initial_number_of_literal_occurrences 3526
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   0
c running_time(sec)                     0.0
c number_of_nodes                       313
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                853
c number_of_pure_literals               0
c number_of_autarkies                   0
c number_of_missed_single_nodes         0
c max_tree_depth                        12
c number_of_table_enlargements          0
c number_of_1-autarkies                 0
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             VanDerWaerden_2-4-5_54.cnf
> OKsolver_2002-O3-DNDEBUG VanDerWaerden_2-4-5_55.cnf
s UNSATISFIABLE
c sat_status                            0
c initial_maximal_clause_length         5
c initial_number_of_variables           55
c initial_number_of_clauses             828
c initial_number_of_literal_occurrences 3663
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   0
c running_time(sec)                     0.1
c number_of_nodes                       683
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                1924
c number_of_pure_literals               0
c number_of_autarkies                   0
c number_of_missed_single_nodes         0
c max_tree_depth                        12
c number_of_table_enlargements          0
c number_of_1-autarkies                 0
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             VanDerWaerden_2-4-5_55.cnf
   \endverbatim
   </li>
   <li> vdw_2(4,6) = 73
   \verbatim
> OKsolver_2002-O3-DNDEBUG VanDerWaerden_2-4-6_72.cnf
s SATISFIABLE
c sat_status                            1
c initial_maximal_clause_length         6
c initial_number_of_variables           72
c initial_number_of_clauses             1311
c initial_number_of_literal_occurrences 6210
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   0
c running_time(sec)                     0.4
c number_of_nodes                       2842
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                10236
c number_of_pure_literals               0
c number_of_autarkies                   0
c number_of_missed_single_nodes         0
c max_tree_depth                        21
c number_of_table_enlargements          0
c number_of_1-autarkies                 0
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             VanDerWaerden_2-4-6_72.cnf
> OKsolver_2002-O3-DNDEBUG VanDerWaerden_2-4-6_73.cnf
s UNSATISFIABLE
c sat_status                            0
c initial_maximal_clause_length         6
c initial_number_of_variables           73
c initial_number_of_clauses             1349
c initial_number_of_literal_occurrences 6390
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   0
c running_time(sec)                     3.5
c number_of_nodes                       21661
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                78198
c number_of_pure_literals               0
c number_of_autarkies                   0
c number_of_missed_single_nodes         0
c max_tree_depth                        21
c number_of_table_enlargements          0
c number_of_1-autarkies                 3
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             VanDerWaerden_2-4-6_73.cnf
   \endverbatim
   </li>
   <li> vdw_2(4,7) = 109
   \verbatim
> OKsolver_2002-O3-DNDEBUG VanDerWaerden_2-4-7_108.cnf
s SATISFIABLE
c sat_status                            1
c initial_maximal_clause_length         7
c initial_number_of_variables           108
c initial_number_of_clauses             2808
c initial_number_of_literal_occurrences 13986
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   0
c running_time(sec)                     95.4
c number_of_nodes                       274839
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                1210747
c number_of_pure_literals               1
c number_of_autarkies                   0
c number_of_missed_single_nodes         5
c max_tree_depth                        30
c number_of_table_enlargements          0
c number_of_1-autarkies                 51
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             VanDerWaerden_2-4-7_108.cnf
> OKsolver_2002-O3-DNDEBUG -M -D10 VanDerWaerden_2-4-7_109.cnf
s UNSATISFIABLE
c sat_status                            0
c initial_maximal_clause_length         7
c initial_number_of_variables           109
c initial_number_of_clauses             2862
c initial_number_of_literal_occurrences 14256
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   0
c running_time(sec)                     462.4
c number_of_nodes                       1173953
c number_of_single_nodes                2
c number_of_quasi_single_nodes          0
c number_of_2-reductions                5248010
c number_of_pure_literals               36
c number_of_autarkies                   0
c number_of_missed_single_nodes         18
c max_tree_depth                        31
c number_of_table_enlargements          0
c number_of_1-autarkies                 1031
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             VanDerWaerden_2-4-7_109.cnf

New run:

> OKsolver_2002-O3-DNDEBUG VanDerWaerden_2-4-7_109.cnf
s UNSATISFIABLE
c sat_status                            0
c initial_maximal_clause_length         7
c initial_number_of_variables           109
c initial_number_of_clauses             2862
c initial_number_of_literal_occurrences 14256
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   0
c running_time(sec)                     357.9
c number_of_nodes                       1184573
c number_of_single_nodes                2
c number_of_quasi_single_nodes          0
c number_of_2-reductions                5006946
c number_of_pure_literals               34
c number_of_autarkies                   0
c number_of_missed_single_nodes         18
c max_tree_depth                        31
c number_of_table_enlargements          0
c number_of_1-autarkies                 1029
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             VanDerWaerden_2-4-7_109.cnf

New order of clauses causing the differences?

   \endverbatim
   </li>
   <li> k=9, n=303:
   \verbatim
> OKsolver_2002-O3-DNDEBUG -M -D30 -F VanDerWaerden_2-4-9_303.cnf
 VanDerWaerden_2-4-9_303.cnf,   30, 1073741824
   26:   1371    830.15  8.91E+11    40.22s    23.54s   801y 156d  0h 43m 57s     0     0   49
   \endverbatim
   so even with 1000 processors one would get it just down to about a month.
   This is "infeasible", and better methods are needed. </li>
  </ul>


  \todo Performance of satz215
  <ul>
   <li>
   \verbatim
> satz215 VanDerWaerden_2-4-4_35.cnf
NB_MONO= 0, NB_UNIT= 707, NB_BRANCHE= 111, NB_BACK= 56
> satz215 VanDerWaerden_2-4-5_55.cnf
NB_MONO= 0, NB_UNIT= 19373, NB_BRANCHE= 2528, NB_BACK= 1279
> satz215 VanDerWaerden_2-4-6_73.cnf
NB_MONO= 39, NB_UNIT= 788138, NB_BRANCHE= 78809, NB_BACK= 39800
   \endverbatim
   </li>
   <li> This looks much worse than OKsolver_2002. </li>
  </ul>


  \todo Performance of march_pl
  <ul>
   <li> For VanDerWaerden_2-4-6_73.cnf 5520 nodes and 8.3 s are needed. </li>
   <li> For VanDerWaerden_2-4-7_108.cnf 73308 nodes and 240 s, and for
   VanDerWaerden_2-4-7_109.cnf 250583 nodes and 817 s. </li>
   <li> Comparison with OKsolver_2002 unclear; march_pl takes longer, but
   needs fewer nodes, perhaps due to the partial r_3-reduction and the added
   resolvents. </li>
  <li> k=9 likely needs to be run at least a month, while there is no
   (usable) progress meter; and given that march_pl is not really drastically
   faster than OKsolver_2002, it seems futile to run march_pl on instances
   where unsatisfiability is conjectured. </li>
  </ul>


  \todo Performance of minisat
  <ul>
   <li> minisat2:
   \verbatim
minisat2 VanDerWaerden_2-4-4_35.cnf
restarts              : 3
conflicts             : 344            (34407 /sec)

> minisat2 VanDerWaerden_2-4-5_55.cnf
restarts              : 8
conflicts             : 3755           (31297 /sec)

> minisat2 VanDerWaerden_2-4-6_73.cnf
restarts              : 17
conflicts             : 194391         (20058 /sec)
   \endverbatim
   </li>
   <li> This looks much worse than OKsolver_2002. </li>
   <li> Using minisat-2.2.0:
   \verbatim
> minisat-2.2.0 VanDerWaerden_2-4-6_73.cnf
conflicts             : 615029
> minisat-2.2.0 VanDerWaerden_2-4-7_109.cnf
conflicts             : 47798510
   \endverbatim
   Possibly here even worse than minisat2. </li>
  </ul>


  \todo Performance of picosat
  <ul>
   <li> picosat236:
   \verbatim
picosat VanDerWaerden_2-4-4_35.cnf
c 6 iterations
c 2 restarts
c 0 failed literals
c 318 conflicts
c 344 decisions

> picosat VanDerWaerden_2-4-5_55.cnf
c 7 iterations
c 3 restarts
c 0 failed literals
c 3191 conflicts
c 3440 decisions

> picosat VanDerWaerden_2-4-6_73.cnf
c 13 iterations
c 3 restarts
c 0 failed literals
c 107987 conflicts
c 116761 decisions

> picosat VanDerWaerden_2-4-7_108.cnf
c 0 iterations
c 3 restarts
c 0 failed literals
c 2442419 conflicts
c 2679965 decisions
> picosat VanDerWaerden_2-4-7_109.cnf
c 20 iterations
c 3 restarts
c 0 failed literals
c 4433747 conflicts
c 4860329 decisions
   \endverbatim
   </li>
   <li> Seems comparable to OKsolver_2002 (better than minisat2). </li>
   <li> Picosat913
    <ol>
     <li> k=7
     \verbatim
> picosat913 VanDerWaerden_2-4-7_109.cnf
4230321 conflicts
105.3 s
     \endverbatim
     similar to above. Likely the best conflict-driven solver, perhaps also
     better than OKsolver_2002. </li>
    </ol>
   </li>
  </ul>


  \todo Precosat
  <ul>
   <li> precosat236:
   \verbatim
> precosat236 VanDerWaerden_2-4-7_109.cnf
4240509 conflicts
   \endverbatim
   similar to picosat236. </li>
   <li> precosat-570
   \verbatim
> precosat-570.1 -v VanDerWaerden_2-4-7_109.cnf
aborted after 2505016 conflicts, 905.6 seconds (csltok)
   \endverbatim
   looks worse than precosat236. </li>
  </ul>


  \todo Glucose
  <ul>
   <li> "glucose VanDerWaerden_2-4-7_109.cnf" interrupted after 3995001
   conflicts and 1812.89s (csltok), without progress. </li>
   <li> Seems to be the worst conflict-driven solver. </li>
  </ul>


  \todo CryptoMiniSat, Grasp
  <ul>
   <li> Running them on VanDerWaerden_2-4-7_109.cnf doesn't seem to make
   progress. </li>
  </ul>


  \todo Palindromic numbers for k <= 8
  <ul>
   <li> All these numbers have been easily established with minisat-2.2.0 via
   \verbatim
> CRunPdVdWk1k2 4 k minisat-2.2.0
   \endverbatim
   </li>
   <li> Evaluation via
   \verbatim
E=read_crunpdvdw_minisat()
   \endverbatim
   in the experiment-directory. </li>
   <li> vdw_2^pd(4,4) = (24,25). </li>
   <li> vdw_2^pd(4,5) = (34,55). Odd might be harder than even. </li>
   <li> vdw_2^pd(4,6) = (62,65). Odd might be harder than even. </li>
   <li> vdw_2^pd(4,7) = (101,106). Odd might be harder than even. </li>
   <li> vdw_2^pd(4,8) = (126,129). Odd might be harder than even. </li>
  </ul>

*/

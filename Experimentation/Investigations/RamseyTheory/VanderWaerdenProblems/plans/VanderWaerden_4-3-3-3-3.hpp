// Oliver Kullmann, 17.4.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/VanderWaerden_4-3-3-3-3.hpp
  \brief Investigating vanderwaerden_4(3,3,3,3) = 76


  \todo Creating instances
  <ul>
   <li> Problems created via
   \verbatim
output_vanderwaerden_stdname([3,3,3,3],75);
output_vanderwaerden_stdname([3,3,3,3],76);
   \endverbatim
   </li>
   <li> This is with amo-clauses; one should try out whether without them
   it gets better, but this seems unlikely. </li>
  </ul>


  \todo vanderwaerden_3(3,3,3)
  <ul>
   <li> vdw_3(3,3,3)=27 is trivial for the OKsolver_2002. </li>
  </ul>


  \todo OKsolver_2002
  <ul>
   <li> The satisfiable case is rather simple for OKsolver_2002:
   \verbatim
s SATISFIABLE
c sat_status=1 initial_maximal_clause_length=4 initial_number_of_variables=300 initial_number_of_clauses=6001 initial_number_of_literal_occurrences=17628 running_time(s)=566.8 number_of_nodes=584228 number_of_single_nodes=1 number_of_quasi_single_nodes=0 number_of_2-reductions=6269491 number_of_pure_literals=0 number_of_autarkies=8 number_of_missed_single_nodes=27 max_tree_depth=46 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=0 reduced_number_of_clauses=0 reduced_number_of_literal_occurrences=0 number_of_1-autarkies=55871544 number_of_initial_unit-eliminations=0 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=450 file_name=VanDerWaerden_4-3-3-3-3_75.cnf
   \endverbatim
   </li>
   <li> The unsatisfiable case first looks relatively easy for
   OKsolver_2002, but then it falls into a hole:
   \verbatim
> OKsolver_2002-O3-DNDEBUG -M -D8 VanDerWaerden_4-3-3-3-3_76.cnf

Name of file and number of monitoring nodes:
 VanDerWaerden_4-3-3-3-3_76.cnf,  256
  1:  9463,    9.7, 2468.4
  2:  9871,   19.9, 2524.8
  3: 13402,   33.4, 2815.0
  4: 24511,   58.2, 3664.1
  5: 25061,   83.9, 4210.3
  6: 39083,  122.7, 5113.8
  7: 19000,  142.1, 5053.3
  8: 27779,  169.9, 5266.9
  9: 69830,  230.8, 6335.6
 10: 60815,  291.2, 7163.3
 11: 57532,  349.1, 7774.5
 12:102581,  448.3, 9115.6
 13: 38307,  493.1, 9217.2
 14: 86761,  583.0, 10078.1
 15: 56084,  639.6, 10276.4
 16: 91333,  731.4, 10970.7
 17: 32319,  763.1, 10727.9
 18: 38685,  801.6, 10599.6
 19: 97230,  896.8, 11185.8
 20: 91447,  991.6, 11700.9
 21:176581, 1149.4, 12862.0
 22:141459, 1287.2, 13690.6
 23:119928, 1401.5, 14197.9
 24:159665, 1562.3, 15102.3
 25:141975, 1693.5, 15648.3
 26:155485, 1843.8, 16310.6
 27:205828, 2035.5, 17263.7
 28:268974, -1989.0, -16196.0
 29: 83153, -1907.4, -14930.2
s UNKNOWN
c sat_status=2 initial_maximal_clause_length=4 initial_number_of_variables=304 initial_number_of_clauses=6156 initial_number_of_literal_occurrences=18088 running_time(s)=XXX number_of_nodes=32888781 number_of_single_nodes=1854 number_of_quasi_single_nodes=0 number_of_2-reductions=355120990 number_of_pure_literals=0 number_of_autarkies=2137 number_of_missed_single_nodes=2758 max_tree_depth=52 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=0 reduced_number_of_clauses=0 reduced_number_of_literal_occurrences=0 number_of_1-autarkies=2833485762 number_of_initial_unit-eliminations=0 number_of_new_2-clauses=0maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=456 file_name=VanDerWaerden_4-3-3-3-3_76.cnf
     \endverbatim
     </li>
     <li> Using OKsolver_2002 with the minisat2-preprocessor looks pretty bad:
     \verbatim
> OKsolver_2002-m2pp -M -D20 VanDerWaerden_4-3-3-3-3_76.cnf

Name of file and number of monitoring nodes:
 VanDerWaerden_4-3-3-3-3_76.cnf_m2pp_23201, 1048576
  1:  4007,    7.9, 8325685.5
  2: 19881,   46.7, 24468474.3
  3:  5024,   56.6, 19793563.0
  4: 10277,   77.8, 20386861.1
s UNKNOWN
c sat_status=2 initial_maximal_clause_length=9 initial_number_of_variables=228 initial_number_of_clauses=5852 initial_number_of_literal_occurrences=25764 running_time(s)=638.2 number_of_nodes=369748 number_of_single_nodes=16 number_of_quasi_single_nodes=0 number_of_2-reductions=2801506 number_of_pure_literals=0 number_of_autarkies=0 number_of_missed_single_nodes=10 max_tree_depth=55 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=0 reduced_number_of_clauses=0 reduced_number_of_literal_occurrences=0 number_of_1-autarkies=0 number_of_initial_unit-eliminations=0 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=228 file_name=VanDerWaerden_4-3-3-3-3_76.cnf_m2pp_23201
   \endverbatim
   Not clear whether it performs worse, or the above good start was just
   deceiving. </li>
   <li> With symmetry-breaking:

   </li>
  </ul>


  \todo march_pl


  \todo satz215


  \todo Minisat
  <ul>
   <li> 
   \verbatim
> minisat2 VanDerWaerden_4-3-3-3-3_76.cnf
*** INTERRUPTED ***
restarts              : 34
conflicts             : 172883204      (2914 /sec)
decisions             : 199996431      (1.62 % random) (3371 /sec)
propagations          : 5944428744     (100200 /sec)
conflict literals     : 5049993151     (21.33 % deleted)
Memory used           : 25.60 MB
CPU time              : 59325.5 s
   \endverbatim
   seems hopeless. </li>
  </ul>


  \todo The behaviour of m -> vanderwaerdend(m,3)
  <ul>
   <li> The known values are
   \verbatim
create_list(vanderwaerdend(m,3),m,0,4);
 [1,3,9,27,76]
   \endverbatim
   </li>
   <li> Given that ralpha_arithprog(3,n) <= f(n) for almost all n,
   where f(x) is continuous and strictly decreasing, we have
   vanderwaerdend(m,3) <= f^-1(1/m) + 1. </li>
   <li> Now by [Brown, Landman, Robertson, 2008] we know that there
   exists a constant c with
   vanderwaerdend(m,3) > m^(c * log(m)). </li>
   <li> Thus the approximation f(n) ~ 1.226541 * n^(-0.3333806)
   is too optimistic (1/f(n) grows too fast). </li>
   <li> By f(n) ~ exp(-0.2087957 * log(n)^1.215668)
   (see "Predictions" in
   Investigations/RamseyTheory/VanderWaerdenProblems/Transversals/plans/Datak3.hpp)
   we get
   f^(-1)(1/m) ~ exp(3.627373 * log(m)^0.822593), yielding the following
   predictions (for the upper bound!) (using ft(m) = f^(-1)(1/m))
   \verbatim
round(ft(1:10),1)
 1.0 14.6 50.4 115.1 213.9 350.9 529.7 753.1 1023.8 1344.2
   \endverbatim
   (compared to 3 9 27 76). </li>
   <li> To summarise, a model for vanderwaerdend(m,3) is
   exp(3.627373 * log(m)^0.822593), or, more generally, exp(a * log(m)^b).
   </li>
   <li> For the given data and the regression model we obtain b <= 1, which
   yields only polynomial growth, while we know that the growth is
   super-polynomial, and thus b > 1 must be the case. </li>
   <li> Using this model directly on the four data points (1 <= m <= 4)
   (of course, this is not much data):
   \verbatim
d = c(3,9,27,76)
x = log(log(1:4)[-1])
y = log(log(d[-1]))
L = lm(y ~ x)
summary(L)
Coefficients:
            Estimate Std. Error t value Pr(>|t|)
(Intercept)  1.13110    0.02106   53.70   0.0119 *
x            0.96507    0.07298   13.22   0.0481 *
plot(x,y)
lines(x,predict(L))
C = coefficients(L)
f = function(m){exp(exp(C[1]) * log(m)^C[2])}
plot(d)
lines(f(1:length(d)))
   \endverbatim
   so we get f(m) ~ exp(3.099079 * log(m)^0.96507. </li>
   <li> So the exponent b "improved", but it is still < 1. </li>
  </ul>

*/

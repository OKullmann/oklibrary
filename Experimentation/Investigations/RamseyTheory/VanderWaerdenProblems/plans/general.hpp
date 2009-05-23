// Oliver Kullmann, 20.9.2008 (Swansea)
/* Copyright 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/general.hpp
  \brief On investigations into van-der-Waerden problems


  \todo Connections
  <ul>
   <li> See ComputerAlgebra/RamseyTheory/Lisp/VanderWaerden/plans/general.hpp. </li>
   <li> See VanderWaerden.cpp (to be updated) for a C++ generator. </li>
   <li> See ComputerAlgebra/Hypergraphs/Lisp/plans/Generators.hpp. </li>
   <li> See ComputerAlgebra/Satisfiability/Lisp/Generators/plans/VanderWaerdenProblems.hpp.
   </li>
   <li> External pages:
    <ol>
     <li> http://www.st.ewi.tudelft.nl/sat/waerden.php contains lower bounds
     and methods how to get them. </li>
    </ol>
   </li>
  </ul>


  \todo Boolean van der Waerden numbers
  <ul>
   <li> Via
   \verbatim
VanderWaerden-O3-DNDEBUG k n > VanderWaerden_2-k-k_n.cnf
   \endverbatim
   diagonal boolean problems can be generated quickly. </li>
   <li> In Maxima we have "output_vanderwaerden2_stdname(k,n)" (in
   ComputerAlgebra/Satisfiability/Lisp/Generators/VanderWaerdenProblems.mac)
   as well as "output_vanderwaerden2nd_stdname(k1,k2,n)"
   </li>
   <li> vanderwaerden_2(3) = 9 (density = 3.6) </li>
   <li> vanderwaerden_2(4) = 35 (density = 10.7) </li>
   <li> vanderwaerden_2(5) = 178 (density = 43.5)
    <ol>
     <li> Relatively easy for OKsolver-2002 (710s, 109703 nodes). </li>
     <li> march_pl needs only 51529 nodes and 238 seconds, so the "double
     look-ahead" seems to be successful here. </li>
     <li> minisat1 needs 213 seconds, so also clause-learning seems applicable
     here; minisat2 is even faster (21 restarts, 829170 conflicts, 90s). </li>
     <li> satz215 doesn't solve it in 30 minutes. </li>
    </ol>
   </li>
   <li> For vanderwaerden_2(6) see
   Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/VanderWaerden_2-6-6.hpp.
   </li>
  </ul>


  \todo Script for searching for a vdW-number
  <ul>
   <li> Input is the vdW-parameter-tuple, and the start value for n. </li>
   <li> Then the script runs through n, n+1, ..., until the instance
   is unsatisfiable. </li>
   <li> For problems with two parts only the SAT solver is needed as
   parameter, while for more parts also the translation method is needed. </li>
   <li> A complication arises when using incomplete algorithms for finding a
   satisfying assignment.
    <ol>
     <li> The main problem then is the number of runs and the cutoff. </li>
     <li> And at a certain point one has to try a complete solver. </li>
     <li> Perhaps all this needs a second script; so we have one script for
     running one complete solver, and one script for running one incomplete
     solver. </li>
    </ol>
   </li>
  </ul>


  \todo Symmetry breaking for van der Waerden numbers
  <ul>
   <li> As for any hypergraph colouring problems, one can choose any variable
   and set it to true (or to false), due to the symmetry between 0 and 1. </li>
   <li> This is best "told" the solver, so that it can use this for the
   branching variable at the root. </li>
   <li> No other symmetry breaking seems possible without conditioning. </li>
   <li> Is there not also, at least, the symmetry about the number line? i.e. 
   if there is a colouring of the numbers such that there is no arithmetic 
   progression of size k, then reversing the colouring should also have this 
   property. </li>  
   <li> For small problems one needs to determine the full automorphism group
   of the clause-sets. </li>
  </ul>


  \todo Overview on performance of SAT solvers
  <ul>
   <li> We need an overview how SAT solvers perform on the different van der
   Waerden problems. </li>
  </ul>


  \todo Preprocessing
  <ul>
   <li> See "A general perspective" and "Preprocess a CNF via computing DNFs
   for parts" in
   ComputerAlgebra/Satisfiability/Lisp/Preprocessing/plans/general.hpp. </li>
   <li> Let FvdW_m(k, S) for a set S of natural numbers denote the natural
   generalisation of the diagonal vdW-clause-set FvdW_m(k,n), using S instead
   of {1,...,n}. </li>
   <li> Now for an arithmetic progression S of length n_0 the clause-set
   FvdW_m(k,S) is isomorphic (without flips) to FvdW_m(k,n_0). </li>
   <li> Then for arbitrary n one can consider all arithmetic progressions S
   of length n_0 in {1,...,n}, and their sub-clause-sets FvdW_m(k, S) of
   FvdW_m(k,n), which together cover much of FvdW_m(k,n) for n >= n_0.
   Or all? No, consider for example n=5 and n_0=4: {1,3,5} is not covered.
   </li>
   <li> Easiest not to do anything with those uncovered clauses. </li>
   <li> The minimal satisfiable assignments of FvdW_m(k,n_0) can be obtained
   from Tr(ap(3,n_0)) (the transversal hypergraph) by considering m
   transversals with empty common intersection. </li>
   <li> Simple statistics for boolean problems:
    <ol>
     <li> k=3:
     \verbatim
for n : 3 do print(n,length(arithprog_hg(3,n)[2]),min_resolution_closure_cs(vanderwaerden2_fcs(3,n)[2])[2]);
3 1 [2,2]
4 2 [4,4]
5 4 [8,12,12]
6 6 [12,22,26,26]
7 9 [18,54,88,38,38]
8 12 [24,94,226,150,116,72,72]
9 16 [32,164,508,402,182,34,1,1]
     \endverbatim
     </li>
     <li> k=4:
     \verbatim
for n : 4 do print(n,length(arithprog_hg(4,n)[2]),min_resolution_closure_cs(vanderwaerden2_fcs(4,n)[2])[2]);
4 1 [2,2]
5 2 [4,4]
6 3 [6,6]
7 5 [10,12,12]
8 7 [14,18,18]
9 9 [18,28,32,32]
10 12 [24,56,80,80]
11 15 [30,92,206,208,202,202]
12 18 [36,136,456,488,460,460]
     \endverbatim
     </li>
     <li> k=5:
     \verbatim
for n : 5 do print(n,length(arithprog_hg(5,n)[2]),min_resolution_closure_cs(vanderwaerden2_fcs(5,n)[2])[2]);
5 1 [2,2]
6 2 [4,4]
7 3 [6,6]
8 4 [8,8]
9 6 [12,14,14]
10 8 [16,20,20]
11 10 [20,26,26]
12 12 [24,36,36]
13 15 [30,60,88,88,88]
14 18 [36,88,186,194,194]
15 21 [42,120,358,422,422]
16 24 [48,160,602,742,742,742]
     \endverbatim
     </li>
     <li> k=6:
     \verbatim
for n : 6 do print(n,length(arithprog_hg(6,n)[2]),min_resolution_closure_cs(vanderwaerden2_fcs(6,n)[2])[2]);
6 1 [2,2]
7 2 [4,4]
8 3 [6,6]
9 4 [8,8]
10 5 [10,10]
11 7 [14,16,16]
12 9 [18,22,22]
13 11 [22,28,28]
14 13 [26,34,34]
15 15 [30,44,48,48]
16 18 [36,60,70,70]
17 21 [42,76,92,92]
18 24 [48,92,114,114]
19 27 [54,116,178,186,186]
20 30 [60,144,282,314,314]
21 34 [68,204,666,870,876,876]
     \endverbatim
     </li>
    </ol>
   </li>
   <li> One should write a generator, first only for the boolean problems,
   which creates a clause-set equivalent to vanderwaerden2_fcs(k,n) by
   removing for given n0 all clauses covered by one of FvdW_2(k, S), for
   arithmetic progressions S in {1,...,n} of length n0, and adding instead
   the prime implicates of FvdW_2(k, S). </li>
   <li> Hard to imagine that the increase in size is compensated by the
   added reasoning power; but who knows (and the different solver types
   might react differently). </li>
   <li> Perhaps considering all S is too much overlap? </li>
   <li> More interesting perhaps is the computation of smallest (w.r.t. the
   number of clauses) DNF representations of FvdW_2(k,n).
    <ol>
     <li> The straight-forward way is to first compute all satisfying
     total assignments (we can just use brutest-force), and then to
     apply first Quine/McCluskey, and then minimisation. </li>
     <li> See "Minimisation" in
     OKlib/Satisfiability/FiniteFunctions/plans/general.hpp. </li>
     <li> 
    </ol>
   </li>
  </ul>

*/


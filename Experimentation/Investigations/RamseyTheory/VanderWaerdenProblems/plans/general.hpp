// Oliver Kullmann, 20.9.2008 (Swansea)
/* Copyright 2008, 2009, 2010 Oliver Kullmann
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
   <li> See ComputerAlgebra/Satisfiability/Lisp/Generators/RamseyTheory/plans/VanderWaerdenProblems.hpp.
   </li>
   <li> External pages:
    <ol>
     <li> http://www.st.ewi.tudelft.nl/sat/waerden.php contains lower bounds
     and methods how to get them. </li>
     <li> http://users.encs.concordia.ca/~ta_ahmed/vdw.html contains the list
     of known numbers (except of transversal vdW-numbers). </li>
    </ol>
   </li>
  </ul>


  \todo Create milestones
  <ul>
   <li> Perhaps the initial version number is 0.2. </li>
   <li> Next milestones concentrate on vdw_2(3,k). </li>
  </ul>


  \todo DONE (as stated)
  Organisation
  <ul>
   <li> We should create a sub-directory "3-k" for investigations into
   vdw_2(3,k). </li>
   <li> There we have file "k.hpp" for special k-values (for example in 30.hpp
   the problems related to vdw_2(3,30) are investigated). </li>
  </ul>


  \todo Boolean van der Waerden numbers
  <ul>
   <li> Via
   \verbatim
> VanderWaerden-O3-DNDEBUG k n > VanDerWaerden_2-k-k_n.cnf
   \endverbatim
   diagonal boolean problems can be generated, and via
   \verbatim
> VanderWaerden-O3-DNDEBUG k1 k2 n > VanDerWaerden_2-k1-k2_n.cnf
   \endverbatim
   non-diagonal problems. </li>
   <li> In Maxima we have "output_vanderwaerden2_stdname(k,n)" (in
   ComputerAlgebra/Satisfiability/Lisp/Generators/RamseyTheory/VanderWaerdenProblems.mac)
   as well as "output_vanderwaerden2nd_stdname(k1,k2,n)".
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
   <li> See
   ExperimentSystem/ControllingLocalSearch/plans/PointOfUnsatisfiability.hpp.
   </li>
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
   <li> The script RunVdW3k is a first very simple tool, just using a fixed
   number of runs and a fixed cutoff. </li>
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


  \todo Palindromic versions
  <ul>
   <li> For every parameter tuple t one can consider the problem where the
   solutions are required to be palindromes. </li>
   <li> This is equivalent to replacing the hypergraphs arithprog_hg(k,n) by
   palindromise_vdw_hg(arithprog_hg(k,n)) (where the k's run through t). </li>
   <li> Boolean problems are generated by
   output_pd_vanderwaerden2nd_stdname(k1,k2,n). </li>
   <li> So we should consider vanderwaerden_g(t,"pd"), the smallest n such that
   for all n' >= n no palindromic solution w.r.t. t exists. </li>
   <li> Compare "Palindromic numbers" in
   ComputerAlgebra/RamseyTheory/Lisp/VanderWaerden/plans/Numbers.hpp. </li>
   <li> Unsat gaps:
    <ol>
     <li> It can happen that for some n the problem is solvable, while it is
     unsolvable for some n' < n. </li>
     <li> For example for t=(3,3) the problems are solvable for n=6,8, while
     unsolvable for n=7:
     \verbatim
for n : 1 thru 9 do print(n,current_satsolver(pd_vanderwaerden2nd_fcs(3,3,n)));
1 true 
2 true 
3 true 
4 true 
5 true 
6 true 
7 false 
8 true 
9 false 
     \endverbatim
     </li>
     <li> So the "unsat gap", the largest difference n_2 - n_1, such that
     (for the given t) the problem is solvable for n_2 and n_1 with n_2 > n_1,
     while for all n_1 < n < n_2 the problem is unsolvable. </li>
     <li> The above example shows an unsat-gap of 2. </li>
     <li> Is an unsat-gap larger than 2 possible? </li>
    </ol>
   </li>
   <li> Transversal extensions are not interesting in their palindromic
   versions, since for k=2 for even n>= 2 there is no solution, while for odd
   n exactly the central vertex has to be chosen. </li>
   <li> Version of RunVdW3k for palindromic problems:
    <ol>
     <li> Let's call it "RunPdVdW33k". </li>
     <li> An upper bound on the unsat-gap is given; say unsat_gap is 2 or 3.
     </li>
     <li> That is, if unsat_gap consecutive "unsatisfiable" problems
     (according to the current settings) are found, then the first
     unsatisfiable problem is revisited. </li>
     <li> Later, one could increate cutoff or rounds, however in the first
     version we just stop. </li>
     <li> So RunPdVdW33k is very similar to RunVdW33k, except that it only
     stops after unsat-gap many unsatisfiable problems in a row, and that
     a different generator is used. </li>
    </ol>
   </li>
  </ul>


  \todo Overview on performance of SAT solvers
  <ul>
   <li> We need an overview how SAT solvers perform on the different van der
   Waerden problems. </li>
  </ul>

*/


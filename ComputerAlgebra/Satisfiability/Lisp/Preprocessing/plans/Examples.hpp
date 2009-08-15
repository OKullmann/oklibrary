// Oliver Kullmann, 15.8.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/Preprocessing/plans/Examples.hpp
  \brief Applying preprocessing to specific instance classes

  Preprocessing by local dualisation should be applied to examples of 
  small and/or well known problems, to test the idea, and to gain insights.


  \todo Pigeonhole formulas:
  <ul>
   <li> As an example, if one takes the weak PHP principle with m pigeons and
   n holes, one can represent pigeon i occupying a single hole by
   n DNF-clauses, each encoding that pigeon i sits in a particular hole
   as its sole occupier. </li>
   <li> The weak PHP problem can then be represented by considering the
   conjunction of these DNF's, which can be represented as a CNF by
   translating each of these DNF's into CNF via the canonical translation.
   </li>
   <li> This can be done using the following code:
   \verbatim
weak_php_ts_fcl(m,n) := 
  cl2fcl(lappend(create_list(
    dualtsext_fcl(cl2fcl(pigeon_in_hole_dnf_cl(i,m,n)), i)[2],
    i, 1, m)))$

pigeon_in_hole_dnf_cl(i,m,n) :=
  create_list(setify(create_list(if i2 = i then php_var(i2,j) else -php_var(i2,j), i2, 1, m)), j, 1, n)$
    
output_weak_php_ts(m,n,filename) := block([FF : standardise_fcl(weak_php_ts_fcl(m,n))],
  output_fcl_v(
    sconcat("Weak PHP with ", m, " pigeons and ", n, " holes in dual Tseitin representation."), 
    FF[1], filename, FF[2]))$
output_weak_php_ts_stdname(m,n) := output_weak_php_ts(m,n,
  sconcat("PHP_weak_ts_",m,"_",n,".cnf"))$
   \endverbatim
   </li>
   <li> MG should present experimental data on this; for this (of course!)
   the above code has to become "official" (with tests and documentation).
   </li>
   <li> Understanding this example:
    <ol>
     <li> Now we want to understand what this "preprocessing" of
     weak_php_fcs(m,n) means. </li>
     <li> The clause-set is covered by m clause-set F_i, each given by the
     "long" clause C_i expressing that pigeon i sits in one hole, plus all
     clauses which can be resolved with C_i. </li>
     <li> F_i expresses exactly that pigeon i sits in at least one hole, and
     no hole occupied by pigeon i is also occupied by another pigeon. </li>
     <li> The unique DNF of F_i is given by pigeon_in_hole_dnf_cl(i,m,n).
     </li>
     <li> So weak_php_ts_fcl(m,n) is an example of what could be obtained from
     weak_php_fcs(m,n) by preprocessing by local dualisation. </li>
     <li> Now consider the prime clause-set P_i of F_i (all prime implicates,
     since F_i is a CNF here). </li>
     <li> P_i is exponentially large, however it contains no interesting
     elements, since all elements of P_i can be obtained from F_i by
     input resolution. </li>
     <li> So F_i has already the property, that every inconsistency and also
     every forced assignment can be detected by unit-resolution, while only
     total satisfying assignments exist. </li>
     <li> This can easily be seen by flipping the sign of all variables
     phi(i,j) (for all j), where now F_i becomes a Horn clause-set. </li>
     <li> Thus very likely F_i is the best representation possible here, and
     preprocessing can only make things worse. </li>
     <li> MG should implement the underlying functions, validate all statements
     experimentally, and then should prove all of this. </li>
     <li> A necessary condition for making the general
     preprocessing-by-local-compilation approach work seems to be that for the
     F_i constituting the cover there must exist some prime clauses which are
     hard to derive at least by tree resolution. </li>
    </ol>
   </li>
  </ul>

*/

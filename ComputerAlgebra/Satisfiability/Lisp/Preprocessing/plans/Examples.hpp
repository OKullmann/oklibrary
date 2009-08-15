// Oliver Kullmann, 15.8.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/Preprocessing/plans/Examples.hpp
  \brief Applying preprocessing to specific instance classes

  Certain hardcoded preprocessing using the idea of finding a cover and then 
  applyingCNF->DNF->CNF translation should be applied to examples of 
  small and/or well known problems, to test the idea and attempt to offer
  insight into the development.


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
  </ul>

*/

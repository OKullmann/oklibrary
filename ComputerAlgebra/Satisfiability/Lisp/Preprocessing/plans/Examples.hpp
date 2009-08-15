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
rename_var(m_i, m, n, FF) := block([FF2 : FF, num_dts_vars : length(FF[1])],
  for i : 1 thru n do
    for i : num_dts_vars - (n*m) thru 1 step -1 do
      FF2 : subst(dts_var((m_i-1) * (num_dts_vars - (n*m)) + i),dts_var(i),  FF2),
  return(FF2))$

weak_php_ts(m,n) := 
  cs2fcs(lunion(create_list(
    setify(rename_var(i,m,n,dualts_fcl(fcs2fcl(cs2fcs(pigeon_in_hole_dnf(i,m,n)))))[2]),
    i, 1, m)))$

pigeon_in_hole_dnf(i,m,n) :=
  setify(create_list(setify(create_list(if i2 = i then php_var(i2,j) else -php_var(i2,j), i2, 1, m)), j, 1, n))$
    
output_weak_php_ts(m,n,f) :=
  output_fcs(
    sconcat("PHP with ", m, " pigeons and ", n, " holes."), 
    standardise_fcs(weak_php_ts(m,n))[1], 
    f)$
   \endverbatim
   </li>
   <li> MG should present experimental data on this; and also documentation
   for the above code is needed (what is the point of "rename_var" ---
   we should always use the original variables, and avoid renaming and thus
   what is needed is a generalisation of dualts_fcl!). </li>
  </ul>

*/

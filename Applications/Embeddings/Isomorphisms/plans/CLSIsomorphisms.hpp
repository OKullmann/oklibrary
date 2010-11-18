// Oliver Kullmann, 12.11.2005 (Swansea)
/* Copyright 2005 - 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Isomorphisms/plans/CLSIsomorphisms.hpp
  \brief Deciding whether two clause-sets are isomorphic by reductions to
  (generalised) SAT problems.

  Given two clause-sets F_1, F_2, the variable sets var(F_1), var(F_2)
  are partioned into equivalence classes of variables with the same domain size,
  and bijections between corresponding equivalence classes of F_1 and F_2 are
  sought together with bijections between the associated domains, such that F_1
  is transported into F_2.


  \todo Relations to other modules
  <ul>
   <li> See ComputerAlgebra/Satisfiability/Lisp/Symmetries/plans/general.hpp
   for the plans on installations at the Maxima/Lisp level. </li>
  </ul>


  \todo Reduction to graph isomorphisms
  <ul>
   <li> The basic reduction is to the graph isomorphism problem (with "coloured"
   vertices), using the variables-literals-clauses graph. For P-literals the
   variable-values-literals-clauses graph is to be used. </li>
   <li> These graphs must be easily accessible for clause-sets, directly usable
   for the graph-isomorphism-algorithms. </li>
  </ul>


  \todo We also need the determination of the automorphism group of a clause-set,
  either explicitly listing all elements or giving a more compact representation
  (using some group theory software).


  \todo Applications
  <ul>
   <li> An important application should be to detect symmetries for SAT
   decision: Given interesting branchings (phi_1, ..., phi_m) for F, check
   for isomorphisms among the phi_i * F (isomorphic branches don't need to be
   repeated). </li>
   <li> To phi_i * F any kind of reduction can be applied as long as it is
   compatible with isomorphisms, since isomorphism checking here is only a means
   to an end. </li>
   <li> For this application the main goal is to avoid repeated computations.
   </li>
   <li> And also incomplete isomorphism tests can be used (potentially not
   finding an isomorphism if there is one). </li>
  </ul>

*/


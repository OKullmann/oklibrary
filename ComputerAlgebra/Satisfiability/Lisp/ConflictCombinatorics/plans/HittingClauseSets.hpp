// Oliver Kullmann, 23.2.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/ConflictCombinatorics/plans/HittingClauseSets.hpp
  \brief Plans related to hitting clause-sets


  \todo Constructions
  <ul>
   <li> The set of hitting clause-sets is a sub-monoid (with zero) of
   CLS with "or_cs" (see
   ComputerAlgebra/Satisfiability/Lisp/ClauseSets/plans/Constructions.hpp).
   </li>
  </ul>


  \todo Representations
  <ul>
   <li> hitting_cls_rep_st(T) represents the boolean function F underlying T
   in the following ways in case T involves reduction r:
    <ol>
     <li> In any case both representations (CNF and DNF) are satisfiability-
     equivalent. </li>
     <li> If r only used forced assignments, then after amendment
     the DNF-representation yields an equivalent clause-set. </li>
    </ol>
   </li>
   <li> This should be documented, and perhaps r-splitting trees using only
   forced assignments get a proper name. </li>
  </ul>


  \todo Decomposing clause-sets into hitting clause-sets
  <ul>
   <li> DONE
   Write a function which actually computes the conflict-partition-number
   of a clause-set (the smallest number of hitting sub-clause-sets which
   partition the clause-set). </li>
  </ul>

*/


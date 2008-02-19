// Oliver Kullmann, 19.2.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/MinimalUnsatisfiability/plans/general.hpp
  \brief Plans for Maxima-components regarding minimally unsatisfiable clause-sets


  \todo Create milestones


  \todo Create tests


  \todo MUSAT(1)
  <ul>
   <li> In
   ComputerAlgebra/Satisfiability/Lisp/ConflictCombinatorics/HittingClauseSets.mac
   we have to generators for elements of SMUSAT(1). </li>
   <li> MUSAT(1)
    <ol>
     <li> The problem is that there are many ways of removing literal
     occurrences from elements of SMUSAT(1). </li>
     <li> Motivated by [Hoory, Szeider, TCS 2005] we should consider the
     following decision problem:
      <ul>
       <li> Input a clause-set F and natural number k, r. </li>
       <li> It is also assumed that F does not contain pure literals, and
       and every clause has length at least k. </li>
       <li> Decide whether it is possible by removal of literal occurrences,
       one by one, without ever producing a pure literal and such that
       the minimal clause-length is at least k, to obtain a maximal
       variable-degree of at most r. </li>
       <li> The interesting case here is F in SMUSAT(1), where it is
       guaranteed that the literal-removal-process will never contract
       clauses (since every clause contains a literal occurring only once).
       </li>
       <li> But this procedure is also relevant in general, since the resulting
       clause-set is always an unsatisfiable clause-set with parameters (k,r)
       in the typical setting here (i.e., minimal rank >= k, maximal
       variable-degree <= r). </li>
      </ul>
     </ul>
     <li> Let's call it "REMLITOCC". </li>
     <li> The problem is in NP, one likely also NP-complete (also for
     inputs from SMUSAT(1)). </li>
     <li> The question is to find out for given k what is the smallest
     possible r = f(k).
      <ol>
       <li> Apparently f(3) = 3 and f(4) = 4 are the only known values. </li>
       <li> We should exhibit such examples. </li>
      </ol>
     </li>
     <li> A translation into SAT of course is of interest, directly into
     CNF, or using active clauses (etc.). </li>
     <li> There is an obvious greedy algorithm, which considers a variable
     occurring maximally often, and removes as many occurrences as needed
     to get the degree down to r (if possible --- otherwise the problem is
     unsolvable). </li>
    </ol>
   </li>
  </ul>

*/


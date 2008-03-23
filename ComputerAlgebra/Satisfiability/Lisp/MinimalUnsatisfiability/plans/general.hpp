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


  \todo Connections to other modules
  <ul>
   <li> In
   ComputerAlgebra/Satisfiability/Lisp/ConflictCombinatorics/HittingClauseSets.mac
   we have to generators for elements of SMUSAT(1). </li>
   <li> See Experimentation/Investigations/plans/general.hpp. </li>
  </ul>


  \todo Small variable-degrees
  <ul>
   <li> Motivated by [Hoory, Szeider, TCS 2005] we consider the
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
   </li>
   <li> Let's call it "REMLITOCC". </li>
   <li> The problem is in NP, one likely also NP-complete (also for
   inputs from SMUSAT(1)). </li>
   <li> The general question is to find out for given k what
   is the smallest possible r = f(k), where in this module we consider
   F in SMUSAT(1).
    <ol>
     <li> Apparently f(3) = 4 and f(4) = 5 are the only known values. </li>
     <li> Interesting also the (minimal) clause-numbers for realising the
     minimal parameter values. </li>
     <li> [Dubois, DAM 1990] realises (k=3,r=4) with 22 clauses, (k=4,r=6) with
     209 clauses, and (k=5,r=11) with 3986 clauses. </li>
     <li> [Hoory, Szeider] realise (k=4,r=5) (optimal) and (k=5,r=7), and
     other values, but one has to investigate the underlying clause-sets
     (which are not discussed there explicitely). </li>
     <li> We should exhibit such examples. </li>
     <li> 
    </ol>
   </li>
   <li> A translation into SAT of course is of interest, directly into
   CNF, or using active clauses (etc.). </li>
   <li> There is an obvious greedy algorithm: see below. </li>
  </ul>


  \todo The heuristical function remlitocc_greedy
  <ul>
   <li> Just to find an example for (k=3,r=5) from uniform_usat_hitting_min(m)
   doesn't work for m <= 14. </li>
   <li> Are these instances in MUSAT(1) so large? </li>
   <li> Or should we start from different elements F in SMUSAT(1) ? </li>
   <li> Or is the heuristic so weak?
    <ol>
     <li> For comparison, we need a precise decision algorithm. </li>
     <li> We could randomise the procedure. </li>
    </ol>
   </li>
  </ul>


  \todo Minimal variable-degree of uniform minimally unsatisfiable clause-sets
  <ul>
   <li> Let minvardeg_umu(k) for k >= 0 be the minimal possible (maximal)
   variable-degree of (minimally) unsatisfiable k-uniform boolean
   clause-sets F. </li>
   <li> The todos "Small variable-degrees" and
   "The heuristical function remlitocc_greedy" above are about heuristics
   for finding upper bounds on minvardeg_umu(k). </li>
   <li> We need to implement minvardeg_umu(k) that it yields the currently
   known lower and upper bounds. </li>
   <li> We also need witnesses for the upper and lower bounds:
    <ol>
     <li> Upper bounds are realised by single examples. </li>
     <li> See "Translations reducing the number of variable-occurrences" in
     ComputerAlgebra/Satisfiability/Lisp/ClauseSets/plans/Translations.hpp for
     the realisation of the upper bound 2+2^(k-2) (k >= 2). </li>
     <li> For lower bounds one needs algorithms which find quickly a
     satisfying assignments. </li>
     <li> The lower bound k+1 (k >= 1) is realised by matching-satisfiability.
     </li>
    </ol>
   </li>
   <li> The restrictions to hitting clause-sets F are considered in
   ComputerAlgebra/Satisfiability/Lisp/ConflictCombinatorics/plans/HittingClauseSets.hpp.
   </li>
  </ul>


  \todo Creating marginal elements of MUSAT(1)
  <ul>
   <li> What is the scope of marginal_musat1(k) ? </li>
   <li> In [Kullmann, 2008] it is shown that the marginal elements of
   MUSAT(1) are exactly those whose conflict graph is a tree (and all
   trees are realisable in this way). </li>
   <li> For a given F in SMUSAT(1), perhaps the marginal F' obtainable
   from F via literal-removal correspond exactly to the spanning trees
   of F? So if F has c clauses, then exactly c^(c-2) different marginal
   clause-sets are obtainable? Are these isomorphic iff the conflict-graphs
   are isomorphic? </li>
   <li> One should test how SAT-solvers (with the appropriate preprocessing)
   react to MUSAT(1): SMUSAT(1) definitely should be easy, also the marginal
   elements (since they can be solved by UCP), but close to the marginal
   there could be harder elements? </li>
  </ul>

*/


/* Oliver Kullmann, 9.8.2009 (Swansea) */
/* Copyright 2009, 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/Primality/plans/general.hpp
  \brief General plans for handling prime clauses and dual prime clauses

  This includes:
  <ul>
   <li> Computation of all prime implicates and/or all prime implicants. </li>
   <li> Computation of shortest CNF and/or shortest DNF representation of a
   finite function. </li>
   <li> Dualisation: Translating CNF clause-sets into equivalent DNF
   clause-sets, or, equivalently, translating DNF clause-sets into equivalent
   CNF clause-sets. Or the same as finding for a CNF a collection of satisfying
   partial assignments which cover all satisfying (total) assignments. </li>
  </ul>


  \todo Organisation
  <ul>
   <li> Move
   ComputerAlgebra/Satisfiability/Lisp/Resolution/PrimeImplicatesImplicants.mac
   here. </li>
   <li> The task of just finding *some* DNF-representation (without new
   variables!) of a CNF seems close enough to finding dual prime clauses
   to be included in this module. </li>
   <li> Or should there be a specialisation module "Duality" for this task?
   But except of the splitting-tree methods all other methods are closely
   related to finding dual prime clauses. </li>
  </ul>


  \todo Notions
  <ul>
   <li> For a clause-set F we simply speak of "prime clauses", which are
   the minimal clauses derivable by resolution from F. </li>
   <li> For F as CNF, these are the minimal clauses following from F,
   while for F as DNF these are the minimal clauses implying F. </li>
   <li> For boolean functions f we use "CNF prime clauses" for
   "prime implicates", and "DNF prime clauses" for "prime implicantes". </li>
   <li> Note that the CNF prime-clauses for f are exactly the prime clauses
   for any CNF-representation F of f, while the DNF prime clauses for f are
   exactly the prime clauses for any DNF-representation F for f. </li>
   <li> A clause-set F is "prime" if all C in F are prime clauses for F,
   while F is "maximal prime" if F is prime, and actually contains all
   its prime-clauses. </li>
   <li> A "dual prime clause" of a clause-set F is a prime clause of the
   combinatorial dualisation dual_cs(F), which at the boolean function level
   corresponds to dualisation of "the" boolean function (negation of inputs
   and outputs; whether CNF- or DNF-representation is used, does not
   matter(!)), which is the same as considering F as CNF instead of DNF or
   vice versa. </li>
   <li> That is, a clause C is a dual prime clause of F iff C is a minimal
   transversal of F, considered as hypergraph over its literals. </li>
   <li> Considering F as CNF, the dual prime clauses are exactly the minimal
   satisfying partial assignments, representing partial assignments as sets
   of literals set to true. </li>
   <li> And considering F as DNF, the dual prime clauses are exactly the
   minimal falsifying partial assignments, representing partial assignments
   as sets of literals set to false. </li>
   <li> The prime clauses of F are exactly the dual prime clauses of the
   set of dual prime clauses of F. </li>
  </ul>


  \todo Connections / Relations
  <ul>
   <li> See Algorithms/AllSolutions/plans/MinimalAssignments.hpp for
   dualisation as computation of generalised transversal hypergraphs. </li>
  </ul>


  \todo Create milestones.


  \todo Extract the methods related to splitting trees
  <ul>
   <li> From the various splitting-tree forms we need to extract
   the clauses representing the satisfying assignments (in CNF interpretation).
   </li>
  </ul>


  \todo Shortest dual representations
  <ul>
   <li> For a clause-set (as CNF) the list of shortest DNF-representations is
   given by minimal sets of dual prime clauses (minimal w.r.t. covering
   everything). </li>
   <li> Some function for computing the list of shortest DNF-representations is
   dual_all_minequiv_bvs_cs in
   Satisfiability/Lisp/Resolution/PrimeImplicatesImplicants.mac.
   </li>
   <li> If all shortest representations are needed, then the algorithm for
   computing all shortest minimum transversals of a hypergraph needs to be
   improved. </li>
   <li> While if just one shortest is needed, then it seems reasonable to first
   find some DNF-representation, and to take this "approximation" as a basis.
   </li>
   <li> For finding some short DNF-representation not all dual prime clauses
   are needed, but it seems indispensable, even for just finding one shortest
   DNF-representation, to compute all dual prime clauses? </li>
   <li> Generalising transversal_hg_rs in
   Hypergraphs/Lisp/Transversals/Minimal/RecursiveSplitting.mac,
   implement the basic recursive splitting algorithm as discussed in
   "The basic algorithm" in
   Algorithms/AllSolutions/plans/MinimalAssignments.hpp. </li>
   <li> Also the other algorithms in
   ComputerAlgebra/Hypergraphs/Lisp/Transversals/Minimal/RecursiveSplitting.mac
   should be generalised. </li>
   <li> See Satisfiability/Algorithms/AllSolutions/plans/general.hpp. </li>
  </ul>


  \todo Computations of all prime clauses for full clause-sets
  <ul>
   <li> Connections:
    <ol>
     <li> See "min_2resolution_closure_cs" in
     Satisfiability/Lisp/Resolution/plans/PrimeImplicatesImplicants.hpp
     for a Maxima function. </li>
     <li> See Satisfiability/FiniteFunctions/plans/QuineMcCluskey.hpp for a
     C++ function. </li>
     <li> And see Investigations/BooleanFunctions/plans/QuineMcCluskey.hpp
     for investigations. </li>
    </ol>
   </li>
   <li> Consider a full clause-set F with c=c(F) clauses: Then at most
   c*(c+1)/2 = 1/2 c^2 + 1/2 c many clauses C with var(C) <= var(F) follow from
   F:
    <ol>
     <li> Consider C with var(C) <= var(F). </li>
     <li> C follows from F iff all full clauses D >= C are elements of F.
     </li>
     <li> Let C_- be the full clause obtained from C by adding all missing
     negative literals, and lit C_+ be the full clause obtained from C by
     adding all missing positive literals. </li>
     <li> Consider - < +, and using lexicographical order, where the variables
     of C form the initial segment, we thus have that C follows from F iff the
     interval [C_-, C_+] is subset of F. </li>
     <li> The map {C clause over var(F)} -> [C_-,C_+] is injective (since we
     have C = C_- intersect C_+). </li>
     <li> It follows that the number of clauses which follow from F is at
     most the number of 1- or 2-element subsets of F, that is,
     binom(c,1) + binom(c,2). </li>
    </ol>
   </li>
   <li> Is that upper bound sharp?
    <ol>
     <li> It is not sharp if F contains all clauses. </li>
    </ol>
   </li>
   <li> Computing all prime-clauses according to the above upper bound:
    <ol>
     <li> All clauses C which follow can be found by running through all
     <=2-element-subsets {D,E} of F, determining C := D intersect E,
     and determining whether all supsets of C are in F. </li>
     <li> The supset-test can be simply done by running through all elements
     of F and counting those which contain C; this can be done in time
     c * n. </li>
     <li> Alternatively one can sort F lexicographically, and search for each
     superset of C using binary search; for very small C the first method
     should be more efficient, for the rest the second method. </li>
     <li> Let F' be the set of all clauses which follow from F. </li>
     <li> The prime-clauses are the subsumption-minimal elements of F. </li>
     <li> F' is "convex" (contains with C <= D also all clauses inbetween),
     and thus we can apply the special algorithm for computing minimal elements
     described in "Minimal elements for convex set-systems" in
     ComputerAlgebra/Hypergraphs/Lisp/plans/SetSystems.hpp. </li>
     <li> Alternatively to first compute F' and then to extract the minimal
     elements (as above), one could, when computing an new implicant C, already
     check whether also some C-{x} exists --- only if not, then the clause
     is included (it's then a prime clause). </li>
    </ol>
   </li>
  </ul>

*/


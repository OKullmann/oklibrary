/* Oliver Kullmann, 9.8.2009 (Swansea) */
/* Copyright 2009 Oliver Kullmann
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


  \todo Connections


  \todo Create milestones.


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
   ComputerAlgebra/Satisfiability/Lisp/Resolution/PrimeImplicatesImplicants.mac.
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
   ComputerAlgebra/Hypergraphs/Lisp/Transversals/Minimal/RecursiveSplitting.mac,
   implement the basic recursive splitting algorithm as discussed in
   "The basic algorithm" in
   Algorithms/AllSolutions/plans/MinimalAssignments.hpp. </li>
   <li> Also the other algorithms in
   ComputerAlgebra/Hypergraphs/Lisp/Transversals/Minimal/RecursiveSplitting.mac
   should be generalised. </li>
   <li> See Satisfiability/Algorithms/AllSolutions/plans/general.hpp. </li>
  </ul>


  \todo Bases of the set of prime clauses relative to some reduction
  <ul>
   <li> Consider some reduction r (i.e., r: CLS -> CLS with r(F)
   satisfiability-equivalent to F). </li>
   <li> A sub-clause-set F' of a clause-set F is an "r-base of F" if
   for every clause D in F - F' and for the corresponding partial assignment
   phi_D^0 the empty clause is element of r(phi_D^0 * F'), while this is not
   true for any strict subset of F'. </li>
   <li> That means that all missing clauses D can be derived from F' by means
   of r. </li>
   <li> There is exactly one r_0-base of every F, namely the result of
   subsumption elimination in F. </li>
   <li> If F is the set of prime implicates of some boolean function, which is
   too big to be included in some CNF-representation, then the hope is that
   r_1-bases or r_2-bases might be much smaller, while still having enough
   inference power. </li>
   <li> The reasoning is that if r is too strong (for example if r detects
   every unsatisfiability, then the bases of (any) F are exactly the
   irredundant cores) then we get smaller F', however we possibly gave up
   too much information, and SAT-solving might be much harder. </li>
   <li> The r_k-bases of such F are exactly the minimal (w.r.t
   subset-inclusion) r_k-based representation via CNFs not using additional
   variables (see "Investigating conditions and their representations" in
   Investigations/Cryptography/AdvancedEncryptionStandard/plans/general.hpp)
   of the underlying boolean function. </li>
   <li> DONE (we have rand_rbase_cs in
   ComputerAlgebra/Satisfiability/Lisp/Primality/RBases.mac)
   First we just need the natural randomised algorithm for producing some
   r-base.
    <ol>
     <li> We start with F' = F, while G = {} is the set of clauses which are
     necessary. </li>
     <li> Choose C at random from F' - G, and let F'' := F' - {C}. </li>
     <li> If for all D in F - F'' we have {} in r(phi_D * F''), then let
     F' := F'', and repeat with choosing another C. </li>
     <li> Otherwise G := G + {C}, and another C is tried. </li>
     <li> The algorithm stops once F' = G, and then F' is an r-basis of F.
     </li>
    </ol>
   </li>
   <li> Searching for minimum bases (that is, of minimum size) might be very
   expensive, but, of course, should be implemented. </li>
  </ul>

*/


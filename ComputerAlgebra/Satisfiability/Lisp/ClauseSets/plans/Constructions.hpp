// Oliver Kullmann, 23.2.2008 (Swansea)
/* Copyright 2008, 2009, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/ClauseSets/plans/Constructions.hpp
  \brief Plans for basic clause-set constructions


  \todo Representations of the basic logical operations
  <ul>
   <li> The problem is, given clause-sets F_1, F_2, find representations
   of F_1 and F_1, F_1 or F_2, not F_1, F_1 <-> F_2, F_1 -> F_2 and so on.
   </li>
   <li> Here we consider F_1, F_2 as CNF --- in general we consider clause-sets
   as combinatorial objects, which can represent CNF *and* DNF, but here
   it seems helpful, since otherwise perhaps no good names are available? </li>
   <li> F_1 and F_2 is the union of F_1, F_2; see "and_cs". </li>
   <li> F_1 or F_2 is or_cs(F_1, F_2). </li>
   <li> (CLS, and_cs, or_cs, {}, {{}}) is a commutative semiring with
   idempotent addition. </li>
   <li> Perhaps these rules should better be represented by the operations.
   </li>
   <li> In CLS as category, with morphisms complement-preserving homomorphisms
   for hypergraphs, it should and_cs be the coproduct, but or_cs doesn't
   look like a product. One could repair that by using as homomorphisms
   complement-preserving maps f: F -> G from literals to literals such that
   for C in F there exists D in G with D <= f(C) ?! </li>
   <li> So perhaps we should use the names "coproduct_cs" and "product_cs"
   ?! </li>
   <li> Negation
    <ol>
     <li> "dual_cs" is the clause-set obtained by full distribution. </li>
     <li> Compare with ComputerAlgebra/Hypergraphs/Lisp/Transversals/plans/general.hpp.
     </li>
     <li> Compare with Algorithms/AllSolutions/plans/MinimalAssignments.hpp.
     </li>
     <li> A semantic negation G of a clause-set F is characterised by
     comp_cs(G) being a DNF-representation of F. </li>
    </ol>
   </li>
  </ul>


  \todo Gluing operations
  <ul>
   <li> Gluing: DONE
    <ol>
     <li> DONE ("full_gluing")
     Given clause-sets F, G, use a new variable v, add v to all of
     F, add -v to all of G, and take the union. </li>
     <li> DONE ("partial_gluing")
     This is "full gluing", while for "partial gluing" v resp. -v
     is only added to some clauses of F resp. G (but in both cases to
     at least one clause). </li>
    </ol>
    Since this is some "combinatorial operation", we could first compute
    n := max(n(F),n(G)), standardise the var-sets of F, G to {1,...,n},
    and then use the new variable n+1.
   </li>
   <li> Semantically this represents ( exists(v) : F or G). </li>
   <li> DONE (we have vardisjoint_full_gluing)
   Often this operation needs variable-disjointness, so we need
   some operation to make F, G variable-disjoint. </li>
  </ul>


  \todo Strengthening dualts_fcl by adding some redundant clauses
  <ul>
   <li> Given a formal clause-list F as DNF, and a partial assignment
   phi, there are (of course) forced assignments on phi * dualts(F) which 
   are not derived by unit propagation (while if phi * dualts(F) is 
   unsatisfiable, then {} is in r_1(phi * dualts(F)) --- so it needs r_2 to
   compute all forced assignments). </li>
   <li> For example the formal DNF [[1,2,3],[{1,2},{1,3}]] has {1} as a forced
   assignment, however
   \verbatim
F : [[1,2,3],[{1,2},{1,3}]]$
G : fcs2cs(fcl2fcs(dualts_fcl(F)));
  {{-3,-1,dts(2)},{-2,-1,dts(1)},{1,-dts(1)},{1,-dts(2)},{2,-dts(1)},
   {3,-dts(2)},{dts(1),dts(2)}}
ucp_lpa_0_cs(G)[2];
   \endverbatim 
   yields [], not [{1}]. </li>
   <li> Adding for every literal x the clause (C -> x) to dualts(F), where C 
   is the conjunction of -dts(i) for every i where x does not occur in the 
   i-th clause in F, gives a representation which has that under any partial 
   assignment phi using only variables from F, all forced assignments on 
   phi * F follow by unit-clause propagation on dualts_fcl(F). </li>
   <li> More generally, adding for some (CNF-)clause D the clause (C -> D), 
   where C is the conjunction C of all negated indicator variables for clauses
   which have empty intersection with D, then for partial assignments phi
   such that D follows from phi * F, D is subsumed by one of the clauses
   of r_1(phi * F). Moreover, the added clause follows from dualts(F). </li>
   <li> Continuing the above example:
   \verbatim
G2 : union(G,
 {{dts(1),dts(2),-1},{1},{dts(1),dts(2),-2},{dts(2),2},{dts(1),dts(2),-3},{dts(1),3}})$
ucp_lpa_0_cs(G2)[2];
   \endverbatim
   correctly yields [{1}]. One sees here that only additional clauses are
   interesting where the literal x (as above) occurs in some clause; there are
   also other strong redundancies. </li>
   </li>
  </ul>

*/


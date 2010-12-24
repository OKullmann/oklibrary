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

*/


// Oliver Kullmann, 15.3.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/Backdoors/plans/general.hpp
  \brief Plans on backdoor-related algorithms


  \todo Blind search for backdoors of size k
  <ul>
   <li> strong/weak/deletion_backdoor_search(F,k,C), where
    <ol>
     <li> F is a clause-set, </li>
     <li> k a natural number, </li>
     <li> and C a unary predicate for the base class (thus C(F) is true
     iff F belongs to the "tractable" class), </li>
    </ol>
    just runs through all subsets V of size at most k (starting from below)
    of the variable-set, and returns V in case
    <ol>
     <li> "strong": for all partial assignments phi with domain V,
     vp * F is in C; </li>
     <li> "weak": there exists a partial assignment phi with domain V
     such that phi * F is satisfiable and belongs to C; </li>
     <li> "deletion": V * F is in C. </li>
    </ol>
    Otherwise false is returned.
   </li>
   <li> I (OK) expect that in general usage of r_k(U,S) (see
   Satisfiability/Lisp/Reductions/plans/GeneralisedUCP.hpp; especially
   topic "Use oracles") is more useful, since it strengthens strong, weak
   and deletion backdoors by inference (if any of those backdoors of size
   k exists, then r_k with U = C cap USAT and S = C cap SAT reduces
   F to {} or {{}}). </li>
  </ul>


  \todo 2-CNF backdoors
  <ul>
   <li> See Satisfiability/Algorithms/Backdoors/plans/2CNF.hpp. </li>
   <li> Here strong and deletion backdoors coincide (obviously), and
   only the variable-hypergraph is of importance. </li>
   <li> We should (just) implement the reduction to the hypergraph
   transversal problem (see Satisfiability/Algorithms/Backdoors/plans/2CNF.hpp).
   </li>
  </ul>


  \todo Horn backdoors
  <ul>
   <li> See Satisfiability/Algorithms/Backdoors/plans/Horn.hpp. </li>
   <li> Again, we should (just) implement the reduction to the transversal
   hypergraph problem. </li>
  </ul>


  \todo Backdoors w.r.t. clause-sets of bounded deficiency
  <ul>
   <li> The parameterised translation of the transversal problem to the
   problem of detecting a backdoor w.r.t. CLS_{delta^* <= r} (see
   [Szeider, Matched formulas and backdoor sets, SAT 2007]) should
   be implemented. </li>
   <li> Somewhere we also need to implement the various algorithms for
   clause-sets with bounded maximal deficiency. </li>
  </ul>

*/


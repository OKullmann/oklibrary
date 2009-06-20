// Oliver Kullmann, 23.3.2008 (Swansea)
/* Copyright 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Hypergraphs/Lisp/Transversals/plans/general.hpp
  \brief Plans regarding hypergraphs transversals (in Maxima/Lisp)


  \todo Connections
  <ul>
   <li> See
   Combinatorics/Hypergraphs/Transversals/Minimal/plans/DirectTransversalEnumeration.hpp
   for computing the transversal hypergraph via recursive splitting. </li>
   <li> See
   Combinatorics/Hypergraphs/Transversals/Bounded/plans/general.hpp
   for the use of a parameter bounding the size of the transversals. </li>
   <li> See especially Transversals/Bounded/BoundedTransversals_bv.cpp and
   Transversals/Bounded/VertexBranching.hpp for implementations of
   transversals_bv(s). </li>
   <li> And, more generally, see
   Combinatorics/Hypergraphs/Transversals/plans/general.hpp. </li>
   <li> See "dual_cs" in
   ComputerAlgebra/Satisfiability/Lisp/ClauseSets/Constructions.mac, and see
   ComputerAlgebra/Satisfiability/Lisp/ClauseSets/plans/Constructions.hpp.
   </li>
  </ul>


  \todo Organisation
  <ul>
   <li> Split Transversals/Transversals.mac into several files. </li>
   <li> Perhaps Transversals/Basics.mac, Transversals/Minimal.mac,
   Transversals/Bounded.mac, and Transversals/Greedy.mac. </li>
  </ul>


  \todo Using ILP
  <ul>
   <li> Finding a minimum transversal is trivially translated into an
   integer linear programming problem. </li>
   <li> Is there some ILP functionality for Maxima? </li>
   <li> See "Linear and integer programming" in
   Buildsystem/ExternalSources/SpecialBuilds/plans/Optimisation.hpp. </li>
  </ul>


  \todo Transfer
  <ul>
   <li> See "Rewrite hypergraph transversal functions in Maxima" in
   ComputerAlgebra/Mupad/plans/general.hpp. </li>
  </ul>


  \todo Testing
  <ul>
   <li> okltest_transversal_hg should be a generic test for computing
   the transversal hypergraph (used for all different methods).
   Further test-cases:
    <ol>
     <li> Claw-graphs. </li>
     <li> Complete graphs (and complete r-graphs). </li>
     <li> Graphs without edges. </li>
    </ol>
    It seems superfluous to also have a dedicated test-function for the
    independence hypergraph (i.e., that check should just transfer the
    test to the transversal-test)? See below.
   </li>
  </ul>


  \todo transversal_hg
  <ul>
   <li> Performance on complete graphs:
    <ol>
     <li> Likely this performs badly. </li>
     <li> And this likely holds for all splitting strategies. </li>
     <li> For example when adding one edge after another, starting
     with the empty edge-set and completing with all edges, there
     should be always some graph in the sequence with large transversal
     hypergraph. </li>
    </ol>
   </li>
  </ul>


  \todo The triangle of the three possibilities
  <ul>
   <li> The complementary problem is about maximal independent sets,
   the dual problem is about hyperedge coverings. </li>
   <li> Likely these problems have each at least one natural algorithm,
   different from the translations of the others. </li>
   <li> So we need to provide a general scheme for translations. </li>
  </ul>


  \todo ind_hg(G)
  <ul>
   <li> See Combinatorics/Hypergraphs/IndependentSets/plans/general.hpp. </li>
   <li> The test-function okltest_ind_hg should be a generic test-function,
   applicable to any function for computing the hypergraph of (maximal)
   independent subsets (the "independence hypergraph"). </li>
   <li> So, as with transversals, we must distinguish between the (many)
   possible algorithms for ind_hg. </li>
   <li> An alternative method for computing ind_hg is by direct
   construction:
    <ol>
     <li> Bottom-up: Start with all independent subsets of size 0.
     Then for k=0, 1, ... take the sets of size k and consider all
     possibilities to a vertex, obtain so level k+1. Those which
     cannot be extended go to the final result, while otherwise
     the processed level is removed. </li>
     <li> This algorithm is for example efficient for complete graphs. </li>
     <li> Top-down: We start with the full vertex-set at level n.
     Then for k = n,n-1, ... all subsets which are independent are
     moved to the final result, while otherwise all possible (k-1)-subsets
     are formed, which makes level k-1, and level k is removed. </li>
     <li> This algorithm is efficient for hypergraphs with no
     hyperedges. </li>
    </ol>
   </li>
   <li> Of course, a question is whether independence hypergraphs are
   really on its own, or we should standardise all considerations by
   considering transversals? Just taking the edgewise-complement is not
   a big step? But it's a step, and also the literature is different. </li>
  </ul>


  \todo Exploiting known bounds
  <ul>
   <li> See "Exploiting self-similarity" in
   Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/Transversals/plans/general.hpp
   for motivation. </li>
   <li> The aim is to improve transversals_bv, transversals_bvs by exploiting
   known bounds on localised transversal sizes. </li>
   <li> Given are a hypergraph G and sets S <= V(G) together with numbers a, b
   fulfilling a <= |S cap T| <= b for every transversal fulfilling the given
   bound |T| <= B. </li>
   <li> Then dynamically (changing with T) the intersection size s = |S cap T|
   and the number r = |S - T| of remaining vertices are updated. </li>
   <li> As soon as s > b or s + r < a holds the current T is "inconsistent".
   </li>
   <li> The algorithm invariants should avoid reaching this situation, and
   instead we should have forced assignments "one step before". </li>
   <li> That is, if s = b is reached, then all vertices in S - T need to
   be removed. </li>
   <li> While if s+r = a is reached, then all vertices in S - T need to
   be included. </li>
   <li> When branching on vertex v the following updates are performed:
    <ol>
     <li> For the branch where v is included: If v in S, then s:s+1
     and r:r-1. </li>
     <li> For the branch where v is excluded: If v in S, then r:r-1
     (this follows from the removal of v from S). </li>
     <li> In both cases, if v is not in S then nothing happens. </li>
     <li> The numbers a, b stay as they are. </li>
     <li> While S doesn't need to be updated (removal of v had no further
     (real) effect). </li>
    </ol>
   </li>
   <li> In the vdW-application S is a progression with s < n, which yields
   the lower bound, while also the complement of S is a progression, which
   yields a lower bound on the size of the complement, and thus an upper
   bound on the size of S. </li>
   <li> A question is whether one needs to have T explicitely represented?
    <ol>
     <li> The only places where S, T are needed is when all of S-T needs
     to be included resp. excluded. </li>
     <li> Perhaps one just keeps S-T up-to-date? </li>
    </ol>
   </li>
   <li> How are the sets S given?
    <ol>
     <li> In the vdW-application it is quickly computable to which S the
     current vertex v belongs. </li>
     <li> So perhaps we actually model a kind of active clause-set
     here. </li>
     <li> Only inclusions resp. exclusions need to be taken care of
     (no undoing here). </li>
     <li> So a function S is supplied, which takes as input pairs [v,e],
     where e=0 for exclusion and e=1 for inclusion (DNF-interpretation). </li>
     <li> While the output is the list of inferred clauses
     (CNF-interpretation), the empty clause for a contradiction, and unit
     clauses for forced inclusions/exclusions. </li>
     <li> So inside S unit-clause propagation must take place. </li>
     <li> The concept of "literal" must be clarified here.
      <ul>
       <li> Boolean variables are natural, with 0 for exclusion and 1 for
       inclusion. </li>
       <li> Since we want to continue working with the vertices as given,
       we work with monosigned literals, where variables have boolean domains.
       </li>
       <li> A hyperedge, say {a,b,c}, stands for {a#0,b#0,c#0}, so we have
       non-boolean CNF-clause-sets (using negative monosigned literals) ---
       but the concept of literals here also allows, e.g., "a=0". </li>
       <li> So literals are actually triples [v,e,s] (see "Notions for
       generalised literals" in
       ComputerAlgebra/Satisfiability/Lisp/ClauseSets/plans/NonBoolean.hpp),
       where e in {0,1} and s in {-1,+1}. </li>
       <li> But perhaps it is more natural for the communication with the
       active clause S to consider the literals communicated as (just)
       positive (so no need for the sign here)? However for the output this
       is not natural, and they should be negative (monosigned), if one
       does not want to use signs. </li>
       <li> Though for CNF-interpretation negative literals are more natural,
       perhaps here we just use positive literals? Then there would be no need
       for re-interpretations. </li>
       <li> Hyperedge {a,b,c} would be interpreted as {a=1,b=1,c=1}, i.e.,
       {[a,1],[b,1],[c,1]}, while the input to S is a (single) literal [v,e]
       as above, and the output is a list of clauses (of length at most 1),
       where, say, {[d,0]} would mean that d is to be exluded. </li>
       <li> Seems natural. </li>
      </ul>
     </li>
    </ol>
   </li>
  <ul>

*/


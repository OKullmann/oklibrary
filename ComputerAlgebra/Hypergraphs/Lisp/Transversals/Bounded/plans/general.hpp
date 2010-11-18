// Oliver Kullmann, 20.6.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Hypergraphs/Lisp/Transversals/Bounded/plans/general.hpp
  \brief Plans regarding finding size-bounded hypergraph transversals


  \todo Connections
  <ul>
   <li> See
   Combinatorics/Hypergraphs/Transversals/Bounded/plans/general.hpp
   for the use of a parameter bounding the size of the transversals. </li>
   <li> See especially Transversals/Bounded/BoundedTransversals_bv.cpp and
   Transversals/Bounded/VertexBranching.hpp for implementations of
   transversals_bv(s). </li>
  </ul>


  \todo Translations to SAT
  <ul>
   <li> We need translations of the problem of finding a hypergraph transversal
   of size "=B" or "<=B" to CNF-SAT-instances plus cardinality constraints.
   </li>
   <li> See "Representing cardinality constraints" in
   ComputerAlgebra/Satisfiability/Lisp/Generators/plans/CardinalityConstraints.hpp.
   </li>
   <li> Then to this "mixed problem" one can apply a translation of
   cardinality constraints to CNF-SAT, to obtain a pure CNF-SAT problem. </li>
   <li> Alternatively, one could translate it fully into a
   cardinality-constraint problem instance, while then for a CNF-translation
   of (pure) cardinality-constraint problems constraints ">= 1" (which just
   represent clauses) are handled directly? </li>
   <li> But this is a loss of information! And we need to have mixed problem
   types anyway, so we should use the mixed presentation. </li>
  </ul>


  \todo Using ILP
  <ul>
   <li> Finding a minimum transversal is trivially translated into an
   integer linear programming problem. </li>
   <li> Is there some ILP functionality for Maxima? </li>
   <li> See "Linear and integer programming" in
   Buildsystem/ExternalSources/SpecialBuilds/plans/Optimisation.hpp. </li>
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
   <li> A question is whether one needs to have T explicitly represented?
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


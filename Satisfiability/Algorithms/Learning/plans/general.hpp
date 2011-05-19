// Oliver Kullmann, 18.1.2006 (Swansea)
/* Copyright 2006 - 2007, 2008, 2009, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Algorithms/Learning/plans/general.hpp
  \brief Plans for the module on clause-learning


  \todo Develop the ideas
  <ul>
   <li> Yet we do not really understand how to combine look-ahead and
   conflict-driven solvers. </li>
   <li> The ideas expressed below are very (i.e., too) close to the (recursive)
   DLL-algorithm, and do not realise the full power of (iterative)
   clause-learning.
    <ol>
     <li> So first investigate the ideas for "pure learning algorithms" as
     discussed in
     ComputerAlgebra/Satisfiability/Lisp/Backtracking/plans/Learning.hpp. </li>
     <li> After ideas become clear, to do some more realistic experiments,
     we need C++ implementations. </li>
     <li> And the combined schemes as discussed in "Develops combinations of
     look-ahead with conflict-driven" in
     ComputerAlgebra/Satisfiability/Lisp/plans/general.hpp need to be
     developed. </li>
     <li> And then also implement them in C++. </li>
     <li> Now it should have become clear what to do with the ideas below.
     </li>
    </ol>
   </li>
  </ul>


  \todo Framework
  <ul>
   <li> The first task seems to be to determine the abstract framework (which
   components in the solving process are responsible for learning?). </li>
   <li> However, this needs to wait until the ideas of learning (and how to
   combine it with look-ahead) become clearer. </li>
  </ul>


  \todo Semantical trees
  <ul>
   <li> A natural form of learning implements directly the translation of
   semantical trees into resolution trees: At the leaves we have the clauses
   causing the conflicts, and at the inner nodes, representing branchings
   (explicit and implicit ones) we do resolution. </li>
   <li> For the OKsolver2002 this was implemented taking only the variables
   into account, no storing of clauses, and only along the current path ---
   with this a surprisingly efficient implementation of "intelligent
   backtracking" was obtained. </li>
   <li> Otherwise learning the (uncompressed) clauses seems to be a bad choice.
   </li>
   <li> Actually, for OKsolver2002 to learn the r_2-compressed decision clause
   (containing exactly the decision variables which caused the conflict) is
   very easy: Just use the current partial assignment, consider only the
   decision variables and from them only those which are mentioned in the
   variable set used for tree-pruning! </li>
   <li> OKsolver2002 does not remove the resolution-variable from the set
   of variables used for tree pruning, since these variables can never
   be used for tree pruning (due to regularity of the tree). Still we
   don't need to remove them, since to determine the learned clause one
   needs only to consider the variables in the partial assignment
   corresponding to the path *leading* to the current node. And for the
   use of this variable set in autarky search (via duality) this is also
   appropriate (since it is the set of all variables involved in the
   resolution refutatiion). </li>
  </ul>


  \todo A very attractive form of learning is "r_k-compressed learning"
  <ul>
   <li> The resolution tree corresponds to the branching tree (without forced
   decisions), taking only the decision variables into account. At the leaves
   the clause C causing the conflict is considered, and a clause C' is derived
   which consists of the decision variables responsible for falsifying C;
   with these C' we build a resolution tree. </li>
   <li> Thus the resolution tree has at most as many nodes as the branching
   tree; and we take advantage of the saving enabled by r_k-reductions! </li>
   <li> To determine the decision variables, we build a directed acyclic graph,
   with variables as the vertices, where each variable points back to the
   variables which causes its setting (the decision variables thus are the
   sources). </li>
   <li> For unit-clause-elimination this is done as follows:
    <ol>
     <li> If {x} is the unit-clause, and y_1, ...,y_k are the other literals
     in the original clause, then var(x) points back to
     var(y_1), ..., var(y_k). </li>
     <li> Given a conflict clause C, we determine all sinks v_1, ..., v_m
     reachable from var(x) for x in C, and learn the clause
       C' := { x in C : var(x) in {v_1, ..., v_m} }. </li>
    </ol>
   </li>
   <li> This form of learning uses the generalised resolution system introduced
   in my article in Annals of Artificial Intelligence and Mathematics on Lower
   bounds: Standard partial assignments and clause-sets, oracle
   U = {F : r_k(F) = {bot}}. Instead of r_k it could also be r_k^* (with local
   learning). </li>
  </ul>


  \todo Afterburner
  <ul>
   <li> If for example the main reduction is r_2, and the obtained (compressed)
   conflict-clause is {x_1, ..., x_k} (literals in the order in which the occur
   on the main path), then we can apply r_3 to the branching formula at x_1
   (which does not need to consider in the first round variables involved on
   the path before x_k) and continue, strengthening the current path, and
   potentially turning decision variables into derived variables. </li>
   <li> In this way the learned clause can be further compressed, and we have
   a reasonable scheme to apply r_3. </li>
   <li> An interesting problem here is the update of the current path
   (including the dependency graph), and making good use of the dependency
   graph to reduce the amount of work needed for r_3 (as usual, the more
   changes are done by r_3, the less we can save --- but then the reduction
   is successful). </li>
  </ul>


  \todo Re-working the tree
  <ul>
   <li> After adding a new learned clause, then potentially all nodes on the
   current path are no longer reduced anymore! </li>
   <li> Is it possible by some conflict-analysis to determine that some nodes
   might actually still be reduced? This question is similar to avoiding
   re-running reductions for r_k (if we use r_k, then adding a single
   conflict-clause seems to need r_{k+1} for the finally inferred assignment,
   and r_{k+2} for the other directions enabled by the added clause). </li>
   <li> The cleanest possibility is to always rework the tree, and (at least
   for experimental purposes) we should support this fully. </li>
  </ul>


  \todo "Intelligent backtracking"
  <ul>
   <li> A question is whether in case of "intelligent backtracking" the learned
   clauses which where found to be not needed for the resolution refutation,
   are kept or deleted: One could keep them since they were "expensive". </li>
  </ul>


  \todo Responsibility for learning
  <ul>
   <li> What kind of component is performing the learning? "Who" is
   "responsible" ? </li>
   <li> It is the backtracking algorithm which is choosing the decisions,
   managing the communication between the partial assignments and the active
   clause-sets and the alliance, and performing the backtracking; so this
   component should be responsible for *global learning* (while the active
   clause-sets might perform "local learning" themselves). </li>
   <li> It seems attractive to consider these algorithms themselves as
   constituting an active clause-set. </li>
  </ul>


  \todo Managing the learned clauses
  <ul>
   <li> The learned clauses should be managed by a special active clause-set L.
   </li>
   <li> It seems natural that L performs full subsumption elimination, but only
   for the full clauses (not considering the effect of the partial assignment).
   </li>
   <li> This should keep L relatively small, so that we can simply keep all
   learned clauses. </li>
   <li> One should look out for a special data structure for L: Since the
   clauses of L form a resolution forest (in general the leaves do not come
   from the original clause-set, for example due to subsumption or due to a
   conflict arising from a learned clause; backtracking adds a new resolvent,
   that is, an inner node, while with a new conflict we get a new leaf),
   it should make sense to use this tree structure for a more efficient
   representation of L. </li>
  </ul>


  \todo In general we do not care about the "source" of the learned clause:
  <ul>
   <li> If a partial assignment phi yields *somehow* a conflict, then we learn
   the compressed phi (only taking the decision variables into account which
   were involved in the conflict). </li>
   <li> Thus we do not get a (compressed) resolution
   refutation for the input clause-set, but this should be regarded as a
   "feature". </li>
   <li> If we really want a (compressed) resolution refutation, then we
   run the learning component in a special mode. </li>
  </ul>


  \todo Autarkies
  <ul>
   <li> For autarky-applications it is important that we know the variables
   used in the (uncompressed) resolution refutation; it seems sensible to use
   here the algorithm from the (old) OKsolver (see above):
   This algorithm yields for the current node in the search tree the set of
   variables involved in the current (first) part of the resolution tree. </li>
   <li> So while the learned clauses C use only the decision variables, for the
   determination of the set of variables used there is the full C' which
   contains also all derived variables, and we maintain var(C') (modulo
   resolution) along the current path. This set var(C') of variables is
   crossed out when searching for an autarky at first backtracking. </li>
  </ul>

*/

/*!
  \namespace OKlib::Satisfiability::Algorithms::Learning
  \brief Components for clause-learning

  Alias "Lrn".
*/

namespace OKlib {
  namespace Satisfiability {
    namespace Algorithms {
      namespace Learning {
      }
      namespace Lrn = Learning;
    }
  }
}


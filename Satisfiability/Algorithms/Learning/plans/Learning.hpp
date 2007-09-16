// Oliver Kullmann, 18.1.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Learning/plans/Learning.hpp
  \brief Plans for the module on clause-learning

  \todo A natural form of learning implements directly the translation of semantical trees
  into resolution trees: At the leaves we have the clauses causing the conflicts, and
  at the inner nodes, representing branchings (explicit and implicit ones) we do resolution.
  For the OKsolver this was implemented taking only the variables into account, no storing
  of clauses, and only along the current path --- with this a surprisingly efficient implementation
  of "intelligent backtracking" was obtained. Otherwise learning the (uncompressed) clauses
  seems to be a bad choice.

  \todo A very attractive form of learning is "r_k-compressed learning":
  The resolution tree corresponds to the branching tree (without forced decisions),
  taking only the decision variables into account. At the leaves the clause C causing
  the conflict is considered, and a clause C' is derived which consists of the decision variables
  responsible for falsifying C; with these C' we build a resolution tree. Thus the resolution tree
  has at most as many nodes as the branching tree; and we take advantage of the saving enabled
  by r_k-reductions! To determine the decision variables, we build a directed acyclic graph,
  with variables as the vertices, where each variable points back to the variables which causes
  its setting (the decision variables thus are the sources). For unit-clause-elimination this is
  done as follows: If {x} is the unit clause, and y_1, ...,y_k are the other literals in the
  original clause, then var(x) points back to var(y_1), ..., var(y_k). Given a conflict clause
  C, we determine all sinks v_1, ..., v_m reachable from var(x) for x in C, and learn the
  clause C' := { x in C : var(x) in {v_1, ..., v_m} }.
  This form of learning uses the generalised resolution system introduced in my article
  in Annals of Artificial Intelligence and Mathematics on Lower bounds:
  Standard partial assignments and clause-sets, oracle U = {F : r_k(F) = {bot}}.
  Instead of r_k it could also be r_k^* (with local learning).

  \todo Afterburner: If for example the main reduction is r_2, and the obtained (compressed)
  conflict-clause is {x_1, ..., x_k} (literals in the order in which the occur on the main path),
  then we can apply r_3 to the branching formula at x_1 (which does not need to consider in the
  first round variables involved on the path before x_k) and continue, strengthening the current
  path, and potentially turning decision variables into derived variables. In this way the learned
  clause can be further compressed, and we have a reasonable scheme to apply r_3.
  An interesting problem here is the update of the current path (including the dependency
  graph), and making good use of the dependency graph to reduce the amount of work
  needed for r_3 (as usual, the more changes are done by r_3, the less we can save --- but
  then the reduction is successful).

  \todo A question is whether in case of "intelligent backtracking" the learned clauses which where
  found to be not needed for the resolution refutation, are kept or deleted: One could keep
  them since they were "expensive".

  \todo What kind of component is performing the learning? "Who" is "responsible" ?
  It is the backtracking algorithm which is choosing the decisions, managing the communication
  between the partial assignments and the active clause-sets and the alliance, and performing
  the backtracking; so this component should be responsible for *global learning* (while
  the active clause-sets might perform "local learning" themselves). It seems attractive to
  consider these algorithms themselves as constituting an active clause-set.

  \todo The learned clauses should be managed by a special active clause-set L.
  It seems natural that L performs full subsumption elimination, but only for the
  full clauses (not considering the effect of the partial assignment). This should keep
  L relatively small, so that we can simply keep all learned clauses.
  One should look out for a special data structure for L: Since the clauses of L
  form a resolution forest (in general the leaves do not come from the original clause-set, for example
  due to subsumption or due to a conflict arising from a learned clause; backtracking adds a new
  resolvent, that is, an inner node, while with a new conflict we get a new leaf), it should make sense
  to use this tree structure for a more efficient representation of L.

  \todo In general we do not care about the "source" of the learned clause: If a partial assignment phi
  yields *somehow* a conflict, then we learn the compressed phi (only taking the decision variables
  into account which were involved in the conflict). Thus we do not get a (compressed) resolution
  refutation for the input clause-set, but this should be regarded as a "feature". If we really
  want a (compressed) resolution refutation, then we run the learning component in a special
  mode.

  \todo For autarky-applications it is important that we know the variables used in the (uncompressed)
  resolution refutation; it seems sensible to use here the algorithm from the (old) OKsolver (see above):
  This algorithm yields for the current node in the search tree the set of variables involved
  in the current (first) part of the resolution tree.

  So while the learned clauses C use only the decision variables, for the determination of the set of
  variables used there is the full C' which contains also all derived variables, and we maintain
  var(C') (modulo resolution) along the current path. This set var(C') of variables is crossed out
  when searching for an autarky at first backtracking.
*/

/*!
  \namespace OKlib::Learning
  \brief Components for clause-learning
*/

namespace OKlib {
  namespace Learning {
  }
}


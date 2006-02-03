// Oliver Kullmann, 18.1.2006 (Swansea)

/*!
  \file Learning_Plans.hpp
  \brief Plans for the module on clause-learning
  \todo A natural form of learning implements directly the translation of semantical trees
  into resolution trees: At the leaves we have the clauses causing the conflicts, and
  at the inner nodes, representing branchings (explicit and implicit ones) we do resolution.
  For the OKsolver this was implemented taking only the variables into account, no storing
  of clauses, and only along the current path --- with this a surprisingly efficient implementation
  of "intelligent backtracking" was obtained.
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
  \todo What kind of component is performing the learning? "Who" is "responsible" ?
  It is the backtracking algorithm which is choosing the decisions, managing the communication
  between the partial assignments and the active clause-sets and the alliance, and performing
  the backtracking; so this component should be responsible for *global learning* (while
  the active clause-sets migh perform "local learning" themselves. It seems attractive to
  consider these algorithms themselves as constituting an active clause-set.
  \todo The learned clauses should be managed by a special active clause-sets L.
  It seems natural that L performs full subsumption elimination, but only for the
  full clauses (not considering the effect of the partial assignment). This should keep
  L relatively small, so that we can simply keep all learned clauses.
  One should look out for a special data structure for L: Since the clauses of L
  form a resolution forest (in general the leaves do not come from the original clause-set, for example
  due to subsumption or due to a conflict arising from a learned clause; backtracking adds a new resolvent,
  that is, an inner node, while with a new conflict we get a new leaf), it should make sense to use this
  tree structure for a more efficient representation of L.
  \todo In general we do not care about the "source" of the learned clause: If a partial assignment phi
  yields *somehow* a conflict, then we learn the compressed phi (only taking the decision variables
  into account which were involved in the conflict). Thus we do not get a (compressed) resolution
  refutation for the input clause-set, but this should be regarded as a "feature". If we really
  want a (compressed) resolution refutation, then we run the learning component in a special
  mode.
*/

/*!
  \namespace OKlib::Learning
  \brief Components for clause-learning
*/

namespace OKlib {
  namespace Learning {
  }
}


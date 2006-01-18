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
  \todo What kind of component is performing the learning? "Who" is "responsible" ?
  It is the backtracking algorithm which is choosing the decisions, managing the communication
  between the partial assignments and the active clause-sets and the alliance, and performing
  the backtracking; so this component should be responsible for *global learning* (while
  the active clause-sets migh perform "local learning" themselves. It seems attractive to
  consider these algorithms themselves as constituting an active clause-set.
*/

/*!
  \namespace OKlib::Learning
  \brief Components for clause-learning
*/

namespace OKlib {
  namespace Learning {
  }
}


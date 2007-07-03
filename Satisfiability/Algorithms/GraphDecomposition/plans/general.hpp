// Oliver Kullmann, 21.11.2005 (Swansea)

/*!
  \file GraphDecomposition/plans/GraphDecomposition.hpp
  \brief Plans for the module regarding SAT decision by means of
  graph decomposition.

  \todo Literature review: SAT decision in SAT and CSP by means of
  graph decomposition (including tree decompositions).

  \todo Considering the following graphs for clause-set F with clauses as vertices, where
  clauses C, D in F, C != F, are joined iff
   - var(C) intersection var(D) not empty
   - C and D clash in at least one literal (this is cg(F))
   - C and D are resolvable (the resolution graph)
   - C and D are resolvable and the resolvent is not subsumed in F (the resolution graph with subsumption).

 \todo Considering the following graphs for clause-set F with variables as vertices, where
  variables v, w in var(F), v != w, are joined iff
   - there is C in F with v, w in var(C).

 \todo Given a clause-set with bipartite structure, "construct" the above graphs as "virtual graphs"
 in the Boost-sense. Algorithms needed for graphs:
  - Determination of the connected components (available with Boost::graph)
  - Computation of all articulation vertices (available with Boost::graph (see biconnected components))
  - Computation and Approximations of the treewidth (see module TreeDecomposition).

 \todo Statistics for benchmark formulas :
  - for each connected component :
   + n, c, for each clause-length k the number c_k
   + number of accumulation vertices
   + treewidth (approximations).
  - Perhaps monitoring, how especially successful algorithms behave regarding decomposition (perhaps they
  split "intuitively" the graphs ?!).
  - Perhaps exploring whether by branching a decomposition can be reached.

 \todo Possible heuristics exploiting decompositions:
  - Opportunistic heuristics: Together with any "standard" heuristics, just exploiting a decomposing
    graph (or when articulation points exist).
  - Replacing the "standard" heuristic by minimisation of some treewidth-approximation (which is used
    as complexity measure).
*/

/*!
  \namespace OKlib::GraphDecomposition
  \brief Module for SAT decision by means of graph decomposition
*/

namespace OKlib {
  namespace GraphDecomposition {
  }
}

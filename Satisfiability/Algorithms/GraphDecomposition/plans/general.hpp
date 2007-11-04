// Oliver Kullmann, 21.11.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file GraphDecomposition/plans/general.hpp
  \brief Plans for the module regarding SAT decision by means of
  graph decomposition.


  \todo Update namespaces.


  \todo Create milestones
  <ul>
   <li> Literature review: SAT decision in SAT and CSP by means of
   graph decomposition (including tree decompositions). </li>
   <li> See [Heule,Kullmann, 2006; Decomposing clause-sets]. </li>
   <li> Two basic approaches: Splitting graphs, and the database-algorithms
   "from the leaves". </li>
  </ul>


  \todo Considering the following graphs for clause-set F with clauses as vertices, where
  clauses C, D in F, C != F, are joined iff
   - var(C) intersection var(D) not empty (the common-variable graph cvg(F))
   - C and D clash in at least one literal (the conflict-graph cg(F))
   - C and D are resolvable (the resolution graph rg(F))
   - C and D are resolvable and the resolvent is not subsumed in F
   (the resolution-subsumption graph rsg(F)).


  \todo Considering the following graphs for clause-set F with variables as vertices, where
  variables v, w in var(F), v != w, are joined iff
   - there is C in F with v, w in var(C) (the variable-interaction graph vig(F)).


  \todo Given a clause-set with bipartite structure, "construct" the above graphs as "virtual graphs"
  in the Boost-sense. %Algorithms needed for graphs:
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


  \todo Classical tree decomposition algorithms and improvements
  <ul>
   <li> Important seems to me (OK), that the classical Yannakakis-algorithm can
   be generalised by allowing at each node a presentation of the set of restrictions
   of the satisfying total assignments to the variable set of the node by *partial*
   *multi*-assignments. </li>
   <li> In this way potentially exponential savings are achieved in representing
   the "tables". </li>
   <li> And for producing the "generalised table" at each node now a SAT-solver is
   used: Every SAT-solver, if running through the end, produces a set of partial
   assignments which is the DNF-representation of the instance; multi-assignments
   are when splitting on signed literals. </li>
   <li> Compare with module "Algorithms/AllSolutions", especially with
   "find an equivalent DNF by DPLL" in AllSolutions/plans/general.hpp. </li>
  </ul>

*/

/*!
  \namespace OKlib::GraphDecomposition
  \brief Module for SAT decision by means of graph decomposition
*/

namespace OKlib {
  namespace GraphDecomposition {
  }
}

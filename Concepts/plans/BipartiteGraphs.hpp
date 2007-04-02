// Oliver Kullmann, 24.7.2005 (Swansea)

/*!
  \file Concepts/plans/BipartiteGraphs.hpp
  \brief Plans for concepts for bipartite graphs.

  \todo Concepts for bipartite graphs (with two vertex types), transferring as much as possible the concepts from the boost graph library. Related matrices: "reduced adjacency matrices".

  \todo Operations for changing bipartite graphs: Adding / deleting edges, and adding /
  deleting vertices (from both parts), and the corresponding undo-operations.

  \todo Literal-clause graphs and vertex-hyperede graphs yield bipartite graphs; for the former
  we have special relations between the literals (via the underlying variable).

  \todo It seems, that the concept of bipartite graphs is (yet) most important as concept ("notion"),
  while models are provided only via hypergraphs and clause-sets, where the bipartite graphs
  are directly "embedded" into the data structure.

  \todo Clarify the relations to module Graphs (see Graphs/plans/Graphs.hpp).

  \todo Clarify the relation to Concepts/plans/Hypergraphs.hpp.
*/


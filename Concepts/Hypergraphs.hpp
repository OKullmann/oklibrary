// Oliver Kullmann, 26.7.2005 (Swansea)

/*!
  \file Hypergraphs.hpp
  \brief Concepts for (general) hypergraphs
  \todo Writing the basic concepts for hypergraphs, similar to the concepts for graphs
  from the Boost graph library (sequences of vertices and hyperedges, where hyperedges
  are sequences of vertices). The direct access from vertices to incident edges
  could be "outsourced" to the concept of an associated bipartite graph (the vertex-hyperedge
  bipartite graph; associated, so that update operations are reflected), but it seems better
  to only "embed" this bipartite graph into the hypergraph data structure, as the
  bipartite vertex-edge graph is embedded into the boost graph concept via the
  refinement of an "incidence graph": This seems to model better the main nature
  of that bipartite graph as a data structure; and also in this way generalisations like
  access only to hyperedges where the vertex is the "first" or the "last" vertex are
  perhaps modelled more easily (in this way we then can model directed graphs,
  and also methods like "watched literals" (such bipartite graphs are actually
  vertex-edge graphs)!). However, it should be the case
  that such "incidence hypergraphs" directly yield models of bipartite graphs via
  regarding either the vertices or the hyperedges as "left part".
  \todo Clause-sets are refinements of hypergraphs, where also application of
  partial assignments may be supported (perhaps also resolution etc. ?).
*/

#ifndef HYPERGRAPHS_okjNNh7

#define HYPERGRAPHS_okjNNh7

namespace OKlib {

  namespace Concepts {

  }

}

#endif

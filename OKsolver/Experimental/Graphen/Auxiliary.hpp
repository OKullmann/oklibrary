// Oliver Kullmann, 18.8.2003

#ifndef AUXILIARYWAECHTER

#define AUXILIARYWAECHTER

#include <iostream>

#include <boost/graph/graph_traits.hpp>

namespace Auxiliary {

  // \cite{LLS2001BGL}, page 6
  template <class Graph>
  bool is_self_loop(typename boost::graph_traits<Graph>::edge_descriptor e, const Graph& g) {
    typename boost::graph_traits<Graph>::vertex_descriptor u, v;
    u = source(e, g);
    v = target(e, g);
    return u == v;
  }

  // \cite{LLS2001BGL}, page 6
  template <class VertexDescriptor, class VertexNameMap>
  void print_vertex_name(VertexDescriptor v, VertexNameMap name_map) {
    std::cout << get(name_map, v);
  }

  // \cite{LLS2001BGL}, page 6
  template <class Graph, class TransDelayMap, class VertexNameMap>
  void print_trans_delay(typename boost::graph_traits<Graph>::edge_descriptor e, const Graph& g, TransDelayMap delay_map, VertexNameMap name_map) {
    std::cout << "trans-delay(" << get(name_map, source(e,g)) << "," << get(name_map, target(e,g)) << ") = " << get(delay_map, e);
  }

}

#endif

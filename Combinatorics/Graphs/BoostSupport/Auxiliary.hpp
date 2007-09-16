// Oliver Kullmann, 18.8.2003 (Swansea)
/* Copyright 2003 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

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

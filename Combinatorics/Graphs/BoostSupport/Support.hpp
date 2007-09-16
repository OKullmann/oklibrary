// Oliver Kullmann, 8.9.2003 (Swansea)
/* Copyright 2003 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#ifndef SUPPORTWAECHTER

#define SUPPORTWAECHTER

#include <functional>
#include <cassert>

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/graph_concepts.hpp>

#include <boost/concept_check.hpp>

#include "Concepts_Graphs.hpp"

namespace Support {

  template <class Graph>
  class edge_check : public std::unary_function<typename boost::graph_traits<Graph>::edge_descriptor, bool> {
    BOOST_CLASS_REQUIRE(Graph, Concepts_Graphs, EdgeDescriptorGraphConcept);
    typedef typename boost::graph_traits<Graph>::edge_descriptor edge_descriptor;
    typedef typename boost::graph_traits<Graph>::vertex_descriptor vertex_descriptor;
    const Graph g;
    const vertex_descriptor a, b;
  public :
    edge_check(const Graph& g, vertex_descriptor a, vertex_descriptor b) : g(g), a(a), b(b) {}
    bool operator() (edge_descriptor e) {
      return source(e,g) == a and target(e,g) == b;
    }
  };
  
  template <class Graph>
  inline edge_check<Graph> edge_checker(const Graph& g, typename boost::graph_traits<Graph>::vertex_descriptor a, typename boost::graph_traits<Graph>::vertex_descriptor b) {
    return edge_check<Graph>(g, a, b);
  }
}

#endif

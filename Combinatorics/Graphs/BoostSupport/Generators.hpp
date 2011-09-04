// Oliver Kullmann, 16.6.2003 (Swansea)
/* Copyright 2003 - 2007, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Combinatorics/Graphs/BoostSupport/Generators.hpp
  \brief Old attempt by BSc project (Kate) for generators.
  \deprecated
*/

#ifndef GENERATORSWAECHTER_jhg4erC
#define GENERATORSWAECHTER_jhg4erC

#include <utility>

#include <boost/graph/graph_traits.hpp>

#include <boost/concept_check.hpp>

#include <OKlib/Combinatorics/Graphs/BoostSupport/Concepts_Graphs.hpp>

namespace Generators {

  template <typename Graph>
  Graph complete_graph(typename boost::graph_traits<Graph>::vertices_size_type n) {
    boost::function_requires<Concepts_Graphs::VertexListSizeConstructibleGraphConcept<Graph> >();
    boost::function_requires<boost::EdgeMutableGraphConcept<Graph> >();
    boost::function_requires<boost::AssignableConcept<Graph> >();
    boost::function_requires<boost::CopyConstructibleConcept<Graph> >();
    Graph g(n);
    complete_graph_imp(g, typename boost::graph_traits<Graph>::directed_category());
    return g;
  }
  template <typename Graph>
  inline void complete_graph_imp(Graph& g, boost::directed_tag) {
    typedef typename boost::graph_traits<Graph>::vertex_iterator vertex_iterator;
    const std::pair<vertex_iterator, vertex_iterator> pi = vertices(g);
    for (vertex_iterator i = pi.first; i != pi.second; ++i)
      for (vertex_iterator j = ++vertex_iterator(i); j != pi.second; ++j) {
        add_edge(*i, *j, g);
        add_edge(*j, *i, g);
      }
  }
  template<typename Graph>
  inline void complete_graph_imp(Graph& g, boost::undirected_tag) {
    typedef typename boost::graph_traits<Graph>::vertex_iterator vertex_iterator;
    const std::pair<vertex_iterator, vertex_iterator> pi = vertices(g);
    for (vertex_iterator i = pi.first; i != pi.second; ++i)
      for (vertex_iterator j = ++vertex_iterator(i); j != pi.second; ++j)
        add_edge(*i, *j, g);
  }

}

#endif


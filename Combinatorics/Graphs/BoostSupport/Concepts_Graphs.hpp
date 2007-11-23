// Oliver Kullmann, 8.9.2003 (Swansea)
/* Copyright 2003 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#ifndef CONCEPTSGRAPHSWAECHTER
#define CONCEPTSGRAPHSWAECHTER

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/graph_concepts.hpp>

#include <boost/concept_check.hpp>

namespace Concepts_Graphs {

  template <class G>
  struct VertexListSizeConstructibleGraphConcept {
    VertexListSizeConstructibleGraphConcept();
    typedef typename boost::graph_traits<G>::vertices_size_type vertices_size_type;
    void constraints() {
      boost::function_requires<boost::VertexListGraphConcept<G> >();
      G(s);
    }
    const vertices_size_type s;
  };

  template <class G>
  struct EdgeDescriptorGraphConcept {
    EdgeDescriptorGraphConcept();
    typedef typename boost::graph_traits<G>::vertex_descriptor vertex_descriptor;
    typedef typename boost::graph_traits<G>::edge_descriptor edge_descriptor;
    void constraints() {
      boost::function_requires<boost::GraphConcept<G> >();
      static_cast<vertex_descriptor>(source(e, g));
      static_cast<vertex_descriptor>(target(e, g));
    }
    const G g;
    const edge_descriptor e;
  };
}

#endif

// Oliver Kullmann, 8.9.2003 (Swansea)

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

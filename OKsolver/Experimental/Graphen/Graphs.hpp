// Oliver Kullmann, 15.9.2003 (Swansea)

#ifndef GRAPHSWAECHTER

#define GRAPHSWAECHTER

#include <boost/graph/graph_traits.hpp>

namespace Graphs {

  class MutableAdjacencyGraph_ViaIntegers {

    typedef unsigned int Index;

    Index N;

    friend class boost::graph_traits<Graphs::MutableAdjacencyGraph_ViaIntegers>;
    class Vertices {
      Index v;
      friend class MutableAdjacencyGraph_ViaIntegers;
      Vertices(Index v) : v(v) {}
    public :
      Vertices() : v(0) {}
      friend bool operator ==(Vertices lhs, Vertices rhs) {
	return lhs.v == rhs.v;
      }
      
    };
    class Edges {
      Index l, r;
      friend class MutableAdjacencyGraph_ViaIntegers;
      Edges(Index l, Index r) : l(l), r(r) {}
    public :
      Edges() : l(0), r(0) {}
      friend bool operator ==(Edges lhs, Edges rhs) {
	return lhs.l == rhs.l and lhs.r == rhs.r;
      }
    };
    
  public :

   
    
    
  };

}

namespace boost {

  template <>
  struct boost::graph_traits<Graphs::MutableAdjacencyGraph_ViaIntegers> {
    typedef Graphs::MutableAdjacencyGraph_ViaIntegers::Vertices vertex_descriptor;
    typedef Graphs::MutableAdjacencyGraph_ViaIntegers::Edges edge_descriptor;
    typedef boost::adjacency_matrix_tag traversal_category;
  };

}

namespace Graphs {

  bool operator !=(boost::graph_traits<MutableAdjacencyGraph_ViaIntegers>::vertex_descriptor lhs, boost::graph_traits<MutableAdjacencyGraph_ViaIntegers>::vertex_descriptor rhs) {
    return !(lhs == rhs);
  }
  bool operator !=(boost::graph_traits<MutableAdjacencyGraph_ViaIntegers>::edge_descriptor lhs, boost::graph_traits<MutableAdjacencyGraph_ViaIntegers>::edge_descriptor rhs) {
    return !(lhs == rhs);
  }

}

#endif


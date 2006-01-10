// Oliver Kullmann, 10.1.2006 (Swansea)

/*!
  \file EliminationSequences_Tests.hpp
  \brief Tests for the methods for handling elimination sequences for graphs (related to the
  notion of treewidth)
*/

#ifndef ELIMINATIONSEQUENCESTESTS_8uyTre

#define ELIMINATIONSEQUENCESTESTS_8uyTre

#include <vector>
#include <cassert>

#include <boost/graph/adjacency_list.hpp>

#include "TestBaseClass.hpp"
#include "TestExceptions.hpp"

namespace OKlib {

  namespace GraphDecomposition {

    template <template <class EliminationSequence, class Graph> class Width_elimination_sequence>
    class Test_Width_elimination_sequence : public ::OKlib::TestSystem::TestBase {
    public :
      typedef Test_Width_elimination_sequence test_type;
      Test_Width_elimination_sequence() {
        insert(this);
      }
    private :
      void perform_test_trivial() {
        {
          typedef boost::adjacency_list<boost::setS, boost::listS, boost::undirectedS> graph_type;
          typedef boost::graph_traits<graph_type> graph_traits_type;
          typedef typename graph_traits_type::vertex_descriptor vertex_descriptor_type;
          typedef std::vector<vertex_descriptor_type> vector_type;
          typedef Width_elimination_sequence<vector_type, graph_type> width_elimination_sequence_type;

          {
            graph_type g;
            vector_type vec;
            width_elimination_sequence_type width;
            const vertex_descriptor_type v1 = add_vertex(g);
            vec.push_back(v1);
            OKLIB_TEST_EQUAL(width(vec, g), 0U);
          }
          {
            graph_type g;
            vector_type vec;
            width_elimination_sequence_type width;
            const vertex_descriptor_type v1 = add_vertex(g);
            const vertex_descriptor_type v2 = add_vertex(g);
            assert(add_edge(v1, v2, g).second);
            assert(num_vertices(g) == 2);
            assert(*vertices(g).first == v1);
            assert(num_edges(g) == 1);
            vec.push_back(v1); vec.push_back(v2);
            OKLIB_TEST_EQUAL(width(vec, g), 1U);
          }

        }
      }

    };
  
  }

}

#endif

// Oliver Kullmann, 10.1.2006 (Swansea)

/*!
  \file EliminationSequences.hpp
  \brief Methods for handling elimination sequences for graphs (related to the
  notion of treewidth)
*/

#ifndef ELIMINATIONSEQUENCES_hhgrrEw34

#define ELIMINATIONSEQUENCES_hhgrrEw34

#include <cassert>
#include <utility>
#include <functional>

#include <boost/utility.hpp>
#include <boost/static_assert.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/graph_concepts.hpp>

namespace OKlib {

  namespace GraphDecomposition {

    /*!
      \class Width_elimination_sequence
      \brief Functor computing the width induced by an elimination sequence

      Width_elimination_sequence<C,G>()(elimination_sequence, g) returns the width
      associated with the elimination sequence (which must be non-empty).
      g is changed by the procedure.

      C must be a container of vertex descriptors of g.
      The graph class G must fulfil the following concepts:
       - boost::AdjacencyGraphConcept (see BOOST_CLASS_REQUIRE below)
       - boost::MutableGraphConcept (see BOOST_CLASS_REQUIRE below)
       - has boost::graph_traits<G>::degree_size_type
       - undirected (see BOOST_STATIC_ASSERT below)
       - no parallel edges (see BOOST_STATIC_ASSERT below)
       - no loops
       - applying add_edge(u,v,g) does not invalidate (or change in any way) adjacency_iterator's
         in the range adjacent_vertices(w,g) for w different from u and v, and neither does it
         invalidate vertex descriptors
       - applying clear_vertex(v,g) does not invalidate other vertex descriptors
       - applying remove_vertex(v,g) does not invalidate other vertex descriptors
    */

    template <class EliminationSequence, class Graph>
    class Width_elimination_sequence : public std::binary_function<const EliminationSequence&, Graph&, typename boost::graph_traits<Graph>::degree_size_type> {

      typedef boost::graph_traits<Graph> graph_traits_type;

      BOOST_CLASS_REQUIRE(Graph, boost, AdjacencyGraphConcept);
      BOOST_CLASS_REQUIRE(Graph, boost, MutableGraphConcept);

      BOOST_STATIC_ASSERT((boost::is_convertible<typename graph_traits_type::directed_category, boost::undirected_tag>::value));
      BOOST_STATIC_ASSERT((boost::is_convertible<typename graph_traits_type::edge_parallel_category, boost::disallow_parallel_edge_tag>::value));
      BOOST_STATIC_ASSERT((boost::is_convertible<typename EliminationSequence::value_type, typename graph_traits_type::vertex_descriptor>::value));

    public :

      typedef EliminationSequence elimination_sequence_type;
      typedef Graph graph_type;

    private :

      typedef typename graph_traits_type::vertex_descriptor vertex_descriptor_type;
      typedef typename graph_traits_type::adjacency_iterator adjacency_iterator_type;
      typedef typename graph_traits_type::degree_size_type degree_size_type;

    public :

      degree_size_type operator() (const elimination_sequence_type& elim, graph_type& g) const {
        assert(not elim.empty());
        degree_size_type width = 0;
        typedef typename elimination_sequence_type::const_iterator container_iterator_t;
        const container_iterator_t& end_elim(elim.end());
        for (container_iterator_t v_it(elim.begin()); v_it != end_elim; ++v_it) {
          const vertex_descriptor_type& v(*v_it);
          const std::pair<adjacency_iterator_type, adjacency_iterator_type>& neighbours(adjacent_vertices(v, g));
          degree_size_type deg = 0;
          for (adjacency_iterator_type i(neighbours.first); i != neighbours.second; ++i, ++deg) {
            assert(*i != v);
            for (adjacency_iterator_type j(boost::next(i)); j != neighbours.second; ++j)
              add_edge(*i, *j, g);
          }
          assert(deg == degree(v, g));
          if (deg > width)
            width = deg;
          clear_vertex(v, g);
          remove_vertex(v, g);
        }
        return width;
      }
    };
  }
}

#endif

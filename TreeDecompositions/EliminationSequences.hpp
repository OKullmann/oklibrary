// Oliver Kullmann, 10.1.2006 (Swansea)

/*!
  \file EliminationSequences.hpp
  \brief Methods for handling elimination sequences for graphs (related to the
  notion of treewidth)
  \todo Test Treewidth_by_enumerating_elimination_sequences; and generalise it
  by using strategy objects.
  \tod Extend Width_elimination_sequence by computing also the tree decompositon.
  \todo Implement the greedy strategy for computing an elimination sequence.
*/

#ifndef ELIMINATIONSEQUENCES_hhgrrEw34

#define ELIMINATIONSEQUENCES_hhgrrEw34

#include <cassert>
#include <utility>
#include <functional>
#include <vector>
#include <algorithm>

#include <boost/range/value_type.hpp>
#include <boost/range/const_iterator.hpp>
#include <boost/range/empty.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/utility.hpp>
#include <boost/static_assert.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/graph_concepts.hpp>
#include <boost/iterator/counting_iterator.hpp>
#include <boost/iterator/transform_iterator.hpp>
#include <boost/range/iterator_range.hpp>

namespace OKlib {

  namespace GraphDecomposition {

    /*!
      \class Width_elimination_sequence
      \brief Functor computing the width induced by an elimination sequence

      "Width_elimination_sequence<C,G>()(elimination_sequence, g)" returns the width
      associated with the elimination sequence (which must be non-empty, since the graph
      with no vertices has tree width -1, while the return degree_size_type is unsigned).
      g is changed by the procedure.

      C is a single pass range of vertex descriptors.
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
      BOOST_STATIC_ASSERT((boost::is_convertible<typename boost::range_value<EliminationSequence>::type, typename graph_traits_type::vertex_descriptor>::value));

    public :

      typedef EliminationSequence elimination_sequence_type;
      typedef Graph graph_type;

    private :

      typedef typename graph_traits_type::vertex_descriptor vertex_descriptor_type;
      typedef typename graph_traits_type::adjacency_iterator adjacency_iterator_type;
      typedef typename graph_traits_type::degree_size_type degree_size_type;

    public :

      degree_size_type operator() (const elimination_sequence_type& elim, graph_type& g) const {
        assert(not boost::empty(elim));
        degree_size_type width = 0;
        typedef typename boost::range_const_iterator<elimination_sequence_type>::type container_iterator_t;
        const container_iterator_t& end_elim(boost::end(elim));
        for (container_iterator_t v_it(boost::begin(elim)); v_it != end_elim; ++v_it) {
          const vertex_descriptor_type& v(*v_it);
          const std::pair<adjacency_iterator_type, adjacency_iterator_type>& neighbours(adjacent_vertices(v, g));
          degree_size_type deg = 0;
          for (adjacency_iterator_type i(boost::begin(neighbours)); i != boost::end(neighbours); ++i, ++deg) {
            assert(*i != v);
            for (adjacency_iterator_type j(boost::next(i)); j != boost::end(neighbours); ++j)
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

    // ##################################################

    template <class Graph>
    struct Treewidth_by_enumerating_elimination_sequences : public std::unary_function<const Graph&, typename boost::graph_traits<Graph>::vertices_size_type> {

      typedef boost::graph_traits<Graph> graph_traits_type;

      BOOST_CLASS_REQUIRE(Graph, boost, AdjacencyGraphConcept);
      BOOST_CLASS_REQUIRE(Graph, boost, VertexListGraphConcept);
      BOOST_CLASS_REQUIRE(Graph, boost, MutableGraphConcept);

      BOOST_STATIC_ASSERT((boost::is_convertible<typename graph_traits_type::directed_category, boost::undirected_tag>::value));
      BOOST_STATIC_ASSERT((boost::is_convertible<typename graph_traits_type::edge_parallel_category, boost::disallow_parallel_edge_tag>::value));

    public :

      typedef Graph graph_type;

    private :

      typedef typename graph_traits_type::vertex_descriptor vertex_descriptor_type;
      typedef typename graph_traits_type::vertex_iterator vertex_iterator_type;
      typedef typename graph_traits_type::vertices_size_type vertices_size_type;

      template <class Container, typename Index>
      class DeIndex : public std::unary_function<typename Container::value_type, Index> {
        const Container& container;
      public :
        DeIndex(const Container& container) : container(container) {}
        Index operator() (Index i) const {
          return container[i];
        }
      };

    public :

      vertices_size_type operator() (const graph_type& g) const {
        const std::pair<vertex_iterator_type, vertex_iterator_type>& vertex_range(vertices(g));
        assert(not boost::empty(vertex_range));
        typedef std::vector<vertex_descriptor_type> vertex_vector_type;
        vertex_vector_type vertex_vector(boost::begin(vertex_range), boost::end(vertex_range)); // inefficient, but unavoidable, since no index concept(!)
        typedef typename vertex_vector_type::size_type index_type;
        typedef std::vector<index_type> index_vector_type;
        const index_type& num_vertices(vertex_vector.size());
        assert(num_vertices >= 1);
        index_vector_type index_vector(boost::counting_iterator<index_type>(0), boost::counting_iterator<index_type>(num_vertices));
        typedef typename index_vector_type::const_iterator index_iterator_type;
        const index_iterator_type& begin_index_vector(index_vector.begin());
        const index_iterator_type& end_index_vector(index_vector.end());

        vertices_size_type treewidth = num_vertices - 1;
        do {
          typedef DeIndex<vertex_vector_type,  index_type> de_index_type;
          typedef boost::transform_iterator<de_index_type, index_iterator_type> transform_iterator_type;
          typedef boost::iterator_range<transform_iterator_type> elimination_sequence_type;
          graph_type g_copy(g);
          const vertices_size_type& new_width(Width_elimination_sequence<elimination_sequence_type, graph_type>()(elimination_sequence_type(transform_iterator_type(begin_index_vector, de_index_type(vertex_vector)), transform_iterator_type(end_index_vector, de_index_type(vertex_vector))), g_copy));
          if (new_width < treewidth)
            treewidth = new_width;
        } while (std::next_permutation(index_vector.begin(), index_vector.end()));
        return treewidth;
      }
    };

  }
}

#endif

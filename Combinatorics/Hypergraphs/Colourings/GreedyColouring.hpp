// Oliver Kullmann, 15.12.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Hypergraphs/Colourings/GreedyColouring.hpp
  \brief Components related to the greedy colouring algorithm

  \todo Complete doxygen documentation.
*/

#ifndef GREEDYCOLOURING_hHHbswtT6t4
#define GREEDYCOLOURING_hHHbswtT6t4

#include <functional>
#include <ostream>
#include <utility>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cassert>

#include <boost/graph/sequential_vertex_coloring.hpp>
#include <boost/iterator/counting_iterator.hpp>
#include <boost/property_map/property_map.hpp>

#include <OKlib/General/Combinatorics.hpp>

namespace OKlib {
  namespace HypergraphColouring {

    /*!
      \class Out_degree_order
      \brief Order predicate to compare vertex descriptors according to their degrees
    */

    template <class Graph>
    struct Out_degree_order : std::binary_function<typename boost::graph_traits<Graph>::vertex_descriptor, typename boost::graph_traits<Graph>::vertex_descriptor, bool> {
      typedef Graph graph_type;
      typedef typename boost::graph_traits<Graph>::vertex_descriptor vertex_descriptor;
      const graph_type& g;
      explicit Out_degree_order(const graph_type& g) : g(g) {}
      bool operator ()(const vertex_descriptor a, const vertex_descriptor b) {
        return out_degree(a, g) < out_degree(b, g);
      }
    };

    //! listing all vertex degrees
    template <class Graph>
    void output_vertex_degrees(const Graph& g, std::ostream& out) {
      typedef typename boost::graph_traits<Graph>::vertex_iterator vertex_iterator;
      typedef std::pair<vertex_iterator, vertex_iterator> vertex_range;
      const vertex_range& r(vertices(g));
      const vertex_iterator& end(r.second);
      for (vertex_iterator i = r.first; i != end; ++i)
        out << get(boost::vertex_name, g, *i) << " : " << out_degree(*i, g) << "\n";
    }

    /*!
      \class Greedy_colouring
      \brief Evaluation of greedy colouring for a graph
    */

    template <class UndirectedGraph>
    struct Greedy_colouring {

      typedef UndirectedGraph graph_type;

      typedef typename boost::graph_traits<graph_type>::vertex_descriptor vertex_descriptor;
      typedef typename boost::graph_traits<graph_type>::vertices_size_type vertices_size_type;
      typedef typename boost::graph_traits<graph_type>::vertex_iterator vertex_iterator;

      //! property map type from vertex_descriptor to vertex_index_type
      typedef typename boost::property_map<graph_type, boost::vertex_index_t>::const_type vertex_index_map;
      typedef typename boost::property_traits<vertex_index_map>::value_type vertex_index_type;

      typedef std::vector<vertex_descriptor> VertexVector;
      typedef std::pair<vertex_iterator, vertex_iterator> VertexRange;

      const graph_type& g;
      const vertices_size_type n;

      typedef ::OKlib::HypergraphColouring::Out_degree_order<graph_type> sort_type;
      sort_type sort;

      typedef unsigned long int count_type;
      typedef std::vector<count_type> hash_vector_type;
      //! hash_orders[i] = number of vertex-orders using (exactly) i colours
      hash_vector_type hash_orders;

      //! from indices to vertex_descriptor: given_order[i] is the vertex-descriptor with index i
      const VertexVector given_order;
  
      VertexVector optimal_order, worst_order;

      typedef vertices_size_type colour_type;
      //! property map for mapping vertex_descriptor to colour_type
      typedef boost::iterator_property_map<colour_type*, vertex_index_map> colour_map_type;

      //! the colour for vertex-indices
      typedef std::vector<colour_type> colour_vector_type;

      //! underlying store for the colour-map "colour"; colour_vec[i] is the current colour of the vertex with index i
      colour_vector_type colour_vec;
      //! running colouring-map for g
      colour_map_type running_colour;
      // number of colours used by running_colour
      colour_type num_colours;

      //! the maps from vertex indices to colours representing optimal resp. worst colourings
      colour_vector_type optimal_colouring, worst_colouring;
      colour_type min_colours, max_colours;

      //! running order, associated with running_colour
      VertexVector running_order;

      //! index-type for the vertices in given_order
      typedef typename VertexVector::size_type order_index_type;
      //! permutations of given_order
      typedef std::vector<order_index_type> permutation_type;

      // ----------------------------------------------------------------------

      //! fills hash_orders and optimal_order, worst_order together with associated optimal_colouring, worst_colouring
      explicit Greedy_colouring(const graph_type& g) :
        g(g),
        n(checked_size(g)),
        sort(g),
        hash_orders(n+1),
        given_order(vertices(g).first, vertices(g).second),
        colour_vec(n),
        running_colour(&colour_vec.front(), get(boost::vertex_index, g)),
        min_colours(n+1),
        max_colours(0),
        running_order(n)
      {}

      void evaluation() {
        permutation_type permutation(boost::counting_iterator<order_index_type>(0), boost::counting_iterator<order_index_type>(n));
        do {
          set_order(permutation);
          colouring();
          assert(num_colours <= n);
          if (num_colours < min_colours) {
            optimal_colouring = colour_vec; min_colours = num_colours; optimal_order = running_order;
          }
          else if (num_colours > max_colours) {
            worst_colouring = colour_vec; max_colours =num_colours; worst_order = running_order;
          }
          ++hash_orders[num_colours];
        } while (std::next_permutation(permutation.begin(), permutation.end()));
        assert(optimal_order.size() == n);
        assert(optimal_colouring.size() == n);
        assert(worst_order.size() == n);
        assert(worst_colouring.size() == n);
        assert(std::accumulate(hash_orders.begin(), hash_orders.end(), (count_type)0) == Combinatorics::factorial(n));
      }
      
      void set_order(const permutation_type& p) {
        for (order_index_type i = 0; i != n; ++i) {
          assert(p[i] < n);
          running_order[i] = given_order[p[i]];
        }
      }
      void colouring() {
        num_colours = sequential_vertex_coloring(g, &running_order[0], running_colour);
      }
      
    private :
      
      vertices_size_type checked_size(const graph_type& g) {
        const vertices_size_type n(num_vertices(g));
        assert(n > 0);
        return n;
      }
      
    };

  }
}

#endif

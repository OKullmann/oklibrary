// Oliver Kullmann, 25.11.2006 (Swansea)
/* Copyright 2006 - 2007, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file QuantumPhysics/OrthogonalTriples.cpp
  \brief Handling orthogonal triples in 3-dimensional Euclidean space

  Reads from standard input a list of vectors in 3-dimensional space (first entry the number of vectors,
  then triplets for each vector; everything space-separated), and outputs
   - first the graph, whose vertices are the vectors, joined by an edge if they are orthogonal
   - then all orthogonal triplets
   - followed by those edges not contained in some triplet
   - and finally the SAT-encoding (DIMACS) of assigning boolean values to the vertices so that
     every triplet has exactly 2 ones, while in no remaining edge we have 2 zeros.
*/

#include <utility>
#include <vector>
#include <iostream>
#include <limits>
#include <cassert>
#include <cstdlib>
#include <tr1/array>

#include <boost/rational.hpp>
#include <boost/range/size.hpp>
#include <boost/range/size_type.hpp>
#include <boost/range/const_iterator.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/utility.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/adjacency_matrix.hpp>
#include <boost/property_map/property_map.hpp>

// Some linear algebra #################################

template <class Vector>
inline void vector_product(Vector& p, const Vector& a, const Vector& b) {
  assert(a.size() == 3);
  assert(b.size() == 3);
  assert(p.size() == 3);
  p(0) = a(1) * b(2) - a(2) * b(1);
  p(1) = a(2) * b(0) - a(0) * b(2);
  p(2) = a(0) * b(1) - a(1) * b(0);
}
template <class Vector>
inline Vector vector_product(const Vector& a, const Vector& b) {
  Vector p(3);
  vector_product(p, a, b);
  return p;
}

template <class Vector>
inline bool orthogonal(const Vector& a, const Vector& b) {
  typedef typename Vector::value_type value_type;
  return std::abs(inner_prod(a, b)) <= 0.000001; // HACK --- only for double
}

// typedef int integer_type;
// typedef boost::rational<integer_type> rational_number_type;
// typedef boost::numeric::ublas::vector<rational_number_type> vector_type;
typedef boost::numeric::ublas::vector<double> vector_type;
typedef std::vector<vector_type> vector2_type;

// Input and output ########################################

bool read(vector2_type& ev) {
  unsigned int m;
  std::cin >> m;
  if (not std::cin) return false;
  ev.reserve(m);
  for (unsigned int i = 0; i < m; ++i) {
    vector_type v(3);
    std::cin >> v[0] >> v[1] >> v[2];
    if (not std::cin) return false;
    ev.push_back(v);
  }
  return true;
}

template <class Graph>
void output_graph(const Graph& g) {
  std::cout << num_vertices(g) << " " << num_edges(g) << "\n\n";
  typedef typename boost::graph_traits<Graph>::edge_iterator edge_iterator;
  typedef std::pair<edge_iterator, edge_iterator> edge_range;
  const edge_range& r(edges(g));
  for (edge_iterator i = r.first; i != r.second; ++i)
    std::cout << source(*i, g) << " " << target(*i, g) << "\n";
}
template <class Graph, class EliminatedEdges, class Hypergraph>
void output_remaining_edges(const Graph& g, const EliminatedEdges& map, const Hypergraph& G) {
  typedef typename boost::graph_traits<Graph>::edge_iterator edge_iterator;
  typedef std::pair<edge_iterator, edge_iterator> edge_range;
  const edge_range& r(edges(g));
  {
    typedef typename boost::graph_traits<Graph>::edges_size_type edges_size_type;
    edges_size_type count = 0;
    for (edge_iterator i = r.first; i != r.second; ++i)
      count += not get(map, *i);
    assert(count == num_edges(g) - 3 * G.size());
    std::cout << count << "\n";
  }
  for (edge_iterator i = r.first; i != r.second; ++i)
    if (not get(map, *i))
      std::cout << source(*i, g) << " " << target(*i, g) << "\n";
}

template <class Hypergraph>
void output_hypergraph(const Hypergraph& G) {
  std::cout << G.size() << "\n";
  typedef typename Hypergraph::const_iterator const_iterator;
  const const_iterator& end(G.end());
  for (const_iterator i = G.begin(); i != end; ++i) {
    typedef typename Hypergraph::value_type value_type;
    typedef typename value_type::const_iterator const_iterator2;
    const const_iterator2& end2(i -> end());
    for (const_iterator2 j = i -> begin(); j != end2; ++j)
      std::cout << *j << " ";
    std::cout << "\n";
  }
}

/*!
  \class SAT_translation
  \brief Creating a CNF in DIMACS format encoding the 101-colouring conditions
*/

template <class Graph>
struct SAT_translation {
  typedef typename boost::graph_traits<Graph>::vertex_descriptor vertex_descriptor;
  typedef typename boost::property_map<Graph, boost::vertex_index_t>::const_type property_map_index;
  typedef typename boost::graph_traits<Graph>::vertices_size_type vertices_size_type;

  const Graph& g;
  const property_map_index index;

  SAT_translation(const Graph& g) : g(g), index(get(boost::vertex_index, g)) {}
  
  int var(const vertex_descriptor v) const {
    const vertices_size_type n(get(index, v));
    assert(n < static_cast<unsigned int>(std::numeric_limits<int>::max()));
    return n + 1;
  }

  template <class Hypergraph, class Map>
  void operator()(const Hypergraph& h, const Map& m) const {
    std::cout << "c Kochen-Specker paradox; generated by OKlibrary (2006)\n";
    std::cout << "c " << num_vertices(g) << " vertices, " << h.size() << " orthogonal triples, " << num_edges(g) - 3 * h.size() << " (remaining) orthogonal pairs\n";
    std::cout << "p " << num_vertices(g) << " " << h.size() + num_edges(g) << "\n";
    {
      typedef typename Hypergraph::const_iterator const_iterator;
      const const_iterator& end(h.end());
      for (const_iterator i(h.begin()); i != end; ++i) {
        typedef typename Hypergraph::value_type value_type;
        const value_type& H(*i);
        typedef typename value_type::const_iterator const_iterator2;
        const const_iterator2& end2(H.end());
        for (const_iterator2 j(H.begin()); j != end2; ++j)
          std::cout << - var(*j) << " ";
        std::cout << "0\n";
        for (const_iterator2 j(H.begin()); j != end2; ++j)
          for (const_iterator2 k(boost::next(j)); k != end2; ++k)
            std::cout << var(*j) << " " << var(*k) << " 0\n";
      }
    }
    {
      typedef typename boost::graph_traits<Graph>::edge_iterator edge_iterator;
      typedef std::pair<edge_iterator, edge_iterator> edge_range;
      const edge_range& r(edges(g));
      for (edge_iterator i = r.first; i != r.second; ++i)
        if (not get(m, *i))
          std::cout << var(source(*i, g)) << " " << var(target(*i, g)) << " 0\n";
    }
  }
};

// Graph constructions #################################

/*!
  \brief The function for computing the orthogonality graph
*/

template <class Graph, class Range>
  // Graph must be undirected
inline Graph orthogonality_relation(const Range& r) {
  typedef std::pair<unsigned int, unsigned int> edge_type;
  typedef std::vector<edge_type> edge_vector_type;
  edge_vector_type ev;

  typedef typename boost::range_size<Range>::type size_type;
  typedef typename boost::range_const_iterator<Range>::type const_iterator;
  const const_iterator& end(boost::const_end(r));
  size_type i = 0;
  for (const_iterator ip(boost::const_begin(r)); ip != end; ++ip, ++i) {
    size_type j = i+1;
    for (const_iterator jp(boost::next(ip)); jp != end; ++jp, ++j) {
      if (orthogonal(*ip, *jp)) ev.push_back(edge_type(i, j));
    }
  }
  Graph g(ev.begin(), ev.end(), boost::size(r));
  assert(num_vertices(g) == boost::size(r));
  assert(num_edges(g) == ev.size());
  return g;
}

/*!
  \brief The function for extracting all orthogonal triples
*/

template <class AdjacencyGraph, class Hypergraph, class EdgeContained>
void extract_triangles(const AdjacencyGraph& g, Hypergraph& hg, EdgeContained& map) {
  typedef typename boost::graph_traits<AdjacencyGraph>::vertex_iterator vertex_iterator;
  typedef std::pair<vertex_iterator, vertex_iterator> vertex_range;
  const vertex_range& r(vertices(g));
  for (vertex_iterator i = r.first; i != r.second; ++i)
    for (vertex_iterator j = boost::next(i); j != r.second; ++j)
      for (vertex_iterator k = boost::next(j); k != r.second; ++k) {
        typedef typename boost::graph_traits<AdjacencyGraph>::edge_descriptor edge_descriptor;
        typedef std::pair<edge_descriptor, bool> returned_edge_type;
        const returned_edge_type& e1(edge(*i, *j, g));
        const returned_edge_type& e2(edge(*i, *k, g));
        const returned_edge_type& e3(edge(*j, *k, g));
        if (e1.second and e2.second and e3.second) {
          typedef typename boost::graph_traits<AdjacencyGraph>::vertex_descriptor vertex_descriptor;
          typedef std::tr1::array<vertex_descriptor, 3> Hyperedge;
          Hyperedge H = {{*i, *j, *k}};
          typedef typename Hypergraph::value_type value_type;
          hg.push_back(value_type(H.begin(), H.end()));
          put(map, e1.first, true); put(map, e2.first, true); put(map, e3.first, true);
        }
      }
}

/*!
  \brief Copying a graph
*/

template <class MatrixGraph, class ListGraph>
MatrixGraph adapt(const ListGraph& g) {
  MatrixGraph mg(num_vertices(g)); // BOOST ERROR: boost::adjacency_matrix does NOT fulfill concept IteratorConstructibleGraph
  typedef typename boost::graph_traits<ListGraph>::edge_iterator edge_iterator;
  typedef std::pair<edge_iterator, edge_iterator> edge_range;
  const edge_range& r(edges(g));
  for (edge_iterator i(r.first); i != r.second; ++i)
    add_edge(source(*i, g), target(*i, g), mg);
  return mg;
}


// Main type defintions

typedef std::vector<unsigned int> Hyperedge;
typedef std::vector<Hyperedge> Hypergraph;

typedef boost::adjacency_list<boost::listS, boost::vecS, boost::undirectedS> UndirectedGraph;
typedef boost::adjacency_matrix<boost::undirectedS, boost::no_property, boost::property<boost::edge_index_t, bool> > UndirectedGraphM;

int main () {

    vector2_type list;
    if (not read(list))
      return EXIT_FAILURE;

    const UndirectedGraph g(orthogonality_relation<UndirectedGraph>(list));
    output_graph(g);
    std::cout << "\n";

    UndirectedGraphM g_m(adapt<UndirectedGraphM>(g));
    assert(num_vertices(g_m) == num_vertices(g));
    assert(num_edges(g_m) == num_edges(g));
    Hypergraph G;

    typedef boost::property_map<UndirectedGraphM, boost::edge_index_t>::type edge_property_type;
    edge_property_type ep(get(boost::edge_index, g_m)); // because of this operation g_m cannot be const???
    {
      typedef boost::graph_traits<UndirectedGraphM>::edge_iterator edge_iterator;
      typedef std::pair<edge_iterator, edge_iterator> edge_range;
      const edge_range& r(edges(g_m));
      for (edge_iterator i = r.first; i != r.second; ++i)
        put(ep, *i, false);
    }
    extract_triangles(g_m, G, ep);
    output_hypergraph(G);
    output_remaining_edges(g_m, ep, G);
    std::cout << "\n";

    (SAT_translation<UndirectedGraphM>(g_m))(G, ep);
}

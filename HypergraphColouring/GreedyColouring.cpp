// Oliver Kullmann, 24.11.2006 (Swansea)

/*!
  \file HypergraphColouring/GreedyColouring.cpp
  \brief Application for performing greedy graph colouring (prototype)

  The program reads from standard input the number n of vertices and the
  number m of edges, and then the m edges as pairs of vertices (everything
  just separated by spaces), where vertices are numbers in {0, ..., n-1}.
  It prints out n and m, and then for i from 0 to n the number of vertex orderings
  resulting in using exactly i colours by greedy colouring; finally the number
  of colours when using the standard vertex ordering (in ascending order) is output.
*/

#include <utility>
#include <vector>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <cassert>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/sequential_vertex_coloring.hpp>
#include <boost/iterator/counting_iterator.hpp>

#include <Transitional/General/Combinatorics.hpp>


typedef boost::adjacency_list<boost::listS, boost::vecS, boost::undirectedS> UndirectedGraph;
typedef boost::graph_traits<UndirectedGraph>::vertex_descriptor vertex_descriptor;
typedef boost::graph_traits<UndirectedGraph>::vertices_size_type vertices_size_type;
typedef boost::graph_traits<UndirectedGraph>::vertex_iterator vertex_iterator;
typedef boost::property_map<UndirectedGraph, boost::vertex_index_t>::const_type vertex_index_map;

typedef std::pair<int, int> Edge;
typedef std::vector<Edge> EdgeVector;
typedef std::vector<vertex_descriptor> VertexVector;
typedef std::pair<vertex_iterator, vertex_iterator> VertexRange;

unsigned int read(EdgeVector& ev) {
  unsigned int n, m;
  std::cin >> n >> m;
  for (unsigned int i = 0; i < m; ++i) {
    int source, target;
    std::cin >> source >> target;
    ev.push_back(Edge(source,target));
  }
  return n;
}


int main() {

  EdgeVector ev;
  const unsigned int n = read(ev);
  std::cout << "\n" << n << ", " << ev.size() << std::endl;

  const UndirectedGraph g(ev.begin(), ev.end(), n);
  assert(n == num_vertices(g));
  const VertexRange& r(vertices(g));
  const VertexVector given_order(r.first, r.second);
  VertexVector order; order.reserve(n);

  std::vector<unsigned int> permutation(boost::counting_iterator<unsigned int>(0), boost::counting_iterator<unsigned int>(n));
  std::vector<unsigned int> hash_orders(n+1);

  std::vector<vertices_size_type> colour_vec(n);
  boost::iterator_property_map<vertices_size_type*, vertex_index_map> colour(&colour_vec.front(), get(boost::vertex_index, g));

  assert(not given_order.empty());
  do {
    for (unsigned int i = 0; i != n; ++i)
      order[i] = given_order[permutation[i]];
    const vertices_size_type num_colours = sequential_vertex_coloring(g, &order[0], colour);
    assert(num_colours <= n);
    ++hash_orders[num_colours];
  } while (std::next_permutation(permutation.begin(), permutation.end()));
  assert(std::accumulate(hash_orders.begin(), hash_orders.end(), (unsigned int)0) == Combinatorics::factorial(n));

  for (unsigned int i = 0; i <= n; ++i)
    std::cout << i << " : " << hash_orders[i] << "\n";
  std::cout << sequential_vertex_coloring(g, colour) << "\n";
}


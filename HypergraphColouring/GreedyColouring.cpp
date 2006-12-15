// Oliver Kullmann, 24.11.2006 (Swansea)

/*!
  \file HypergraphColouring/GreedyColouring.cpp
  \brief Application for performing greedy graph colouring (prototype)

  The program reads from standard input the number n of vertices and the
  number m of edges, and then the m edges as pairs of vertices (everything
  just separated by spaces), where vertices are numbers in {0, ..., n-1}.
  It prints out n and m and the vertex degrees, followed by an analysis of the generic
  colouring algorithm for the input graph.
*/

#include <iostream>
#include <cassert>

#include <boost/graph/adjacency_list.hpp>

#include <Transitional/HypergraphColouring/GreedyColouring.hpp>

int main() {

  //! undirected graphs with out-edges stored in lists and the vertices stored in a vector (to enable index-access)
  typedef boost::adjacency_list<boost::listS, boost::vecS, boost::undirectedS> UndirectedGraph;

  OKlib::HypergraphColouring::EdgeVector ev;
  const unsigned int n = OKlib::HypergraphColouring::read(ev);
  std::cout << "\n" << n << ", " << ev.size() << std::endl;

  const UndirectedGraph g(ev.begin(), ev.end(), n);
  assert(n == num_vertices(g));
  OKlib::HypergraphColouring::output_vertex_degrees(g, std::cout);
  std::cout << std::endl;

  OKlib::HypergraphColouring::Full_greedy_colouring<UndirectedGraph> fgg(g);
  assert(fgg.n == n);

  for (unsigned int i = 0; i <= n; ++i)
    std::cout << i << " : " << fgg.hash_orders[i] << "\n";
  std::cout << std::endl;
  std::cout << "min = " << fgg.min_colours << std::endl;
  for (unsigned int i = 0; i < n; ++i)
    std::cout << fgg.optimal_order[i] << " ";
  std::cout << std::endl;
  for (unsigned int i = 0; i < n; ++i)
    std::cout << i << " -> " << fgg.optimal_colouring[i] << ", ";
  std::cout << std::endl;
  std::cout << "max = " << fgg.max_colours << std::endl;
  for (unsigned int i = 0; i < n; ++i)
    std::cout << fgg.worst_order[i] << " ";
  std::cout << std::endl;
  for (unsigned int i = 0; i < n; ++i)
    std::cout << i << " -> " << fgg.worst_colouring[i] << ", ";
  std::cout << std::endl;

  fgg.running_order = fgg.given_order;
  fgg.colouring();
  std::cout << "standard order = " << fgg.num_colours << std::endl;
  for (unsigned int i = 0; i < n; ++i)
    std::cout << fgg.running_order[i] << " ";
  std::cout << std::endl;
  for (unsigned int i = 0; i < n; ++i)
    std::cout << i << " -> " << fgg.colour_vec[i] << ", ";
  std::cout << std::endl;
  
  std::stable_sort(fgg.running_order.begin(), fgg.running_order.end(), fgg.sort);
  fgg.colouring();
  std::cout << "smallest degrees first = " << fgg.num_colours << std::endl;
  for (unsigned int i = 0; i < n; ++i)
    std::cout << fgg.running_order[i] << " ";
  std::cout << std::endl;
  for (unsigned int i = 0; i < n; ++i)
    std::cout << i << " -> " << fgg.colour_vec[i] << ", ";
  std::cout << std::endl;

  std::reverse(fgg.running_order.begin(), fgg.running_order.end());
  fgg.colouring();
  std::cout << "largest degrees first = " << fgg.num_colours << std::endl;
  for (unsigned int i = 0; i < n; ++i)
    std::cout << fgg.running_order[i] << " ";
  std::cout << std::endl;
  for (unsigned int i = 0; i < n; ++i)
    std::cout << i << " -> " << fgg.colour_vec[i] << ", ";
  std::cout << std::endl;

}


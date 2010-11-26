// Oliver Kullmann, 24.11.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Hypergraphs/Colourings/GreedyColouring.cpp
  \brief Application for performing greedy graph colouring (prototype)

  The program reads from standard input an undirected graph in dot-format.
  It prints the number of vertices, the number of edges, and all vertex degrees,
  followed by an analysis of the generic colouring algorithm for the input graph.

  If no command-line argument is given, then only the lexicographical order and
  the max- and min-degree order are evaluated, while otherwise all orders are
  evaluated.

*/

#include <iostream>
#include <cassert>
#include <string>
#include <map>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/property_map/dynamic_property_map.hpp>


#include <OKlib/Combinatorics/Hypergraphs/Colourings/GreedyColouring.hpp>

int main(const int argc, const char* const argv[]) {

  //! undirected graphs with out-edges stored in lists and the vertices stored in a vector (to enable index-access)
  typedef boost::adjacency_list<boost::listS, boost::vecS, boost::undirectedS, boost::property<boost::vertex_name_t, std::string> > UndirectedGraph;
  typedef boost::graph_traits<UndirectedGraph>::vertex_descriptor vertex_type;
  typedef boost::graph_traits<UndirectedGraph>::vertices_size_type vertices_size_type;
  typedef boost::graph_traits<UndirectedGraph>::edges_size_type edges_size_type;

  UndirectedGraph g;
  {
    boost::dynamic_properties p;
    p.property("node_id", get(boost::vertex_name, g));
    boost::read_graphviz(std::cin, g, p);
  }
  
  const vertices_size_type n = num_vertices(g);
  const edges_size_type m = num_edges(g);
  
  std::cout << "\n" << n << ", " << m << std::endl;
  OKlib::HypergraphColouring::output_vertex_degrees(g, std::cout);
  std::cout << std::endl;

  OKlib::HypergraphColouring::Greedy_colouring<UndirectedGraph> fgg(g);
  assert(fgg.n == n);

  {
    typedef std::map<std::string, vertex_type> name_map_type;
    name_map_type map;
    for (vertices_size_type i = 0; i < n; ++i)
      map.insert(std::make_pair(get(boost::vertex_name, g, fgg.given_order[i]), fgg.given_order[i]));
    assert(map.size() == n);
    typedef name_map_type::const_iterator iterator;
    const iterator end(map.end());
    vertices_size_type i = 0;
    for (iterator j(map.begin()); j != end; ++i, ++j)
      fgg.running_order[i] = j -> second;
  }
  fgg.colouring();
  std::cout << "lexicographical order = " << fgg.num_colours << std::endl;
  for (vertices_size_type i = 0; i < n; ++i)
    std::cout << get(boost::vertex_name, g, fgg.running_order[i]) << " ";
  std::cout << std::endl;
  for (vertices_size_type i = 0; i < n; ++i)
    std::cout << get(boost::vertex_name, g, fgg.running_order[i]) << " -> " << fgg.colour_vec[get(boost::vertex_index, g, fgg.running_order[i])] << ", ";
  std::cout << std::endl;
  
  std::stable_sort(fgg.running_order.begin(), fgg.running_order.end(), fgg.sort);
  fgg.colouring();
  std::cout << "smallest degrees first = " << fgg.num_colours << std::endl;
  for (vertices_size_type i = 0; i < n; ++i)
    std::cout << get(boost::vertex_name, g, fgg.running_order[i]) << " ";
  std::cout << std::endl;
  for (vertices_size_type i = 0; i < n; ++i)
    std::cout << get(boost::vertex_name, g, fgg.running_order[i]) << " -> " << fgg.colour_vec[get(boost::vertex_index, g, fgg.running_order[i])] << ", ";
  std::cout << std::endl;
  
  std::reverse(fgg.running_order.begin(), fgg.running_order.end());
  fgg.colouring();
  std::cout << "largest degrees first = " << fgg.num_colours << std::endl;
  for (vertices_size_type i = 0; i < n; ++i)
    std::cout << get(boost::vertex_name, g, fgg.running_order[i]) << " ";
  std::cout << std::endl;
  for (vertices_size_type i = 0; i < n; ++i)
    std::cout << get(boost::vertex_name, g, fgg.running_order[i]) << " -> " << fgg.colour_vec[get(boost::vertex_index, g, fgg.running_order[i])] << ", ";
  std::cout << std::endl;
  
  if (argc > 1) {
    fgg.evaluation();

    std::cout << "\n";
    for (vertices_size_type i = 0; i <= n; ++i)
      std::cout << i << " : " << fgg.hash_orders[i] << "\n";
    std::cout << std::endl;
    
    std::cout << "min numbers of colours = " << fgg.min_colours << std::endl;
    for (vertices_size_type i = 0; i < n; ++i)
      std::cout << get(boost::vertex_name, g, fgg.optimal_order[i]) << " ";
    std::cout << std::endl;
    for (vertices_size_type i = 0; i < n; ++i)
      std::cout <<  get(boost::vertex_name, g, fgg.optimal_order[i]) << " -> " << fgg.optimal_colouring[get(boost::vertex_index, g, fgg.optimal_order[i])] << ", ";
    std::cout << std::endl;
    
    std::cout << "max number of colours = " << fgg.max_colours << std::endl;
    for (vertices_size_type i = 0; i < n; ++i)
      std::cout <<  get(boost::vertex_name, g, fgg.worst_order[i]) << " ";
    std::cout << std::endl;
    for (vertices_size_type i = 0; i < n; ++i)
      std::cout << get(boost::vertex_name, g, fgg.worst_order[i]) << " -> " << fgg.worst_colouring[get(boost::vertex_index, g, fgg.worst_order[i])] << ", ";
    std::cout << std::endl;
  }

}


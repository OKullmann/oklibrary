// Oliver Kullmann, 26.11.2007 (Swansea)
/* Copyright 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Graphs/Isomorphisms/SimpleBacktracking.cpp
  \brief Application for isomorphism testing of graphs via the Boost backtracking implementation

  Needs two filenames for dot-graphs as parameters.

*/

#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include <vector>
#include <utility>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/property_map/dynamic_property_map.hpp>
#include <boost/graph/isomorphism.hpp>

int main(const int argc, const char* const argv[]) {

  if (argc != 3) {
    std::cerr << "ERROR[SimpleBacktracking]: Exactly two parameters are needed, the filenames for the two graphs.\n";
    return EXIT_FAILURE;
  }

  typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::property<boost::vertex_name_t, std::string> > UndirectedGraph;
  typedef boost::graph_traits<UndirectedGraph>::vertex_descriptor vertex_type;
  typedef boost::graph_traits<UndirectedGraph>::vertices_size_type vertices_size_type;
  typedef boost::graph_traits<UndirectedGraph>::edges_size_type edges_size_type;

  UndirectedGraph ga, gb;
  vertices_size_type n;

  { // Reading
    const std::string grapha_s(argv[1]);
    const std::string graphb_s(argv[2]);
    { // ga
      std::ifstream grapha_f(grapha_s.c_str());
      if (not grapha_f) {
        std::cerr << "ERROR[SimpleBacktracking]: Can't read from first file \"" << grapha_s << "\".\n";
        return EXIT_FAILURE;
      }
      {
        boost::dynamic_properties p;
        p.property("node_id", get(boost::vertex_name, ga));
        boost::read_graphviz(grapha_f, ga, p);
      }
      if (not grapha_f) {
        std::cerr << "ERROR[SimpleBacktracking]: Error while reading from first file \"" << grapha_s << "\".\n";
        return EXIT_FAILURE;
      }
    }
    { // gb
      std::ifstream graphb_f(graphb_s.c_str());
      if (not graphb_f) {
        std::cerr << "ERROR[SimpleBacktracking]: Can't read from second file \"" << graphb_s << "\".\n";
        return EXIT_FAILURE;
      }
      {
        boost::dynamic_properties p;
        p.property("node_id", get(boost::vertex_name, gb));
        boost::read_graphviz(graphb_f, gb, p);
      }
      if (not graphb_f) {
        std::cerr << "ERROR[SimpleBacktracking]: Error while reading from second file \"" << graphb_s << "\".\n";
        return EXIT_FAILURE;
      }
    }
    n = num_vertices(ga);
    std::cout << "First graph (\"" << grapha_s << "\"):\n " << num_vertices(ga) << " vertices, " << num_edges(ga) << " edges.\n";
    std::cout << "Second graph (\"" << graphb_s << "\"):\n " << num_vertices(gb) << " vertices, " << num_edges(gb) << " edges.\n";
  }
 
  if (n != num_vertices(gb)) {
    std::cout << "Graphs non-isomorphic due to different number of vertices.\n";
    return 0;
  }
  std::cout << "Isomorphism testing started." << std::endl;
  typedef std::vector<boost::graph_traits<UndirectedGraph>::vertex_descriptor> map_type;
  map_type f(n);
  const bool result = boost::isomorphism(ga, gb, boost::isomorphism_map(boost::make_iterator_property_map(f.begin(), get(boost::vertex_index, ga), f[0])));
  std::cout << "Result = " << result << ".\n";
  if (result) {
    typedef boost::graph_traits<UndirectedGraph>::vertex_iterator vertex_iterator;
    typedef std::pair<vertex_iterator, vertex_iterator> vertex_range;
    const vertex_range& r(vertices(ga));
    const vertex_iterator& end(r.second);
    for (vertex_iterator i = r.first; i != end; ++i)
      std::cout << get(boost::vertex_name, ga, *i) << " -> " << get(boost::vertex_name, gb, f[get(boost::vertex_index, ga, *i)]) << "\n";
  }
}


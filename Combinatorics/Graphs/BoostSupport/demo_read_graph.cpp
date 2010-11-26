// Oliver Kullmann, 23.11.2007 (Swansea)
/* Copyright 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Graphs/BoostSupport/demo_read_graph.cpp
  \brief Demonstrating reading a graph in the "DOT" language

  Reads from standard input, prints the number of vertices and the
  number of edges, and outputs the graph to standard output.
  If no command-line argument is provided, then an undirected graph is
  expected, otherwise a directed graph.


  \todo It seems that boost::read_graphviz handles vertex names always
  as strings, and thus the dynamic property map needs to contain at least
  the map node_id?

  \todo Print out the vertex names.

  \todo The doxygen-output of structure "process" is complete nonsense.

*/

#include <iostream>
#include <string>

#include <boost/graph/graphviz.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/property_map/dynamic_property_map.hpp>

namespace {

  template <class Graph>
  struct process {
    Graph g;
    boost::dynamic_properties p;
    process() {
      p.property("node_id", get(boost::vertex_name, g));
      boost::read_graphviz(std::cin, g, p);
      std::cout << num_vertices(g) << ", " << num_edges(g) << "\n";
      boost::write_graphviz_dp(std::cout, g, p); 
    }
  };
}

int main(const int argc, const char* const argv[]) {
  if (argc == 1) {
    typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::property<boost::vertex_name_t, std::string> > Graph;
    process<Graph>();
  }
  else {
    typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::property<boost::vertex_name_t, std::string> > Graph;
    process<Graph>();
  }
}


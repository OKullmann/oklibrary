// Oliver Kullmann, 16.6.2003 (Swansea)
/* Copyright 2003 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

// #include <iostream>
#include <cassert>
// #include <utility>

#include <boost/graph/adjacency_list.hpp>
// #include <boost/graph/graph_utility.hpp>
// #include <boost/graph/graph_traits.hpp>

#include "Generators.hpp"
#include "Tests_Generators.hpp"
#include "Graphs.hpp"

typedef boost::adjacency_list<boost::listS, boost::vecS, boost::undirectedS> simple_undirected_graph;

int max_size_complete_graph = 10;

int main() {
  try {
    Tests_Generators::complete_graph<simple_undirected_graph>(max_size_complete_graph); 
  }
  catch(...) {

  }
}


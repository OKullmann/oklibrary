// Oliver Kullmann, 16.6.2003

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


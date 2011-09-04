// Oliver Kullmann, 16.6.2003 (Swansea)
/* Copyright 2003 - 2007, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Graphs/BoostSupport/Test.cpp
  \deprecated Old version of tests (incomplete)
*/

#include <boost/graph/adjacency_list.hpp>

#include "Generators.hpp"
#include "Tests_Generators.hpp"
#include "Graphs.hpp"

namespace {

  typedef boost::adjacency_list<boost::listS, boost::vecS, boost::undirectedS> simple_undirected_graph;

  const int max_size_complete_graph = 10;

}

int main() {
  Tests_Generators::complete_graph<simple_undirected_graph>(max_size_complete_graph);
}


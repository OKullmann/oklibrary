// Oliver Kullmann, 8.9.2003 (Swansea)
/* Copyright 2003 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#ifndef TESTSGENERATORSWAECHTER

#define TESTSGENERATORSWAECHTER

#include <cassert>

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/graph_concepts.hpp>

#include <boost/concept_check.hpp>
#include <boost/type_traits.hpp>

#include "Generators.hpp"
#include "Support.hpp"

namespace Tests_Generators {

  template<typename Graph>
  void complete_graph(typename boost::graph_traits<Graph>::vertices_size_type max_size) {

    boost::function_requires<boost::EdgeListGraphConcept<Graph> >();

    typedef typename boost::graph_traits<Graph>::vertices_size_type vertices_size_type;
    {
      Graph g(Generators::complete_graph<Graph>(0));
      assert(boost::num_vertices(g) == 0);
      assert(boost::num_edges(g) == 0);
      g = Generators::complete_graph<Graph>(1);
      assert(boost::num_vertices(g) == 1);
      assert(boost::num_edges(g) == 0);
    }
    for (vertices_size_type n = 2; n <= max_size; ++n) {
      Graph g(Generators::complete_graph<Graph>(n));
      assert(boost::num_vertices(g) == n);
      typedef typename boost::graph_traits<Graph>::directed_category directed_category;
      if (boost::is_same<directed_category, boost::directed_tag>::value)
	assert(boost::num_edges(g) == n * (n-1));
      else if (boost::is_same<directed_category, boost::undirected_tag>::value)
	assert(boost::num_edges(g) == (n * (n-1))/2);
      else {
	// THROW XXXXXXXXXXXXXXXXXXXXXX
      }
      typedef typename boost::graph_traits<Graph>::vertex_iterator vertex_iterator;
      typedef typename boost::graph_traits<Graph>::edge_iterator edge_iterator;
      const std::pair<vertex_iterator, vertex_iterator> pvi = boost::vertices(g);
      const std::pair<edge_iterator, edge_iterator> pei = boost::edges(g);
      for (vertex_iterator i = pvi.first; i != pvi.second; ++i)
	for (vertex_iterator j = ++vertex_iterator(i); j != pvi.second; ++j)
	  if (boost::is_same<directed_category, boost::directed_tag>::value) {
	    assert(std::find_if(pei.first, pei.second, Support::edge_checker(g, *i, *j)) != pei.second);
	    assert(std::find_if(pei.first, pei.second, Support::edge_checker(g, *j, *i)) != pei.second);
	  }
	  else
	    assert(std::find_if(pei.first, pei.second, Support::edge_checker(g, *i, *j)) != pei.second);
    }
  }
}


#endif

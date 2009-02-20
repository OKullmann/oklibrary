// Oliver Kullmann, 10.1.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Combinatorics/Graphs/TreeDecompositions/EliminationSequences_Tests.hpp
  \brief Tests for the methods for handling elimination sequences for graphs (related to the
  notion of treewidth)

*/

#ifndef ELIMINATIONSEQUENCESTESTS_8uyTre
#define ELIMINATIONSEQUENCESTESTS_8uyTre

#include <vector>
#include <cassert>

#include <OKlib/TestSystem/TestBaseClass.hpp>
#include <OKlib/TestSystem/TestExceptions.hpp>

namespace OKlib {
  namespace GraphDecomposition {

    template <template <class EliminationSequence, class Graph> class Width_elimination_sequence, class Graph>
    class Test_Width_elimination_sequence : public ::OKlib::TestSystem::TestBase {
    public :
      typedef Test_Width_elimination_sequence test_type;
      Test_Width_elimination_sequence() {
        insert(this);
      }
    private :
      typedef Graph graph_type;
      void perform_test_trivial() {
        {
          typedef boost::graph_traits<graph_type> graph_traits_type;
          typedef typename graph_traits_type::vertex_descriptor vertex_descriptor_type;
          typedef std::vector<vertex_descriptor_type> vector_type;
          typedef Width_elimination_sequence<vector_type, graph_type> width_elimination_sequence_type;

          {
            graph_type g;
            vector_type vec;
            width_elimination_sequence_type width;
            const vertex_descriptor_type v1 = add_vertex(g);
            vec.push_back(v1);
            OKLIB_TEST_EQUAL(width(vec, g), 0U);
          }
          {
            graph_type g;
            vector_type vec;
            width_elimination_sequence_type width;
            const vertex_descriptor_type v1 = add_vertex(g);
            const vertex_descriptor_type v2 = add_vertex(g);
            add_edge(v1, v2, g);
            vec.push_back(v1); vec.push_back(v2);
            OKLIB_TEST_EQUAL(width(vec, g), 1U);
          }
          {
            graph_type g;
            vector_type vec;
            width_elimination_sequence_type width;
            const unsigned int path_size = 20;
            vec.reserve(path_size);
            assert(path_size >= 1);
            for (unsigned int i = 0; i < path_size; ++i)
              vec.push_back(add_vertex(g));
            for (unsigned int i = 0; i < path_size-1; ++i)
              add_edge(vec[i], vec[i+1], g);
            OKLIB_TEST_EQUAL(width(vec, g), 1U);
          }
          {
            graph_type g;
            vector_type vec;
            width_elimination_sequence_type width;
            const unsigned int cycle_length = 20;
            vec.reserve(cycle_length);
            assert(cycle_length >= 1);
            for (unsigned int i = 0; i < cycle_length; ++i)
              vec.push_back(add_vertex(g));
            for (unsigned int i = 0; i < cycle_length-1; ++i)
              add_edge(vec[i], vec[i+1], g);
            add_edge(vec[cycle_length-1], vec[0], g);
            OKLIB_TEST_EQUAL(width(vec, g), 2U);
          }
          {
            graph_type g;
            vector_type vec;
            width_elimination_sequence_type width;
            const unsigned int clique_size = 20;
            vec.reserve(clique_size);
            assert(clique_size >= 1);
            for (unsigned int i = 0; i < clique_size; ++i)
              vec.push_back(add_vertex(g));
            for (unsigned int i = 0; i < clique_size; ++i)
              for (unsigned int j = i+1; j < clique_size; ++j)
                add_edge(vec[i], vec[j], g);
            OKLIB_TEST_EQUAL(width(vec, g), clique_size - 1);
          }
        }
      }

    };
  
  }

}

#endif

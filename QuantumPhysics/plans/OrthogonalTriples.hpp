// Oliver Kullmann, 22.11.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file QuantumPhysics/plans/OrthogonalTriples.hpp
  \brief Plans regarding the elementary algorithms for handling orthogonal triples in 3-dimensional Euclidean space

  \todo The following todo's are about the reusable components in
  QuantumPhysics/OrthogonalTriples.cpp, which should be moved to appropriate
  library files (where they are tested).

  \todo Output CNFs
  SAT_translation should use the components in InputOutput.

  \todo Input and output of graphs
  With output_graph and read in HypergraphColouring/GreedyColouring.cpp we have
  a simple text format: There is also a simple format used in yEd? Perhaps this is
  better; we should have the possibilities for comments, and perhaps for vertex
  and edge properties. This all shold go into a dedicated graph-module.

  \todo Input and Output
  Use Messages and ProgramOptions.

  \todo Linear algebra
  Move the components
   - vector_product (not in Boost::Ublas ?)
   - orthogonal (checking needs to be improved)
  to appropriate modules, and test them.

  \todo Hypergraphs
  In output_remaining_edges, output_hypergraph and extract_triangles a certain interface
  for hypergraphs is used --- compare this with Concepts/plans/Hypergraphs.hpp.

  \todo Orthogonality relation
  The function orthogonality_relation should be generalised, so that
  an arbitrary binrary predicate can be used.

  \todo Extract triangles
  The function extract_triangles should go into the general graph/hypergraph module.
  Are there better algorithms?

  \todo Copying graphs
  Why does

  #include <boost/graph/copy.hpp>
  UndirectedGraphM g_m(num_vertices(g));
  boost::copy_graph(g, g_m);

  not work??? Perhaps the problem is, that boost::copy_graph also wants to set the property in g_m ???

  \todo We need a new function "completion", which, given a point configuration, adds the unique
  orthogonal direction to every pair which is not contained in a triple. For the Peres 33-point
  configuration one then needs to check, whether the 24 pairs yield 24 new points (likely
  not). The completion happens via the vector product, and another function checks whether
  two directions given by two vectors are equal or not.

*/

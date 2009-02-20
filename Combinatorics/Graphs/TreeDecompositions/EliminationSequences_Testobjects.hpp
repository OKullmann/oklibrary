// Oliver Kullmann, 10.1.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#ifndef ELIMINATIONSEQUENCESTESTOBJECTS_098Uyt
#define ELIMINATIONSEQUENCESTESTOBJECTS_098Uyt

#include <boost/graph/adjacency_list.hpp>

#include <OKlib/Combinatorics/Graphs/TreeDecompositions/EliminationSequences.hpp>
#include <OKlib/Combinatorics/Graphs/TreeDecompositions/EliminationSequences_Tests.hpp>

namespace OKlib {
  namespace GraphDecomposition {

    Test_Width_elimination_sequence<Width_elimination_sequence, boost::adjacency_list<boost::setS, boost::listS, boost::undirectedS> > test_width_elimination_sequence;

  }
}

#endif

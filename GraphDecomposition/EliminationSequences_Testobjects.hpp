// Oliver Kullmann, 10.1.2006 (Swansea)

#ifndef ELIMINATIONSEQUENCESTESTOBJECTS_098Uyt

#define ELIMINATIONSEQUENCESTESTOBJECTS_098Uyt

#include <boost/graph/adjacency_list.hpp>

#include "EliminationSequences.hpp"
#include "EliminationSequences_Tests.hpp"

namespace OKlib {

  namespace GraphDecomposition {

    Test_Width_elimination_sequence<Width_elimination_sequence, boost::adjacency_list<boost::setS, boost::listS, boost::undirectedS> > test_width_elimination_sequence;

  }

}

#endif

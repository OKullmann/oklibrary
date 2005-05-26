// Oliver Kullmann, 25.5.2005

#ifndef ITERATORSTESTOBJECTS_oPlN5Rtf

#define ITERATORSTESTOBJECTS_oPlN5Rtf

#include "Iterators.hpp"
#include "Iterators_Tests.hpp"

namespace OKlib {

  namespace Concepts {

    namespace Archetype_tests {

      template class MultiPassInputIterator<MultiPassInputIterator_Archetype<int> >;

    }

  }

}

#endif

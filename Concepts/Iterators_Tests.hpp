// Oliver Kullmann, 15.5.2005

#ifndef ITERATORSTESTS_818tBgah

#define ITERATORSTESTS_818tBgah

#include "Iterators.hpp"

namespace OKlib {

  namespace Concepts {

    namespace Archetype_tests {

      template class MultiPassInputIterator<MultiPassInputIterator_Archetype<int> >;

    }

  }

}

#endif

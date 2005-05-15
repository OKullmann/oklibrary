// Oliver Kullmann, 15.5.2005

#ifndef STDITERATORSTESTS_oopl5Bb

#define STDITERATORSTESTS_oopl5Bb

#include "std_Iterators.hpp"

namespace OKlib {

  namespace Concepts {

    namespace Archetype_tests {

      template class InputIterator<InputIterator_Archetype<int> >;

    }

  }

}

#endif

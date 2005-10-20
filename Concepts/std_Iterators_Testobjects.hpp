// Oliver Kullmann, 25.5.2005

#ifndef STDITERATORSTESTOBECTS_638usuDq

#define STDITERATORSTESTOBECTS_638usuDq

#include "LibraryBasics_Tests.hpp"

#include "std_Iterators.hpp"
#include "std_Iterators_Tests.hpp"

namespace OKlib {

  namespace Concepts {

    namespace Archetype_tests {

      OKLIB_BASIC_CONCEPT_TEST_TAG(InputIterator)
      OKLIB_BASIC_CONCEPT_TEST_ARCHETYPE(InputIterator)

      OKLIB_BASIC_CONCEPT_TEST_TAG(MultiPassInputIterator)
      OKLIB_BASIC_CONCEPT_TEST_ARCHETYPE(MultiPassInputIterator)

      OKLIB_BASIC_CONCEPT_TEST_TAG(ForwardIterator)
      OKLIB_BASIC_CONCEPT_TEST_ARCHETYPE(ForwardIterator)

    }

  }

}

#endif

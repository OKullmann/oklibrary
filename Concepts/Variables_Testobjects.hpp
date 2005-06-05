// Oliver Kullmann, 26.5.2005

#ifndef VARIABLESTESTOBJECTS_olKmNb56

#define VARIABLESTESTOBJECTS_olKmNb56

#include "LibraryBasics_Tests.hpp"

#include "Variables.hpp"
#include "Variables_Tests.hpp"

namespace OKlib {

  namespace Concepts {

    OKLIB_BASIC_CONCEPT_TEST_TAG(Variables);
    OKLIB_BASIC_CONCEPT_TEST_ARCHETYPE(Variables);

    OKLIB_BASIC_CONCEPT_TEST_TAG(VariablesWithIndex);
    OKLIB_BASIC_CONCEPT_TEST_ARCHETYPE(VariablesWithIndex);

    OKLIB_BASIC_CONCEPT_TEST_TAG(VariablesAsIndex);
    OKLIB_BASIC_CONCEPT_TEST_ARCHETYPE(VariablesAsIndex);

  }

}

#endif

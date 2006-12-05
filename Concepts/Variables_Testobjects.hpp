// Oliver Kullmann, 26.5.2005

/*!
  \file Concepts/Variables_Testobjects.hpp
  \brief Checking whether concept and archetype definitions are consistent

  \todo Move to the new test system.
*/

#ifndef VARIABLESTESTOBJECTS_olKmNb56
#define VARIABLESTESTOBJECTS_olKmNb56

#include <Transitional/Concepts/LibraryBasics_Tests.hpp>

#include <Transitional/Concepts/Variables.hpp>
#include <Transitional/Concepts/Variables_Tests.hpp>

namespace OKlib {

  namespace Concepts {

    OKLIB_BASIC_CONCEPT_TEST_TAG(Variables)
    OKLIB_BASIC_CONCEPT_TEST_ARCHETYPE(Variables)

    OKLIB_BASIC_CONCEPT_TEST_TAG(VariablesWithIndex)
    OKLIB_BASIC_CONCEPT_TEST_ARCHETYPE(VariablesWithIndex)

    OKLIB_BASIC_CONCEPT_TEST_TAG(VariablesAsIndex)
    OKLIB_BASIC_CONCEPT_TEST_ARCHETYPE(VariablesAsIndex)

  }

}

#endif

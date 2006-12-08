// Oliver Kullmann, 6.12.2006

/*!
  \file Concepts/testobjects/AtomicConditions.cpp
  \brief Checking whether concept and archetype definitions are consistent
*/

#include <Transitional/Concepts/LibraryBasics_Tests.hpp>

#include <Transitional/Concepts/AtomicConditions.hpp>

namespace OKlib {
  namespace Concepts {
    
  OKLIB_BASIC_CONCEPT_TEST_TAG(OKlib::Concepts::AtomicCondition)
  OKLIB_BASIC_CONCEPT_TEST_ARCHETYPE(OKlib::Concepts::AtomicCondition)

  }
}


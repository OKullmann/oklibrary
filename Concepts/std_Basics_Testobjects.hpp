// Oliver Kullmann, 25.5.2005 (Swansea)

#ifndef STDBASICSTESTOBJECTS_kcXzw235

#define STDBASICSTESTOBJECTS_kcXzw235

#include "LibraryBasics_Tests.hpp"

#include "std_Basics.hpp"
#include "std_Basics_Tests.hpp"

namespace OKlib {

  namespace Concepts {

    OKLIB_BASIC_CONCEPT_TEST_TAG(EqualityComparable)
    OKLIB_BASIC_CONCEPT_TEST_ARCHETYPE(EqualityComparable)

    OKLIB_BASIC_CONCEPT_TEST_ARCHETYPE(LessThanComparable)
    OKLIB_BASIC_CONCEPT_TEST_TAG(LessThanComparable)

    OKLIB_BASIC_CONCEPT_TEST_ARCHETYPE(Destructible)
    OKLIB_BASIC_CONCEPT_TEST_TAG(Destructible)

    OKLIB_BASIC_CONCEPT_TEST_ARCHETYPE(CopyConstructible)
    OKLIB_BASIC_CONCEPT_TEST_TAG(CopyConstructible)

    OKLIB_BASIC_CONCEPT_TEST_ARCHETYPE(DefaultConstructible)
    OKLIB_BASIC_CONCEPT_TEST_TAG(DefaultConstructible)

    OKLIB_BASIC_CONCEPT_TEST_ARCHETYPE(Assignable)
    OKLIB_BASIC_CONCEPT_TEST_TAG(Assignable)

  }

}

#endif

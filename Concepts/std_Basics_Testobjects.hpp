// Oliver Kullmann, 25.5.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#ifndef STDBASICSTESTOBJECTS_kcXzw235
#define STDBASICSTESTOBJECTS_kcXzw235

#include <OKlib/Concepts/LibraryBasics_Tests.hpp>

#include <OKlib/Concepts/std_Basics.hpp>
#include <OKlib/Concepts/std_Basics_Tests.hpp>

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

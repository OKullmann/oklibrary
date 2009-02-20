// Oliver Kullmann, 11.6.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#ifndef RESULTELEMENTTESTOBJECTS_957yBhF
#define RESULTELEMENTTESTOBJECTS_957yBhF

#include <OKlib/Concepts/LibraryBasics_Tests.hpp>

#include <OKlib/Concepts/ResultElements.hpp>
#include <OKlib/Concepts/ResultElements_Tests.hpp>

namespace OKlib {

  namespace Concepts {

    OKLIB_BASIC_CONCEPT_TEST_TAG(ResultElement)
    OKLIB_BASIC_CONCEPT_TEST_ARCHETYPE(ResultElement)

    OKLIB_BASIC_CONCEPT_TEST_TAG(ResultElementWithOrder)
    OKLIB_BASIC_CONCEPT_TEST_ARCHETYPE(ResultElementWithOrder)

    OKLIB_BASIC_CONCEPT_TEST_TAG(ResultElementWithName)
    OKLIB_BASIC_CONCEPT_TEST_ARCHETYPE(ResultElementWithName)

  }

}

#endif

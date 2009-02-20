// Oliver Kullmann, 25.5.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#ifndef STDITERATORSTESTOBECTS_638usuDq
#define STDITERATORSTESTOBECTS_638usuDq

#include <OKlib/Concepts/LibraryBasics_Tests.hpp>

#include <OKlib/Concepts/std_Iterators.hpp>
#include <OKlib/Concepts/std_Iterators_Tests.hpp>

namespace OKlib {

  namespace Concepts {

      OKLIB_BASIC_CONCEPT_TEST_TAG(InputIterator)
      OKLIB_BASIC_CONCEPT_TEST_ARCHETYPE(InputIterator)

      OKLIB_BASIC_CONCEPT_TEST_TAG(MultiPassInputIterator)
      OKLIB_BASIC_CONCEPT_TEST_ARCHETYPE(MultiPassInputIterator)

      OKLIB_BASIC_CONCEPT_TEST_TAG(ForwardIterator)
      OKLIB_BASIC_CONCEPT_TEST_ARCHETYPE(ForwardIterator)

  }

}

#endif

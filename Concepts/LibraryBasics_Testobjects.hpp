// Oliver Kullmann, 25.5.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#ifndef LIBRARYBASICSTESTOBJECTS_nnttrqr367
#define LIBRARYBASICSTESTOBJECTS_nnttrqr367

#include <OKlib/Concepts/LibraryBasics.hpp>
#include <OKlib/Concepts/LibraryBasics_Tests.hpp>

namespace OKlib {

  namespace Concepts {

      OKLIB_BASIC_CONCEPT_TEST_TAG(WithConceptTag)
      OKLIB_BASIC_CONCEPT_TEST_ARCHETYPE(WithConceptTag)

      OKLIB_BASIC_CONCEPT_TEST_TAG(BasicRequirements)
      OKLIB_BASIC_CONCEPT_TEST_ARCHETYPE(BasicRequirements)    

    namespace Macro_tests {

      template <typename>
      struct Test_OKLIB_MODELS_CONCEPT {
        struct c : virtual ConceptsBase_tag {};
        struct test {
          typedef c concept_tag;
        };
        void f() {
          OKLIB_MODELS_CONCEPT_REQUIRES(test, WithConceptTag);
          OKLIB_MODELS_CONCEPT_TAG(test, WithConceptTag);
        }
      };
      
    }

  }

}

#endif

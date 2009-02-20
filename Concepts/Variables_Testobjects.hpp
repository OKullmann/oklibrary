// Oliver Kullmann, 26.5.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Concepts/Variables_Testobjects.hpp
  \brief Checking whether concept and archetype definitions are consistent

  \deprecated Move to the new test system.
*/

#ifndef VARIABLESTESTOBJECTS_olKmNb56
#define VARIABLESTESTOBJECTS_olKmNb56

#include <OKlib/Concepts/LibraryBasics_Tests.hpp>

#include <OKlib/Concepts/Variables.hpp>
#include <OKlib/Concepts/Variables_Tests.hpp>

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

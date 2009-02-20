// Oliver Kullmann, 5.12.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Concepts/testobjects/Literals.cpp
  \brief Checking whether concept and archetype definitions are consistent
*/

#include <OKlib/Concepts/LibraryBasics_Tests.hpp>

#include <OKlib/Concepts/Literals.hpp>

namespace OKlib {
  namespace Concepts {
    
  OKLIB_BASIC_CONCEPT_TEST_TAG(OKlib::Concepts::Literals)
  OKLIB_BASIC_CONCEPT_TEST_ARCHETYPE(OKlib::Concepts::Literals)

  }
}


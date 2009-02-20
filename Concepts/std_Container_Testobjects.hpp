// Oliver Kullmann, 3.7.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#ifndef STDCONTAINERTESTOBJECTS_6654ama
#define STDCONTAINERTESTOBJECTS_6654ama

#include <OKlib/Concepts/LibraryBasics_Tests.hpp>

#include <OKlib/Concepts/std_Container.hpp>
#include <OKlib/Concepts/std_Container_Tests.hpp>

namespace OKlib {
  namespace Concepts {

    OKLIB_BASIC_CONCEPT_TEST_TAG(OKlib::Concepts::Container)
    OKLIB_BASIC_CONCEPT_TEST_ARCHETYPE(OKlib::Concepts::Container)

    template class OKlib::Concepts::Test_ContainerConcept<OKlib::Concepts::Container>;

  }

}

#endif

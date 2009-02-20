// Oliver Kullmann, 11.6.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#ifndef SINGLERESULTTESTOBJECTS_BVcr678
#define SINGLERESULTTESTOBJECTS_BVcr678

#include <OKlib/Concepts/ResultElements_Tests.hpp>

#include <OKlib/Experimentation/Competition/SingleResult.hpp>
#include <OKlib/Experimentation/Competition/SingleResult_Tests.hpp>

namespace OKlib {
  namespace SATCompetition {

    ::OKlib::Concepts::ResultElement_basic_test<SATCompetition::ResultElement> test_ResultElement;
    ::OKlib::Concepts::ResultElementWithName_basic_test<SATCompetition::ResultElement_with_name> test_ResultElement_with_name;

  }

}

#endif

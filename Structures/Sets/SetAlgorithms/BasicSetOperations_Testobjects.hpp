// Oliver Kullmann, 19.5.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#ifndef BASICSETOPERATIONSTESTOBJECTS_llLn4rT
#define BASICSETOPERATIONSTESTOBJECTS_llLn4rT

#include <OKlib/Structures/Sets/SetAlgorithms/BasicSetOperations.hpp>
#include <OKlib/Structures/Sets/SetAlgorithms/BasicSetOperations_Tests.hpp>

namespace OKlib {
  namespace SetAlgorithms {
    namespace Testobjects {

      Test_Union<Union> test_union;

      Test_Intersection<Intersection> test_intersection;

      // ToDo: Testing instantiation of Union and Intersection with archetypes.

    }

  }

}

#endif

// Oliver Kullmann, 11.9.2004 (Swansea)
/* Copyright 2004 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#ifndef ALGORITHMS_TESTOBJECTS_agBBv310Olk

#define ALGORITHMS_TESTOBJECTS_agBBv310Olk

#include <OKlib/General/Algorithms.hpp>
#include <OKlib/General/Algorithms_Tests.hpp>

namespace Algorithms_Testobjects {

  Algorithms_Tests::test_median<Algorithms::Median> test_median_1;
  Algorithms_Tests::test_median_with_sorting<Algorithms::Median_with_sorting> test_median_with_sorting_1;

  Algorithms_Tests::Test_power_natural<Algorithms::Power_natural> test_power_natural;
  
}

#endif

// Oliver Kullmann, 11.9.2004

#ifndef ALGORITHMS_TESTOBJECTS_agBBv310Olk

#define ALGORITHMS_TESTOBJECTS_agBBv310Olk

#include "Algorithms.hpp"
#include "Algorithms_Tests.hpp"

namespace Algorithms_Testobjects {

  Algorithms_Tests::test_median<Algorithms::Median> test_median_1;
  Algorithms_Tests::test_median_with_sorting<Algorithms::Median_with_sorting> test_median_with_sorting_1;

  Algorithms_Tests::Test_power_natural<Algorithms::Power_natural> test_power_natural;
  
}

#endif

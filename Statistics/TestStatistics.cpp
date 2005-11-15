#include <iostream>

#include "TestBaseClass.hpp"

#include "RegocniseRandomFormulas_Testobjects.hpp"
#include "TimeSeriesAnalysis_Testobjects.hpp"

int main() {
  return OKlib::TestSystem::TestBase::run_tests_default(std::cerr);
} 

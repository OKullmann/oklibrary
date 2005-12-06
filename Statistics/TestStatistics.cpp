#include <iostream>

#include "TestBaseClass.hpp"

#include "RecogniseRandomFormulas_Testobjects.hpp"
#include "TimeSeriesAnalysis_Testobjects.hpp"

int main() {
  return OKlib::TestSystem::TestBase::run_tests_default(std::cerr);
} 

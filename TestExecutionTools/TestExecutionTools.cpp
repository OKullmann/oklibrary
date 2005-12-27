#include <iostream>

#include "TestBaseClass.hpp"

#include "TestAlgorithms_Testobjects.hpp"
#include "SecondOrderTestTools_Testobjects.hpp"

int main() {
  return OKlib::TestSystem::TestBase::run_tests_default(std::cerr);
} 

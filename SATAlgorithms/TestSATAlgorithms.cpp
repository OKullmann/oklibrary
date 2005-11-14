#include <iostream>

#include "TestBaseClass.hpp"

#include "EnumerationAssignments_Testobjects.hpp"
#include "EnumerationSubclausesets_Testobjects.hpp"

int main() {
  return OKlib::TestSystem::TestBase::run_tests_default(std::cerr);
} 

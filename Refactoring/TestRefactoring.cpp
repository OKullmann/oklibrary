#include <iostream>

#include "TestBaseClass.hpp"

#include "IncludeHandling_Testobjects.hpp"
#include "Renaming_Testobjects.hpp"

int main() {
  return OKlib::TestSystem::TestBase::run_tests_default(std::cerr);
} 

#include <iostream>

#include "TestBaseClass.hpp"

#include "OptionBase_Testobjects.hpp"

int main() {
  return OKlib::TestSystem::TestBase::run_tests_default(std::cerr);
} 

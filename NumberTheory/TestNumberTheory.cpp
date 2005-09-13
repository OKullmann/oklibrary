#include <iostream>

#include "TestBaseClass.hpp"

#include "BigIntegers_Testobjects.hpp"
#include "ArithmeticalFunctions_Testobjects.hpp"

int main() {
  return OKlib::TestSystem::TestBase::run_tests_default(std::cerr);
} 

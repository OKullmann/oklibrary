#include <iostream>

#include "TestBaseClass.hpp"

#include "GrayCodes_Testobjects.hpp"
#include "Permutations_Testobjects.hpp"

int main() {
  return OKlib::TestSystem::TestBase::run_tests_default(std::cerr);
} 

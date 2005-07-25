#include <iostream>

#include "TestBaseClass.hpp"

#include "Dimacs_Testobjects.hpp"

int main() {
  return OKlib::TestSystem::TestBase::run_tests_default(std::cerr);
} 

#include <iostream>

#include "TestBaseClass.hpp"

#include "Dimacs_Testobjects.hpp"
#include "Exceptions_Testobjects.hpp"
#include "ClauseSetAdaptors_Testobjects.hpp"

int main() {
  return OKlib::TestSystem::TestBase::run_tests_default(std::cerr);
} 

// Oliver Kullmann, 29.8.2005 (Swansea)

#include <iostream>

#include <Transitional/TestSystem/TestBaseClass.hpp>

#include <Transitional/Refactoring/IncludeHandling_Testobjects.hpp>

int main() {
  return OKlib::TestSystem::TestBase::run_tests_default(std::cerr);
} 

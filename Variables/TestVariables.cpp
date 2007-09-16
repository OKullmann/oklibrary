// 5.6.2005 (Swansea)

#include <iostream>

#include <Transitional/TestSystem/TestBaseClass.hpp>

#include <Transitional/Variables/TrivialVariables_Testobjects.hpp>

int main() {
  return OKlib::TestSystem::TestBase::run_tests_default(std::cerr);
  // ToDo: normal output to std::cout, error messages to std::cerr (thus 2 streams, as in Aeryn).
} 

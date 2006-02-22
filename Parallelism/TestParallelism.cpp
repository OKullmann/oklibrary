#include <iostream>

#include <Transitional/Testsystem/TestBaseClass.hpp>

#include <Transitional/Parallelism/ExperimentalThreads_Testobjects.hpp>

int main() {
  return OKlib::TestSystem::TestBase::run_tests_default(std::cerr);
  // ToDo: normal output to std::cout, error messages to std::cerr (thus 2 streams, as in Aeryn).
} 

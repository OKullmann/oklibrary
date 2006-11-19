#include <iostream>

#include <Transitional/TestSystem/TestBaseClass.hpp>

#include <Transitional/Parallelism/ExperimentalThreads_Testobjects.hpp>

int main() {
  return OKlib::TestSystem::TestBase::run_tests_default(std::cerr);
} 

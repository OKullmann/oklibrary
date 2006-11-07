#include <iostream>

#include <Transitional/TestSystem/TestBaseClass.hpp>

#include <Transitional/InputOutput/Dimacs_Testobjects.hpp>

int main() {
  return OKlib::TestSystem::TestBase::run_tests_default(std::cerr);
} 

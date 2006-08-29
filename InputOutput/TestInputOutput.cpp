#include <iostream>

#include <Transitional/TestSystem/TestBaseClass.hpp>

#include <Transitional/InputOutput/Dimacs_Testobjects.hpp>
#include <Transitional/InputOutput/Exceptions_Testobjects.hpp>
#include <Transitional/InputOutput/ClauseSetAdaptors_Testobjects.hpp>

int main() {
  return OKlib::TestSystem::TestBase::run_tests_default(std::cerr);
} 

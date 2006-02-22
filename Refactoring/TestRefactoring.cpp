#include <iostream>

#include <Transitional/Testsystem/TestBaseClass.hpp>

#include <Transitional/Refactoring/IncludeHandling_Testobjects.hpp>
#include <Transitional/Refactoring/Renaming_Testobjects.hpp>

int main() {
  return OKlib::TestSystem::TestBase::run_tests_default(std::cerr);
} 

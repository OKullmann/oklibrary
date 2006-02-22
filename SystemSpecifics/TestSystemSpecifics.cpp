#include <iostream>

#include <Transitional/Testsystem/TestBaseClass.hpp>

#include <Transitional/SystemSpecifics/SafeReading_Testobjects.hpp>

int main() {
  return OKlib::TestSystem::TestBase::run_tests_default(std::cerr);
} 

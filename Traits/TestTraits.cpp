#include <iostream>

#include <Transitional/Testsystem/TestBaseClass.hpp>

#include <Transitional/Traits/TypeTraits_Testobjects.hpp>
#include <Transitional/Traits/TypeTraitsContainer_Testobjects.hpp>

int main() {
  return OKlib::TestSystem::TestBase::run_tests_default(std::cerr);
} 

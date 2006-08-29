#include <iostream>

#include <Transitional/TestSystem/TestBaseClass.hpp>

#include <Transitional/traits/TypeTraits_Testobjects.hpp>
#include <Transitional/traits/TypeTraitsContainer_Testobjects.hpp>

int main() {
  return OKlib::TestSystem::TestBase::run_tests_default(std::cerr);
} 

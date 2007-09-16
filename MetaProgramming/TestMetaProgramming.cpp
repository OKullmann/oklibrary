// Oliver Kullmann, 26.5.2005 (Swansea)

#include <iostream>

#include <Transitional/TestSystem/TestBaseClass.hpp>

#include <Transitional/MetaProgramming/Logical_Testobjects.hpp>
#include <Transitional/MetaProgramming/TaggingPolymorphism_Testobjects.hpp>
#include <Transitional/MetaProgramming/Numerical_Testobjects.hpp>

int main() {
  return OKlib::TestSystem::TestBase::run_tests_default(std::cerr);
}

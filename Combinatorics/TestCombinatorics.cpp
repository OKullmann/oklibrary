#include <iostream>

#include <Transitional/Testsystem/TestBaseClass.hpp>

#include <Transitional/Combinatorics/GrayCodes_Testobjects.hpp>
#include <Transitional/Combinatorics/Permutations_Testobjects.hpp>
#include <Transitional/Combinatorics/Subsets_Testobjects.hpp>

int main() {
  return OKlib::TestSystem::TestBase::run_tests_default(std::cerr);
} 

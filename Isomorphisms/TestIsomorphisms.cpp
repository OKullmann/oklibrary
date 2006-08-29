#include <iostream>

#include <Transitional/TestSystem/TestBaseClass.hpp>

#include <Transitional/Isomorphisms/CLSIsomorphisms_Testobjects.hpp>
#include <Transitional/Isomorphisms/GraphIsomorphisms_Testobjects.hpp>

int main() {
  return OKlib::TestSystem::TestBase::run_tests_default(std::cerr);
} 

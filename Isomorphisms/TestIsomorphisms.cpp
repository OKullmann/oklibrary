#include <iostream>

#include "TestBaseClass.hpp"

#include "CLSIsomorphisms_Testobjects.hpp"
#include "GraphIsomorphisms_Testobjects.hpp"

int main() {
  return OKlib::TestSystem::TestBase::run_tests_default(std::cerr);
} 

#include <iostream>

#include "TestBaseClass.hpp"

#include "RecursiveDirectoryIteration_Testobjects.hpp"
#include "IOStreamFilters_Testobjects.hpp"
#include "IOStreamSinks_Testobjects.hpp"

int main() {
  return OKlib::TestSystem::TestBase::run_tests_default(std::cerr);
} 

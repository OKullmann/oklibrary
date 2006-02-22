#include <iostream>

#include <Transitional/Testsystem/TestBaseClass.hpp>

#include <Transitional/GeneralInputOutput/RecursiveDirectoryIteration_Testobjects.hpp>
#include <Transitional/GeneralInputOutput/IOStreamFilters_Testobjects.hpp>
#include <Transitional/GeneralInputOutput/IOStreamSinks_Testobjects.hpp>
#include <Transitional/GeneralInputOutput/Multiplexer_Testobjects.hpp>

int main() {
  return OKlib::TestSystem::TestBase::run_tests_default(std::cerr);
} 

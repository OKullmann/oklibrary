// rename to TestTestExecutionTools.cpp

#include <iostream>

#include <Transitional/Testsystem/TestBaseClass.hpp>

#include <Transitional/TestExecutionTools/TestAlgorithms_Testobjects.hpp>
#include <Transitional/TestExecutionTools/SecondOrderTestTools_Testobjects.hpp>
#include <Transitional/TestExecutionTools/RelationTests_Testobjects.hpp>
#include <Transitional/TestExecutionTools/ExampleRelations_Testobjects.hpp>

int main() {
  return OKlib::TestSystem::TestBase::run_tests_default(std::cerr);
} 

#include <iostream>

#include <Transitional/Testsystem/TestBaseClass.hpp>

#include <Transitional/SetAlgorithms/BasicSetOperations_Testobjects.hpp>
#include <Transitional/SetAlgorithms/BasicMapOperations_Testobjects.hpp>
#include <Transitional/SetAlgorithms/SequenceOperations_Testobjects.hpp>
#include <Transitional/SetAlgorithms/Subsumption_Testobjects.hpp>

int main() {
  return OKlib::TestSystem::TestBase::run_tests_default(std::cerr);
} 

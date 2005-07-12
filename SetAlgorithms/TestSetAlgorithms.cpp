#include <iostream>

#include "TestBaseClass.hpp"

#include "BasicSetOperations_Testobjects.hpp"
#include "BasicMapOperations_Testobjects.hpp"
#include "SequenceOperations_Testobjects.hpp"
#include "Subsumption_Testobjects.hpp"


/*!
 * \todo normal output to std::cout, error messages to std::cerr (thus 2 streams, as in Aeryn).
 */

int main() {
  return OKlib::TestSystem::TestBase::run_tests_default(std::cerr);
} 

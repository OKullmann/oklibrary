#include <iostream>

#include "TestBaseClass.hpp"

#include "SingleResult_Testobjects.hpp"
#include "ParsingSingleResult_Testobjects.hpp"
#include "ParsingResultSequences_Testobjects.hpp"
#include "ResultProcessing_Testobjects.hpp"
#include "AnalysisTools_Testobjects.hpp"
#include "Scoring_Testobjects.hpp"

int main() {
  return OKlib::TestSystem::TestBase::run_tests_default(std::cerr);
  // ToDo: normal output to std::cout, error messages to std::cerr (thus 2 streams, as in Aeryn).
} 

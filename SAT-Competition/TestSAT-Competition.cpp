#include <iostream>

#include <Transitional/Testsystem/TestBaseClass.hpp>

#include <Transitional/SAT-Competition/SingleResult_Testobjects.hpp>
#include <Transitional/SAT-Competition/ParsingSingleResult_Testobjects.hpp>
#include <Transitional/SAT-Competition/ParsingResultSequences_Testobjects.hpp>
#include <Transitional/SAT-Competition/ResultProcessing_Testobjects.hpp>
#include <Transitional/SAT-Competition/AnalysisTools_Testobjects.hpp>
#include <Transitional/SAT-Competition/Scoring_Testobjects.hpp>
#include <Transitional/SAT-Competition/LexicographicalEvaluation_Testobjects.hpp>

/*!
  \file TestSAT-Competition.cpp
  \todo Tests reading files belong to the enhanced tests.
*/

int main() {
  return OKlib::TestSystem::TestBase::run_tests_default(std::cerr);
} 

#include <iostream>

#include <Transitional/Testsystem/TestBaseClass.hpp>

#include <Transitional/SATCompetition/SingleResult_Testobjects.hpp>
#include <Transitional/SATCompetition/ParsingSingleResult_Testobjects.hpp>
#include <Transitional/SATCompetition/ParsingResultSequences_Testobjects.hpp>
#include <Transitional/SATCompetition/ResultProcessing_Testobjects.hpp>
#include <Transitional/SATCompetition/AnalysisTools_Testobjects.hpp>
#include <Transitional/SATCompetition/Scoring_Testobjects.hpp>
#include <Transitional/SATCompetition/LexicographicalEvaluation_Testobjects.hpp>

/*!
  \file SATCompetition/TestSATCompetition.cpp
  \todo Tests reading files belong to the enhanced tests.
*/

int main() {
  return OKlib::TestSystem::TestBase::run_tests_default(std::cerr);
} 

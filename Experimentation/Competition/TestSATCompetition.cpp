// Oliver Kullmann, 7.5.2005 (Swansea)

#include <iostream>

#include <Transitional/TestSystem/TestBaseClass.hpp>

#include <Transitional/Experimentation/Competition/SingleResult_Testobjects.hpp>
#include <Transitional/Experimentation/Competition/ParsingSingleResult_Testobjects.hpp>
#include <Transitional/Experimentation/Competition/ParsingResultSequences_Testobjects.hpp>
#include <Transitional/Experimentation/Competition/ResultProcessing_Testobjects.hpp>
#include <Transitional/Experimentation/Competition/AnalysisTools_Testobjects.hpp>
#include <Transitional/Experimentation/Competition/Scoring_Testobjects.hpp>
#include <Transitional/Experimentation/Competition/LexicographicalEvaluation_Testobjects.hpp>

/*!
  \file Experimentation/Competition/TestSATCompetition.cpp
  \todo Tests reading files belong to the enhanced tests.
*/

int main() {
  return OKlib::TestSystem::TestBase::run_tests_default(std::cerr);
} 

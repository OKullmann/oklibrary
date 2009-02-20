// Oliver Kullmann, 7.5.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <iostream>

#include <OKlib/TestSystem/TestBaseClass.hpp>

#include <OKlib/Experimentation/Competition/SingleResult_Testobjects.hpp>
#include <OKlib/Experimentation/Competition/ParsingSingleResult_Testobjects.hpp>
#include <OKlib/Experimentation/Competition/ParsingResultSequences_Testobjects.hpp>
#include <OKlib/Experimentation/Competition/ResultProcessing_Testobjects.hpp>
#include <OKlib/Experimentation/Competition/AnalysisTools_Testobjects.hpp>
#include <OKlib/Experimentation/Competition/Scoring_Testobjects.hpp>
#include <OKlib/Experimentation/Competition/LexicographicalEvaluation_Testobjects.hpp>

/*!
  \file Experimentation/Competition/TestSATCompetition.cpp
  \todo Tests reading files belong to the enhanced tests.
*/

int main() {
  return OKlib::TestSystem::TestBase::run_tests_default(std::cerr);
} 

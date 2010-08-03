// Oliver Kullmann, 19.5.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <iostream>

#include <OKlib/TestSystem/TestBaseClass.hpp>

#include <OKlib/Structures/Sets/SetAlgorithms/BasicSetOperations_Testobjects.hpp>
#include <OKlib/Structures/Sets/SetAlgorithms/BasicMapOperations_Testobjects.hpp>
#include <OKlib/Structures/Sets/SetAlgorithms/SequenceOperations_Testobjects.hpp>
#include <OKlib/Structures/Sets/SetAlgorithms/Subsumption_Testobjects.hpp>
#include <OKlib/Structures/Sets/SetAlgorithms/SubsumptionHypergraph_Testobjects.hpp>

int main() {
  return OKlib::TestSystem::TestBase::run_tests_default(std::cerr);
} 

// Matthew Gwynne, 12.8.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <OKlib/TestSystem/RunTest_DesignStudy.hpp>

#include <OKlib/Structures/Sets/SetAlgorithms/tests/SubsumptionHypergraph.hpp>

namespace {

  ::OKlib::TestSystem::RunTest test_Subsumption_hypergraph(new ::OKlib::SetAlgorithms::tests::Test_Subsumption_hypergraph< OKlib::SetAlgorithms::Subsumption_hypergraph>());

}


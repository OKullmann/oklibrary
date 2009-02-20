// Oliver Kullmann, 25.10.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#ifndef LEXICOGRAPHICALEVALUATIONTESTOBJECTS_757556
#define LEXICOGRAPHICALEVALUATIONTESTOBJECTS_757556

#include <OKlib/Experimentation/Competition/LexicographicalEvaluation.hpp>
#include <OKlib/Experimentation/Competition/LexicographicalEvaluation_Tests.hpp>

namespace OKlib {
  namespace SATCompetition {

    Test_RepresentationSolverSeries<LexicographicalEvaluation> test_representation_solver_series;

    Test_LexicographicalEvaluation<LexicographicalEvaluation> test_lexicographical_evaluation;

    Test_LexicographicalEvaluationRandom<LexicographicalEvaluationRandom> test_lexicographical_evaluation_random;
  }

}

#endif

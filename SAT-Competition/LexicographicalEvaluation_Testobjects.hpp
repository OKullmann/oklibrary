// Oliver Kullmann, 25.10.2005 (Swansea)

#ifndef LEXICOGRAPHICALEVALUATIONTESTOBJECTS_757556

#define LEXICOGRAPHICALEVALUATIONTESTOBJECTS_757556

#include "LexicographicalEvaluation.hpp"
#include "LexicographicalEvaluation_Tests.hpp"

namespace OKlib {

  namespace SATCompetition {

    Test_RepresentationSolverSeries<LexicographicalEvaluation> test_representation_solver_series;

    Test_LexicographicalEvaluation<LexicographicalEvaluation> test_lexicographical_evaluation;

    Test_LexicographicalEvaluationRandom<LexicographicalEvaluationRandom> test_lexicographical_evaluation_random;
  }

}

#endif

// Oliver Kullmann, 2.3.2006 (Swansea)

/*!
  \file SAT-Competition/plans/SATCompetition.hpp
  \brief Plans for the module on the evaluation of the SAT-2005 competition

  \todo Rename the directory "SAT-Competition" to "SATCompetition".

  \todo In SAT-Competition/LexicographicalEvaluation.hpp for the transition
  from g++ version 4.0.2 to version 4.1.0 the friend-definitions in
  class template LexicographicalEvaluationRandom had to be replaced
  by member function declarations (see the code which was commented out),
  since it didn't work anymore in ComputeLexigraphicalEvaluation.cpp.
  Kind of clear that there is a problem, since ADL can't find typedef (as
  LexicographicalEvaluationRandom::map_super_series_to_evaluations_type); however
  putting class template Evaluation in ComputeLexigraphicalEvaluation.cpp into namespace
  OKlib::SATCompetition didn't help, and neither did the use of using-declarations ---
  why this?
*/



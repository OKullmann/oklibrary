// Oliver Kullmann, 17.3.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Solvers for the LS-MOLS-conditions, for Gecode

TODOS:

1. Write the most basic Gecode-solver, for the purpose of testing
   the constraints, which takes a constraint-object and an RT, and
   returns a BasicSR.

*/

#ifndef SOLVERS_PNeIRm1Ic7
#define SOLVERS_PNeIRm1Ic7

#include "Conditions.hpp"

namespace Solvers {

  namespace CD = Conditions;
  using size_t = CD::size_t;

  // Run-Type:
  enum class RT {
    sat_decision = 0,
    count_solutions = 1,
    enumerate_solutions = 2
  };

  // Simplest solver-return:
  struct BasicSR {
    size_t sol_found;
  };

}

#endif

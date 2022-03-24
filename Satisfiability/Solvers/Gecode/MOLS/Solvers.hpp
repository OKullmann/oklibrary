// Oliver Kullmann, 17.3.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Solvers for the LS-MOLS-conditions, for Gecode

TODOS:

1. (OZ; highest priority)
   Write the most basic Gecode-solver, for the purpose of testing
   the constraints, which takes a GenericMols-object (or a pointer)
   and an RT, and returns a BasicSR.
    - Based on this then a function is needed, which takes the
      partial squares and the conditions plus an RT, and returns a BasicSR.
    - Another version takes input-streams for the partial squares and the
      conditions.
    - The solver is as basic as possible, using defaults from Gecode whenever
      possible.

*/

#ifndef SOLVERS_PNeIRm1Ic7
#define SOLVERS_PNeIRm1Ic7

#include <set>

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


  typedef int val_t;
  typedef std::set<val_t> sval_t;
  typedef size_t var_t;
  typedef std::pair<var_t, sval_t> asg_t;
  typedef std::vector<asg_t> sol_t;
  typedef std::vector<sol_t> listsol_t;


  // Simplest solver-return:
  struct BasicSR {
    RT rt;
    size_t sol_found;
    listsol_t list_sol;
  };
  inline bool valid(const BasicSR sr) noexcept {
    if (sr.rt == RT::sat_decision)
      return sr.sol_found <= 1 and sr.list_sol.empty();
    else if (sr.rt == RT::count_solutions)
      return sr.list_sol.empty();
    else return sr.sol_found == sr.list_sol.size();
  }

}

#endif

// Oliver Kullmann, 17.3.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Solvers for the LS-MOLS-conditions, for Gecode

TODOS:

0. Is a Gecode "solution" a *total* or a *partial* assignment?
    - In MPG.pdf, Page 19, one finds
      "A search engine ensures that constraint propagation is performed and
       that all variables are assigned as described by the branching(s) of
       the model passed to the search engine."
      That sounds as if only total assignments are produced.
    - Also the test-cases in TestSolvers.cpp, which pose no constraint, return
      only total assignments.

1. DONE (OZ; highest priority)
   Write the most basic Gecode-solver, for the purpose of testing
   the constraints, which takes a GenericMols-object (or a pointer)
   and an RT, and returns a BasicSR.
    - DONE Based on this then a function is needed, which takes the
      partial squares and the conditions plus an RT, and returns a BasicSR.
    - DONE Another version takes input-streams for the partial squares and the
      conditions.
    - DONE
      The solver is as basic as possible, using defaults from Gecode whenever
      possible.
    - DONE
      It seems best to not using the services in Lookahead.hpp here, but to
      directly invoke the Gecode-functions directly.
    - DONE
      Then also not using Lookahead::Node, since we don't want to use these
      services here (we don't need them, and they might introduce bugs).
    - DONE The solver-class:
       - first creates the GenericMols0-object
       - then posts the (trivial) branching
       - finally runs the solver, with the loop over the solutions storing
         them.
    - DONE For the solution-extraction, the major problem is that the
      encoding-object is needed for the decoding of the solution?
      Perhaps that is a service provided by the constraint-class.

*/

#ifndef SOLVERS_PNeIRm1Ic7
#define SOLVERS_PNeIRm1Ic7

#include <set>

#include <cassert>

#include <gecode/search.hh>

#include "Conditions.hpp"
#include "Constraints.hpp"
#include "Parsing.hpp"
#include "PartialSquares.hpp"

namespace Solvers {

  namespace GC = Gecode;
  namespace CD = Conditions;
  namespace EC = Encoding;
  namespace CT = Constraints;
  namespace PR = Parsing;
  namespace PS = PartialSquares;

  using size_t = CD::size_t;

  // Run-Type:
  enum class RT {
    sat_decision = 0,
    count_solutions = 1,
    enumerate_solutions = 2
  };

  typedef std::vector<PS::PSquares> listsol_t;

  // Simplest solver-return:
  struct BasicSR {
    RT rt = RT::sat_decision;
    size_t sol_found = 0;
    listsol_t list_sol;
  };
  inline bool valid(const BasicSR sr) noexcept {
    if (sr.rt == RT::sat_decision)
      return sr.sol_found <= 1 and sr.list_sol.size() == sr.sol_found;
    else if (sr.rt == RT::count_solutions)
      return sr.list_sol.empty();
    else return sr.sol_found == sr.list_sol.size();
  }


  BasicSR solver0(const EC::EncCond& enc, const RT rt) {
    CT::GenericMols0* const gm = new CT::GenericMols0(enc);
    GC::branch(*gm, gm->V, GC::INT_VAR_SIZE_MIN(), GC::INT_VAL_MIN());
    GC::DFS<CT::GenericMols0> s(gm);
    delete gm;

    BasicSR res{rt};
    if (rt == RT::sat_decision) {
      if (CT::GenericMols0* const leaf = s.next()) {
        res.list_sol.push_back(enc.decode(leaf->V));
        res.sol_found = 1;
        delete leaf;
      }
    }
    else if (rt == RT::count_solutions) {
      while (CT::GenericMols0* const leaf = s.next()) {
        ++res.sol_found;
        delete leaf;
      }
    }
    else {
      assert(rt == RT::enumerate_solutions);
      while (CT::GenericMols0* const leaf = s.next()) {
        res.list_sol.push_back(enc.decode(leaf->V));
        ++res.sol_found;
        delete leaf;
      }
    }
    return res;
  }

  BasicSR solver0(const RT rt, const size_t N,
                  std::istream& in_cond, std::istream& in_ps) {
    const auto ac = PR::ReadAC()(in_cond);
    // Remark: ac must be constructed first, due to the (global)
    // names of squares.
    return solver0(EC::EncCond(ac, PS::PSquares(N, in_ps)), rt);
  }

}

#endif

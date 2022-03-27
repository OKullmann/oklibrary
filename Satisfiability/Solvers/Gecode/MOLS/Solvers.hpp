// Oliver Kullmann, 17.3.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Solvers for the LS-MOLS-conditions, for Gecode

   - solver0 : the simplest solvers, only for testing
   - solver_gc : everything from Gecode, but nothing beyond
   - solver_la : our look-ahead method.

TODOS:

0. Is a Gecode "solution" a *total* or a *partial* assignment?
    - In MPG.pdf, Page 19, one finds
      "A search engine ensures that constraint propagation is performed and
       that all variables are assigned as described by the branching(s) of
       the model passed to the search engine."
      That sounds as if only total assignments are produced.
    - Also the test-cases in TestSolvers.cpp, which pose no constraint, return
      only total assignments.
    - OK went through occurrences of "solution" in MPG.pdf, and could not find
      any hint that solutions would not be total. If partial solutions would
      be possible, that one needed quite some discussion about handling that.
      So it seems that indeed solutions are always total.
    - With look-ahead we we abort once a "solution" was found: when propagation
      is carried completely, also this should to total.

1. Statistics for the gc-solver
    - At least the user-runtime.
    - Plus possibly everthing Gecode has on offer?

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
    sat_solving = 0,
    sat_decision = 1,
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
      return sr.sol_found <= 1 and sr.list_sol.empty();
    else if (sr.rt == RT::sat_solving)
      return sr.sol_found <= 1 and sr.list_sol.size() == sr.sol_found;
    else if (sr.rt == RT::count_solutions)
      return sr.list_sol.empty();
    else return sr.sol_found == sr.list_sol.size();
  }


  BasicSR solver_basis(const EC::EncCond& enc, const RT rt,
                       const GC::IntVarBranch vrb,
                       const GC::IntValBranch vlb) {
    CT::GenericMols0* const gm = new CT::GenericMols0(enc);
    GC::branch(*gm, gm->V, vrb, vlb);
    GC::DFS<CT::GenericMols0> s(gm);
    delete gm;

    BasicSR res{rt};
    if (rt == RT::sat_decision) {
      if (CT::GenericMols0* const leaf = s.next()) {
        res.sol_found = 1;
        delete leaf;
      }
    }
    else if (rt == RT::sat_solving) {
      if (CT::GenericMols0* const leaf = s.next()) {
        assert(EC::EncCond::unit(leaf->V));
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
        assert(EC::EncCond::unit(leaf->V));
        res.list_sol.push_back(enc.decode(leaf->V));
        ++res.sol_found;
        delete leaf;
      }
    }
    return res;
  }


  BasicSR solver0(const EC::EncCond& enc, const RT rt) {
    return solver_basis(enc, rt,
                        GC::INT_VAR_SIZE_MIN(), GC::INT_VAL_MIN());
  }

  BasicSR solver0(const RT rt, const size_t N,
                  std::istream& in_cond, std::istream& in_ps) {
    const auto ac = PR::ReadAC()(in_cond);
    // Remark: ac must be constructed first, due to the (global)
    // names of squares.
    const auto ps = PS::PSquares(N, in_ps);
    return solver0(EC::EncCond(ac, ps), rt);
  }


  BasicSR solver_gc(const RT rt, const size_t N,
                    const GC::IntPropLevel pl,
                    const GC::IntVarBranch vrb, const GC::IntValBranch vlb,
                    std::istream& in_cond, std::istream& in_ps) {
    const auto ac = PR::ReadAC()(in_cond);
    const auto ps = PS::PSquares(N, in_ps);
    return solver_basis(EC::EncCond(ac, ps, pl), rt, vrb, vlb);
  }

}

#endif

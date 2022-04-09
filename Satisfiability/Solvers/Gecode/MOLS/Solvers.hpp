// Oliver Kullmann, 17.3.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Solvers for the LS-MOLS-conditions, for Gecode

  Types for handling of solver-results

   - import of Options::RT
   - typedef listsol_t for a vector of PSquares

   - class BasicSR for the direct solver-results
     function valid(BasicSR)
   - class BasicSR extends this by (gecode-)statistics.

  Solver for unit-testings (as simple as possible):

   - main function solver_basis
   - helper function solver0 in two overloads

  The pure Gecode-solver:

   - helper-function make_options
   - main function gcsolver_basis
   - helper function solver_gc

  The half look-ahead solver (only la-reduction):

   - XXX

  The full look-ahead solver:

   - main function lasolver
   - helper function solver_la.


TODOS:

-1. A solver using the la-reduction, but only Gecode-branching is needed.

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
    - With look-ahead we abort once a "solution" was found: when propagation
      is carried completely, also this should to total.

1. DONE Statistics for the gc-solver
    - DONE At least the user-runtime.
    - DONE Plus possibly everthing Gecode has on offer:
     - Using the statistics() member function of the search engine
       (Section 9.3 of MPG.pdf).
     - Figure 9.5 gives 6 numbers; restart and nogood currently are not used.
     - The object is of type GC::Search::Statistics
       https://www.gecode.org/doc/6.2.0/reference/classGecode_1_1Search_1_1Statistics.html


*/

#ifndef SOLVERS_PNeIRm1Ic7
#define SOLVERS_PNeIRm1Ic7

#include <vector>
#include <istream>

#include <cassert>

#include <gecode/int.hh>
#include <gecode/search.hh>

#include <SystemSpecifics/Timing.hpp>

#include "Conditions.hpp"
#include "Encoding.hpp"
#include "Constraints.hpp"
#include "Parsing.hpp"
#include "PartialSquares.hpp"
#include "Options.hpp"
#include "LookaheadBranching.hpp"

namespace Solvers {

  namespace GC = Gecode;
  namespace CD = Conditions;
  namespace EC = Encoding;
  namespace CT = Constraints;
  namespace PR = Parsing;
  namespace PS = PartialSquares;
  namespace OP = Options;
  namespace LAB = LookaheadBranching;

  using size_t = CD::size_t;


  typedef Options::RT RT;
  typedef std::vector<PS::PSquares> listsol_t;

  // Simplest solver-return:
  struct BasicSR {
    RT rt = RT::sat_decision;
    size_t sol_found = 0;
    listsol_t list_sol;
    bool operator ==(const BasicSR&) const noexcept = default;
    friend std::ostream& operator <<(std::ostream& out, const BasicSR& sr) {
      out << sr.rt << " " << sr.sol_found << "\n";
      Environment::out_line(out, sr.list_sol, "");
      return out;
    }
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

  struct GBasicSR  {
    BasicSR b;
    typedef GC::Search::Statistics gc_stats_t;
    gc_stats_t gs;
    double ut = 0;
    bool operator ==(const GBasicSR&) const noexcept = default;
  };


  /*
    The solver for the testing of encodings etc.
  */

  BasicSR solver_basis(const EC::EncCond& enc, const RT rt,
                       const GC::IntVarBranch vrb,
                       const GC::IntValBranch vlb) {
    CT::GenericMols0* const gm = new CT::GenericMols0(enc);
    GC::branch(*gm, gm->V, vrb, vlb);
    GC::DFS<CT::GenericMols0> s(gm);
    delete gm;

    BasicSR res{rt};
    if (rt == RT::sat_decision) {
      if (CT::GenericMols0* const leaf=s.next()){res.sol_found=1;delete leaf;}
    }
    else if (rt == RT::sat_solving) {
      if (CT::GenericMols0* const leaf = s.next()) {
        assert(EC::EncCond::unit(leaf->V));
        res.list_sol.push_back(enc.decode(leaf->V));
        res.sol_found = 1; delete leaf;
      }
    }
    else if (rt == RT::count_solutions) {
      while (CT::GenericMols0* const leaf = s.next()) {
        ++res.sol_found; delete leaf;
      }
    }
    else {
      assert(rt == RT::enumerate_solutions);
      while (CT::GenericMols0* const leaf = s.next()) {
        assert(EC::EncCond::unit(leaf->V));
        res.list_sol.push_back(enc.decode(leaf->V));
        ++res.sol_found; delete leaf;
      }
    }
    return res;
  }

  BasicSR solver0(const EC::EncCond& enc, const RT rt) {
    return solver_basis(enc, rt, GC::INT_VAR_SIZE_MIN(), GC::INT_VAL_MIN());
  }

  BasicSR solver0(const RT rt, const size_t N,
                  std::istream& in_cond, std::istream& in_ps) {
    const auto ac = PR::ReadAC()(in_cond);
    // Remark: ac must be constructed first, due to the (global)
    // names of squares.
    const auto ps = PS::PSquares(N, in_ps);
    return solver0(EC::EncCond(ac, ps), rt);
  }


  /*
    The pure Gecode-solver
  */
  // Safe creation of options for GC-search:
  GC::Search::Options make_options(const double t) noexcept {
    GC::Search::Options res; res.threads = t;
    return res;
  }
  GBasicSR gcsolver_basis(const EC::EncCond& enc, const RT rt,
                          const GC::IntVarBranch vrb,
                          const GC::IntValBranch vlb,
                          const double threads) {
    CT::GenericMols0* const gm = new CT::GenericMols0(enc);
    GC::branch(*gm, gm->V, vrb, vlb);
    GC::DFS<CT::GenericMols0> s(gm, make_options(threads));
    delete gm;

    GBasicSR res{rt};
    if (rt == RT::sat_decision) {
      if (CT::GenericMols0* const leaf=s.next()){
        res.b.sol_found = 1; delete leaf;
      }
      res.gs = s.statistics();
    }
    else if (rt == RT::sat_solving) {
      if (CT::GenericMols0* const leaf = s.next()) {
        assert(EC::EncCond::unit(leaf->V));
        res.b.list_sol.push_back(enc.decode(leaf->V));
        res.b.sol_found = 1; delete leaf;
      }
      res.gs = s.statistics();
    }
    else if (rt == RT::count_solutions) {
      while (CT::GenericMols0* const leaf = s.next()) {
        ++res.b.sol_found; delete leaf;
      }
      res.gs = s.statistics();
    }
    else {
      assert(rt == RT::enumerate_solutions);
      while (CT::GenericMols0* const leaf = s.next()) {
        assert(EC::EncCond::unit(leaf->V));
        res.b.list_sol.push_back(enc.decode(leaf->V));
        ++res.b.sol_found; delete leaf;
      }
      res.gs = s.statistics();
    }
    return res;
  }

  GBasicSR solver_gc(const EC::EncCond& enc, const RT rt,
                     const GC::IntVarBranch vrb,
                     const GC::IntValBranch vlb,
                     const double threads = 1) {
    Timing::UserTime timing;
    const Timing::Time_point t0 = timing();
    GBasicSR res = gcsolver_basis(enc, rt, vrb, vlb, threads);
    const Timing::Time_point t1 = timing();
    res.ut = t1 - t0;
    return res;
  }


  /*
    The solver with look-ahead -reduction and -branching
  */
  GBasicSR lasolver(const EC::EncCond& enc, const RT rt,
                    const Options::LAT lat, const Options::BHO bord,
                    const LAB::vec_t wghts, const double threads = 1) {
    CT::LookaheadMols* const gm = new CT::LookaheadMols(enc, wghts);
    LAB::post_la_branching<CT::LookaheadMols>(*gm, gm->var(), lat, bord);

    GC::DFS<CT::LookaheadMols> s(gm, make_options(threads));
    delete gm;

    GBasicSR res{rt};
    if (rt == RT::sat_decision) {
      if (CT::LookaheadMols* const leaf = s.next()) {
        res.b.sol_found = 1;
        delete leaf;
      }
      res.gs = s.statistics();
    }
    else if (rt == RT::sat_solving) {
      if (CT::LookaheadMols* const leaf = s.next()) {
        assert(EC::EncCond::unit(leaf->var()));
        res.b.list_sol.push_back(enc.decode(leaf->var()));
        res.b.sol_found = 1; delete leaf;
      }
      res.gs = s.statistics();
    }
    else if (rt == RT::count_solutions) {
      // XXX
    }
    else {
      assert(rt == RT::enumerate_solutions);
      // XXX
    }
    return res;
  }

  GBasicSR solver_la(const EC::EncCond& enc, const RT rt,
                     const OP::LAT lat, const OP::BHO bord,
                     const LAB::vec_t wghts, const double threads = 1) {
    Timing::UserTime timing;
    const Timing::Time_point t0 = timing();
    GBasicSR res = lasolver(enc, rt, lat, bord, wghts, threads);
    const Timing::Time_point t1 = timing();
    res.ut = t1 - t0;
    return res;
  }

}

#endif

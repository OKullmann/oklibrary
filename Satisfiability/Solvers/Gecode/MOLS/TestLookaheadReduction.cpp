// Oleg Zaikin, 29.3.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

Testing of look-ahead reduction for the Gecode library.

TODOS:

-1.Testing lareduction is URGENTLY needed:
  - DONE The space provided should be just GenericMols0.
  - DONE More precisely, a class say "GenericMolsNB" should be derived from
    GenericMols0, with "NB" for "no branching", which posts a brancher
    which immediately throws an exception -- this class is only to be
    used for testing.

    The first trial in this direction is
      GC::branch(*m, [](GC::Space&)->void{;});
    but then apparently solutions are no longer recognised?
    Perhaps the brancher must somehow recognise when the problem is solved?
  - The functions are to be tested exactly as they are.
  - The parameters of lareduction are *exactly* appropriate -- global variables
    must be avoided (and class-variables are just global variables).

0. Tests should mostly used enumeration-modes:
  - So that also the satisfying assignments can be tested.

1. DONE (GenericMoldNB class with the trivial brancher is used now)
   Use a trivial brancher in unit tests.
  - Since in Gecode branching must be posted to apply the status() function
    of a Space, a trivial customised brancher should be used to be sure
    that no branching is really done.
  - The status() function of the customised brancher must return true if there
    is at least one unassigned variable in the array.
  - Gecode must not call the choice() function of the customised brancher,
    This must be checked.

*/

#include <iostream>
#include <sstream>
#include <memory>

#include <cassert>

#include <gecode/int.hh>
#include <gecode/search.hh>

#include <ProgramOptions/Environment.hpp>

#include "LookaheadReduction.hpp"
#include "Conditions.hpp"
#include "Parsing.hpp"
#include "PartialSquares.hpp"
#include "Encoding.hpp"
#include "Options.hpp"
#include "Constraints.hpp"
#include "GcVariables.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.2.1",
        "12.5.2022",
        __FILE__,
        "Oleg Zaikin and Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Solvers/Gecode/MOLS/TestLookaheadReduction.cpp",
        "GPL v3"};

  using namespace LookaheadReduction;
  using namespace Conditions;
  using namespace Parsing;
  using namespace PartialSquares;
  using namespace Encoding;
  using namespace Options;
  using namespace Constraints;
  using namespace GcVariables;

  namespace GC = Gecode;

  // A trivial customised brancher that performs no branching.
  // It is needed for testing propagation, since Gecode::Space::status()
  // after propagation finds a brancher which status() function
  // returns true. Here status() returns true if there is at least
  // one unassigned variable.
  class VoidBrancher : public GC::Brancher {
    IntViewArray x;
    mutable int firstunassign;
  public:
    VoidBrancher(const GC::Home home, const IntViewArray& x) :
      GC::Brancher(home), x(x), firstunassign(0) {}
    VoidBrancher(GC::Space& home, VoidBrancher& b)
      : GC::Brancher(home,b), firstunassign(b.firstunassign) {
      x.update(home, b.x);
    }
    static void post(GC::Home home, const IntViewArray& x) {
      new (home) VoidBrancher(home, x);
    }
    virtual GC::Brancher* copy(GC::Space& home) {
      return new (home) VoidBrancher(home, *this);
    }
    virtual bool status(const GC::Space&) const noexcept {
      for (auto i = firstunassign; i < x.size(); ++i)
        if (not x[i].assigned()) { firstunassign = i; return true; }
      return false;
    }
    struct ModChoice : public GC::Choice {
      ModChoice(const VoidBrancher& b)
        : GC::Choice(b, 0) {}
    };
    // No branching is done, so neither choice() nor commit() are called:
    virtual GC::Choice* choice(GC::Space&) {
      assert(false);
      return new ModChoice(*this);
    }
    virtual GC::Choice* choice(const GC::Space&, GC::Archive&) {
      assert(false);
      return new ModChoice(*this);
    }
    virtual GC::ExecStatus commit(GC::Space&, const GC::Choice&,
                                  const unsigned) {
      assert(false);
      return GC::ES_OK;
    }
  };

  inline void post_void_branching(GC::Home home, const GC::IntVarArgs& V)
    noexcept {
    const IntViewArray y(home, V);
    VoidBrancher::post(home, y);
  }

  struct GenericMolsNB : GenericMols0 {
    GenericMolsNB(const EncCond& enc) : GenericMols0(enc) {
      post_void_branching(*this, V);
    }
  };

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  {// An empty square or order 2, four variables, each with domain {0,1}:
   std::istringstream in_cond("squares A\n");
   std::istringstream in_ps("");
   const AConditions ac = ReadAC()(in_cond);
   const PSquares ps = PSquares(2, in_ps);
   const GC::IntPropLevel pl = GC::IPL_VAL;
   const EncCond enc(ac, ps, pl);
   GenericMolsNB* const m = new GenericMolsNB(enc);
   assert(m->status() == Gecode::SS_BRANCH);
   assert(m->V.size() == 4);
   assert(assignedvars(m->V) == 0);
   assert(sumdomsizes(m->V) == 8);
   // Post X[0] == 0:
   std::unique_ptr<GenericMolsNB> ch =
     child_node<GenericMolsNB>(m, 0, 0, pl, true);
   assert(ch->status() == Gecode::SS_BRANCH);
   assert(ch->V.size() == m->V.size());
   assert(assignedvars(ch->V) == 1);
   assert(sumdomsizes(ch->V) == 7);
   // Check X[0] == 0:
   assert(assignedval(ch->V, 0) == 0);
   // Post X[0] != 0, so X[0] == 1:
   std::unique_ptr<GenericMolsNB> ch2 =
     child_node<GenericMolsNB>(m, 0, 0, pl, false);
   assert(ch2->status() == Gecode::SS_BRANCH);
   assert(ch2->V.size() == m->V.size());
   assert(assignedvars(ch2->V) == 1);
   assert(sumdomsizes(ch2->V) == 7);
   // Check X[0] == 1:
   assert(assignedval(ch2->V, 0) == 1);
   // Check probing:
   assert(probe(m, 0, 0, pl) == Gecode::SS_BRANCH);
   assert(probe(m, 0, 1, pl) == Gecode::SS_BRANCH);
   assert(probe(m, 1, 0, pl) == Gecode::SS_BRANCH);
   assert(probe(m, 1, 1, pl) == Gecode::SS_BRANCH);
   // Check that the original space has not been changed:
   assert(assignedvars(m->V) == 0);
   assert(sumdomsizes(m->V) == 8);
   delete m;
  }

  {// An empty Latin square of order 2:
   std::istringstream in_cond("squares A\nls A\n");
   std::istringstream in_ps("");
   const AConditions ac = ReadAC()(in_cond);
   const PSquares ps = PSquares(2, in_ps);
   const GC::IntPropLevel pl = GC::IPL_VAL;
   const EncCond enc(ac, ps, pl);
   GenericMolsNB* const m = new GenericMolsNB(enc);
   assert(m->status() == Gecode::SS_BRANCH);
   assert(m->V.size() == 4);
   assert(assignedvars(m->V) == 0);
   assert(sumdomsizes(m->V) == 8);
   // Post X[0] == 0:
   std::unique_ptr<GenericMolsNB> ch =
     child_node<GenericMolsNB>(m, 0, 0, pl, true);
   assert(ch->status() == Gecode::SS_SOLVED);
   assert(ch->V.size() == m->V.size());
   assert(assignedvars(ch->V) == 4);
   assert(sumdomsizes(ch->V) == 4);
   // Check X[0] == 0:
   assert(assignedval(ch->V,0) == 0);
   // Post X[0] != 0, so X[0] == 1:
   std::unique_ptr<GenericMolsNB> ch2 =
     child_node<GenericMolsNB>(m, 0, 0, pl, false);
   assert(ch2->status() == Gecode::SS_SOLVED);
   assert(ch2->V.size() == m->V.size());
   assert(assignedvars(ch2->V) == 4);
   assert(sumdomsizes(ch2->V) == 4);
   // Check X[0] == 1:
   assert(assignedval(ch2->V,0) == 1);
   // Check probing:
   assert(probe(m, 0, 0, pl) == Gecode::SS_SOLVED);
   assert(probe(m, 0, 1, pl) == Gecode::SS_SOLVED);
   assert(probe(m, 1, 0, pl) == Gecode::SS_SOLVED);
   assert(probe(m, 1, 1, pl) == Gecode::SS_SOLVED);
   // Check that the original space has not been changed:
   assert(assignedvars(m->V) == 0);
   assert(sumdomsizes(m->V) == 8);
   ReductionStatistics stat =
     lareduction<GenericMolsNB>(m, RT::enumerate_solutions, GC::IPL_VAL,
       LAR::eag_npr);
   assert(stat.props() == 1);
   assert(stat.elimvals() == 2);
   assert(stat.prunes() == 0);
   assert(stat.maxprune() == 0);
   assert(stat.probes() == 2);
   assert(stat.rounds() == 1);
   assert(stat.solc() == 2);
   assert(stat.leafcount() == 1);
   delete m;
  }

  {// An empty square of order 3:
   std::istringstream in_cond("squares A\n");
   std::istringstream in_ps("");
   const AConditions ac = ReadAC()(in_cond);
   const PSquares ps = PSquares(3, in_ps);
   const GC::IntPropLevel pl = GC::IPL_VAL;
   const EncCond enc(ac, ps, pl);
   GenericMolsNB* const m = new GenericMolsNB(enc);
   assert(m->status() == Gecode::SS_BRANCH);
   assert(m->V.size() == 9);
   assert(assignedvars(m->V) == 0);
   assert(sumdomsizes(m->V) == 27);
   // Post X[0] == 0:
   std::unique_ptr<GenericMolsNB> ch =
     child_node<GenericMolsNB>(m, 0, 0, pl, true);
   assert(ch->status() == Gecode::SS_BRANCH);
   assert(ch->V.size() == m->V.size());
   assert(assignedvars(ch->V) == 1);
   assert(sumdomsizes(ch->V) == 25);
   // Check X[0] == 0:
   assert(assignedval(ch->V,0) == 0);
   // Post X[0] != 0:
   std::unique_ptr<GenericMolsNB> ch2 =
     child_node<GenericMolsNB>(m, 0, 0, pl, false);
   assert(ch2->status() == Gecode::SS_BRANCH);
   assert(ch2->V.size() == m->V.size());
   assert(assignedvars(ch2->V) == 0);
   assert(sumdomsizes(ch2->V) == 26);
   // Check that X[0] has domain of size 2:
   assert(ch2->V[0].size() == 2);
   // Check probing:
   assert(probe(m, 0, 0, pl) == Gecode::SS_BRANCH);
   assert(probe(m, 0, 1, pl) == Gecode::SS_BRANCH);
   assert(probe(m, 0, 2, pl) == Gecode::SS_BRANCH);
   assert(probe(m, 1, 0, pl) == Gecode::SS_BRANCH);
   assert(probe(m, 1, 1, pl) == Gecode::SS_BRANCH);
   assert(probe(m, 1, 2, pl) == Gecode::SS_BRANCH);
   assert(probe(m, 2, 0, pl) == Gecode::SS_BRANCH);
   assert(probe(m, 2, 1, pl) == Gecode::SS_BRANCH);
   assert(probe(m, 2, 2, pl) == Gecode::SS_BRANCH);
   assert(assignedvars(m->V) == 0);
   assert(sumdomsizes(m->V) == 27);
   delete m;
  }

  {// A Latin square of order 3 with A[0,0] == 0:
   std::istringstream in_cond("squares A\nls A\n");
   std::istringstream in_ps("A\n0 * *\n* * *\n* * *\n");
   const AConditions ac = ReadAC()(in_cond);
   const PSquares ps = PSquares(3, in_ps);
   const GC::IntPropLevel pl = GC::IPL_VAL;
   const EncCond enc(ac, ps, pl);
   GenericMolsNB* const m = new GenericMolsNB(enc);
   assert(m->status() == Gecode::SS_BRANCH);
   assert(m->V.size() == 9);
   assert(assignedvars(m->V) == 1);
   assert(sumdomsizes(m->V) == 21);
   // Check X[0] == 0:
   assert(assignedval(m->V,0) == 0);
   // Post X[0] == 0:
   std::unique_ptr<GenericMolsNB> ch =
     child_node<GenericMolsNB>(m, 0, 0, pl, true);
   assert(ch->status() == Gecode::SS_BRANCH);
   assert(ch->V.size() == m->V.size());
   assert(assignedvars(ch->V) == 1);
   assert(sumdomsizes(ch->V) == 21);
   // Check X[0] == 0:
   assert(assignedval(ch->V,0) == 0);
   // Post X[0] != 0:
   std::unique_ptr<GenericMolsNB> ch2 =
     child_node<GenericMolsNB>(m, 0, 0, pl, false);
   assert(ch2->status() == Gecode::SS_FAILED);
   assert(ch2->V.size() == m->V.size());
   assert(assignedvars(ch2->V) == 1);
   assert(sumdomsizes(ch2->V) == 21);
   // Check X[0] == 0:
   assert(assignedval(ch2->V,0) == 0);
   // Check probing:
   assert(probe(m, 0, 0, pl) == Gecode::SS_BRANCH);
   assert(probe(m, 0, 1, pl) == Gecode::SS_FAILED);
   assert(probe(m, 0, 2, pl) == Gecode::SS_FAILED);
   assert(probe(m, 1, 0, pl) == Gecode::SS_FAILED);
   assert(probe(m, 1, 1, pl) == Gecode::SS_BRANCH);
   assert(probe(m, 1, 2, pl) == Gecode::SS_BRANCH);
   assert(probe(m, 2, 0, pl) == Gecode::SS_FAILED);
   assert(probe(m, 2, 1, pl) == Gecode::SS_BRANCH);
   assert(probe(m, 2, 2, pl) == Gecode::SS_BRANCH);
   assert(assignedvars(m->V) == 1);
   assert(sumdomsizes(m->V) == 21);
   delete m;
  }

  {// A Latin square of order 3 with A[1,1] == 1:
   std::istringstream in_cond("squares A\nls A\n");
   std::istringstream in_ps("A\n* * *\n* 1 *\n* * *\n");
   const AConditions ac = ReadAC()(in_cond);
   const PSquares ps = PSquares(3, in_ps);
   const GC::IntPropLevel pl = GC::IPL_VAL;
   const EncCond enc(ac, ps, pl);
   GenericMolsNB* const m = new GenericMolsNB(enc);
   assert(m->status() == Gecode::SS_BRANCH);
   assert(m->V.size() == 9);
   assert(assignedvars(m->V) == 1);
   assert(sumdomsizes(m->V) == 21);
   // Check X[4] == 1:
   assert(assignedval(m->V,4) == 1);
   // Post X[0] == 0:
   std::unique_ptr<GenericMolsNB> ch =
     child_node<GenericMolsNB>(m, 0, 0, pl, true);
   assert(ch->status() == Gecode::SS_SOLVED);
   assert(ch->V.size() == m->V.size());
   assert(assignedvars(ch->V) == 9);
   assert(sumdomsizes(ch->V) == 9);
   // Check X[0] == 0, X[4] == 1:
   assert(assignedval(ch->V,0) == 0);
   assert(assignedval(ch->V,4) == 1);
   // Post X[0] != 0:
   std::unique_ptr<GenericMolsNB> ch2 =
     child_node<GenericMolsNB>(m, 0, 0, pl, false);
   assert(ch2->status() == Gecode::SS_BRANCH);
   assert(ch2->V.size() == m->V.size());
   assert(assignedvars(ch2->V) == 1);
   assert(sumdomsizes(ch2->V) == 20);
   // Check X[4] == 1:
   assert(assignedval(ch2->V,4) == 1);
   // Check probing:
   assert(probe(m, 0, 0, pl) == Gecode::SS_SOLVED);
   assert(probe(m, 0, 1, pl) == Gecode::SS_BRANCH);
   assert(probe(m, 0, 2, pl) == Gecode::SS_SOLVED);
   assert(probe(m, 1, 0, pl) == Gecode::SS_BRANCH);
   assert(probe(m, 1, 1, pl) == Gecode::SS_FAILED);
   assert(probe(m, 1, 2, pl) == Gecode::SS_BRANCH);
   assert(probe(m, 2, 0, pl) == Gecode::SS_SOLVED);
   assert(probe(m, 2, 1, pl) == Gecode::SS_BRANCH);
   assert(probe(m, 2, 2, pl) == Gecode::SS_SOLVED);
   assert(assignedvars(m->V) == 1);
   assert(sumdomsizes(m->V) == 21);
   delete m;
  }

}

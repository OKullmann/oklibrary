// Oleg Zaikin, 29.3.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

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

namespace {

  const Environment::ProgramInfo proginfo{
        "0.0.9",
        "6.5.2022",
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

  namespace GC = Gecode;

  typedef std::int64_t signed_t;
}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  {// One square, N==2, four variables, each with domain {0,1}:
   std::istringstream in_cond("squares A\n");
   std::istringstream in_ps("");
   const AConditions ac = ReadAC()(in_cond);
   const PSquares ps = PSquares(2, in_ps);
   const GC::IntPropLevel pl = GC::IPL_VAL;
   const EncCond enc(ac, ps, pl);
   LookaheadMols* const m =
     new LookaheadMols(enc, RT::sat_decision, GBO::asc, LAR::eager, {0});
   GC::branch(*m, m->var(), GC::INT_VAR_SIZE_MIN(), GC::INT_VAL_MIN());
   assert(m->valid());
   assert(m->status() == Gecode::SS_BRANCH);
   assert(m->valid());
   assert(m->var().size() == 4);
   assert(m->valid(0));
   assert(m->valid(3));
   assert(not m->valid(4));
   assert(m->assignedvars() == 0);
   assert(m->sumdomsizes() == 8);
   // Post X[0] == 0:
   std::unique_ptr<LookaheadMols> ch =
     child_node<LookaheadMols>(m, 0, 0, pl, true);
   assert(ch->valid());
   assert(ch->status() == Gecode::SS_BRANCH);
   assert(ch->valid());
   assert(ch->var().size() == m->var().size());
   assert(ch->assignedvars() == 1);
   assert(ch->sumdomsizes() == 7);
   // Check X[0] == 0:
   GC::Int::IntView view = ch->var()[0];
   assert(view.assigned());
   GC::IntVarValues j(view);
   signed_t val = j.val();
   assert(val == 0);
   // Post X[0] != 0, so X[0] == 1:
   std::unique_ptr<LookaheadMols> ch2 =
     child_node<LookaheadMols>(m, 0, 0, pl, false);
   assert(ch2->valid());
   assert(ch2->status() == Gecode::SS_BRANCH);
   assert(ch2->valid());
   assert(ch2->var().size() == m->var().size());
   assert(ch2->assignedvars() == 1);
   assert(ch2->sumdomsizes() == 7);
   // Check X[0] == 1:
   GC::Int::IntView view2 = ch2->var()[0];
   assert(view2.assigned());
   GC::IntVarValues j2(view2);
   signed_t val2 = j2.val();
   assert(val2 == 1);
   // Check probing:
   assert(probe(m, 0, 0, pl) == Gecode::SS_BRANCH);
   assert(probe(m, 0, 1, pl) == Gecode::SS_BRANCH);
   assert(probe(m, 1, 0, pl) == Gecode::SS_BRANCH);
   assert(probe(m, 1, 1, pl) == Gecode::SS_BRANCH);
   // Check that the original space has not been changed:
   assert(m->assignedvars() == 0);
   assert(m->sumdomsizes() == 8);
  }

  {// One Latin square, N==2, four variables, each with domain {0,1}:
   std::istringstream in_cond("squares A\nls A\n");
   std::istringstream in_ps("");
   const AConditions ac = ReadAC()(in_cond);
   const PSquares ps = PSquares(2, in_ps);
   const GC::IntPropLevel pl = GC::IPL_VAL;
   const EncCond enc(ac, ps, pl);
   LookaheadMols* const m =
     new LookaheadMols(enc, RT::sat_decision, GBO::asc, LAR::eager, {0});
   GC::branch(*m, m->var(), GC::INT_VAR_SIZE_MIN(), GC::INT_VAL_MIN());
   assert(m->valid());
   assert(m->status() == Gecode::SS_BRANCH);
   assert(m->valid());
   assert(m->var().size() == 4);
   assert(m->valid(0));
   assert(m->valid(3));
   assert(not m->valid(4));
   assert(m->assignedvars() == 0);
   assert(m->sumdomsizes() == 8);
   // Post X[0] == 0:
   std::unique_ptr<LookaheadMols> ch =
     child_node<LookaheadMols>(m, 0, 0, pl, true);
   assert(ch->valid());
   assert(ch->status() == Gecode::SS_SOLVED);
   assert(ch->valid());
   assert(ch->var().size() == m->var().size());
   assert(ch->assignedvars() == 4);
   assert(ch->sumdomsizes() == 4);
   // Check X[0] == 0:
   GC::Int::IntView view = ch->var()[0];
   assert(view.assigned());
   GC::IntVarValues j(view);
   signed_t val = j.val();
   assert(val == 0);
   // Post X[0] != 0, so X[0] == 1:
   std::unique_ptr<LookaheadMols> ch2 =
     child_node<LookaheadMols>(m, 0, 0, pl, false);
   assert(ch2->valid());
   assert(ch2->status() == Gecode::SS_SOLVED);
   assert(ch2->valid());
   assert(ch2->var().size() == m->var().size());
   assert(ch2->assignedvars() == 4);
   assert(ch2->sumdomsizes() == 4);
   // Check X[0] == 1:
   GC::Int::IntView view2 = ch2->var()[0];
   assert(view2.assigned());
   GC::IntVarValues j2(view2);
   signed_t val2 = j2.val();
   assert(val2 == 1);
   // Check probing:
   assert(probe(m, 0, 0, pl) == Gecode::SS_SOLVED);
   assert(probe(m, 0, 1, pl) == Gecode::SS_SOLVED);
   assert(probe(m, 1, 0, pl) == Gecode::SS_SOLVED);
   assert(probe(m, 1, 1, pl) == Gecode::SS_SOLVED);
   // Check that the original space has not been changed:
   assert(m->assignedvars() == 0);
   assert(m->sumdomsizes() == 8);
  }

  {// One square, N==3, nine variables, each with domain {0,1,2}:
   std::istringstream in_cond("squares A\n");
   std::istringstream in_ps("");
   const AConditions ac = ReadAC()(in_cond);
   const PSquares ps = PSquares(3, in_ps);
   const GC::IntPropLevel pl = GC::IPL_VAL;
   const EncCond enc(ac, ps, pl);
   LookaheadMols* const m =
     new LookaheadMols(enc, RT::sat_decision, GBO::asc, LAR::eager, {0,2});
   GC::branch(*m, m->var(), GC::INT_VAR_SIZE_MIN(), GC::INT_VAL_MIN());
   assert(m->valid());
   assert(m->status() == Gecode::SS_BRANCH);
   assert(m->valid());
   assert(m->var().size() == 9);
   assert(m->valid(0));
   assert(m->valid(8));
   assert(not m->valid(9));
   assert(m->assignedvars() == 0);
   assert(m->sumdomsizes() == 27);
   // Post X[0] == 0:
   std::unique_ptr<LookaheadMols> ch =
     child_node<LookaheadMols>(m, 0, 0, pl, true);
   assert(ch->valid());
   assert(ch->status() == Gecode::SS_BRANCH);
   assert(ch->valid());
   assert(ch->var().size() == m->var().size());
   assert(ch->assignedvars() == 1);
   assert(ch->sumdomsizes() == 25);
   // Check X[0] == 0:
   GC::Int::IntView view = ch->var()[0];
   assert(view.assigned());
   GC::IntVarValues j(view);
   signed_t val = j.val();
   assert(val == 0);
   // Post X[0] != 0:
   std::unique_ptr<LookaheadMols> ch2 =
     child_node<LookaheadMols>(m, 0, 0, pl, false);
   assert(ch2->valid());
   assert(ch2->status() == Gecode::SS_BRANCH);
   assert(ch2->valid());
   assert(ch2->var().size() == m->var().size());
   assert(ch2->assignedvars() == 0);
   assert(ch2->sumdomsizes() == 26);
   // Check that X[0] has domain of size 2:
   assert(ch2->var()[0].size() == 2);
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
   assert(m->assignedvars() == 0);
   assert(m->sumdomsizes() == 27);
  }

}

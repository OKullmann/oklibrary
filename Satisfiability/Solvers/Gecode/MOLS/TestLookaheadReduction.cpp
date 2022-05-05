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
        "0.0.5",
        "5.5.2022",
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
}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  {std::istringstream in_cond("squares A\n");
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
   std::unique_ptr<LookaheadMols> ch =
     child_node<LookaheadMols>(m, 0, 0, pl, true);
   assert(ch->valid());
   assert(ch->status() == Gecode::SS_BRANCH);
   assert(ch->valid());
   assert(ch->var().size() == m->var().size());
   assert(ch->assignedvars() == 1);
   assert(ch->sumdomsizes() == 7);
  }

}

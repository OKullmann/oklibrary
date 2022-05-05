// Oleg Zaikin, 29.3.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <iostream>
#include <sstream>

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
        "0.0.3",
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
}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  {std::istringstream ss_cond("squares A\n");
   const AConditions ac = ReadAC()(ss_cond);
   const PSquares ps = PSquares(2,{});
   const EncCond enc(ac, ps, Gecode::IPL_VAL);
   LookaheadMols* const m =
     new LookaheadMols(enc, RT::sat_decision, GBO::asc, LAR::eager, {0});
   assert(m->valid());
   assert(m->var().size() == 4);
   assert(m->valid(0));
   assert(m->valid(1));
   assert(m->valid(2));
   assert(m->valid(3));
   assert(not m->valid(4));
  }

}

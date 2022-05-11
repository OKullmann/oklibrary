// Oleg Zaikin, 11.5.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
 Unit tests for GcVariables.hpp's helper functions.

TODOS:

*/

#include <iostream>
#include <sstream>

#include <cassert>

#include <ProgramOptions/Environment.hpp>

#include "GcVariables.hpp"
#include "Conditions.hpp"
#include "Parsing.hpp"
#include "PartialSquares.hpp"
#include "Encoding.hpp"
#include "Options.hpp"
#include "Constraints.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.0.1",
        "11.5.2022",
        __FILE__,
        "Oleg Zaikin",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Solvers/Gecode/MOLS/TestGcVariables.cpp",
        "GPL v3"};

  using namespace GcVariables;
  using namespace Conditions;
  using namespace Parsing;
  using namespace PartialSquares;
  using namespace Encoding;
  using namespace Constraints;

  namespace GC = Gecode;
}


int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  {// An empty square or order 2, four variables, each with domain {0,1}:
   std::istringstream in_cond("squares A\n");
   std::istringstream in_ps("");
   const AConditions ac = ReadAC()(in_cond);
   const PSquares ps = PSquares(2, in_ps);
   const EncCond enc(ac, ps, GC::IPL_VAL);
   GenericMols0* const m = new GenericMols0(enc);
   assert(valid(m->V));
   assert(m->V.size() == 4);
   assert(valid(m->V,0));
   assert(valid(m->V,1));
   assert(valid(m->V,2));
   assert(valid(m->V,3));
   assert(not valid(m->V,4));
   delete m;
  }

}

// Oleg Zaikin, 31.3.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

TODOS:

1. Urgently unit-tests are needed, for all components.
   - DONE tr() function
   - ValVec struct
   - append() function
   - create() function
   - GcBranching struct
   - VVElim struct
   - create_la() function
   - rlaStats struct
   - RlaBranching struct
   - BranchingStatistics class
   - laStats struct
   - branch_measure() function
   - LaBranching struct

*/

#include <iostream>
#include <sstream>
#include <memory>

#include <cassert>

#include <gecode/int.hh>
#include <gecode/search.hh>

#include <ProgramOptions/Environment.hpp>

#include "LookaheadBranching.hpp"
#include "Options.hpp"
#include "GcVariables.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.0.10",
        "7.6.2022",
        __FILE__,
        "Oleg Zaikin and Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Solvers/Gecode/MOLS/TestLookaheadBranching.cpp",
        "GPL v3"};

  using namespace LookaheadBranching;

  namespace GC = Gecode;
  namespace GV = GcVariables;

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  {assert(tr(GV::gcintarr(1, 1)[0].size(), 1) == 1);
   assert(tr(GV::gcintarr(1, 2)[0].size(), 1) == 2);
   assert(tr(GV::gcintarr(2, 1)[0].size(), 1) == 1);
   assert(tr(GV::gcintarr(2, 1)[1].size(), 1) == 1);
   assert(tr(GV::gcintarr(2, 2)[0].size(), 1) == 2);
   assert(tr(GV::gcintarr(2, 2)[1].size(), 1) == 2);
  }

}

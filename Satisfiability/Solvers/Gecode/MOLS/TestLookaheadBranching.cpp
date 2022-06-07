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
   - DONE ValVec struct
   - DONE append() function
   - DONE create() function
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
#include "Cases.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.2",
        "7.6.2022",
        __FILE__,
        "Oleg Zaikin and Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Solvers/Gecode/MOLS/TestLookaheadBranching.cpp",
        "GPL v3"};

  using namespace LookaheadBranching;
  using namespace GcVariables;

  namespace GC = Gecode;
  namespace CS = Cases;
  namespace OP = Options;

  template <class X>
  constexpr bool eqp(const X& lhs, const X& rhs) noexcept {
    return lhs == rhs;
  }

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  {assert(tr(GecodeIntVarArray(1, 1)[0].size(), 1) == 1);
   assert(tr(GecodeIntVarArray(1, 2)[0].size(), 1) == 2);
   assert(tr(gcintarr(2, 1)[0].size(), 1) == 1);
   assert(tr(gcintarr(2, 1)[1].size(), 1) == 1);
   assert(tr(gcintarr(2, 2)[0].size(), 1) == 2);
   assert(tr(gcintarr(2, 2)[1].size(), 1) == 2);
  }

  {GenericIntArray g(1, 1);
   const ValVec vv(CS::Void(g), {0, 0});
   assert(eqp(vv.br, {0, 0}));
  }
  {GenericIntArray g(1, 2);
   const ValVec vv(CS::Void(g), {0, 0, 1});
   assert(eqp(vv.br, {0, 0, 1}));
  }
  {GenericIntArray g(1, 3);
   const ValVec vv(CS::Void(g), {0, 0, 1, 2});
   assert(eqp(vv.br, {0, 0, 1, 2}));
  }

  {assert(eqp(append(0, {1}, true), {0, 1}));
   assert(eqp(append(0, {1}, false), {0, 1}));
   assert(eqp(append(0, {1, 2}, true), {0, 1, 2}));
   assert(eqp(append(0, {1, 2}, false), {0, 2, 1}));
  }

  {GenericIntArray g(1, 3);
   CS::Void b(g);
   const ValVec* const vvp = create(0, {1, 2}, OP::BRT::bin, OP::GBO::asc, b);
   assert(eqp(vvp->br, {0, 1}));
  }
  {GenericIntArray g(1, 3);
   CS::Void b(g);
   const ValVec* const vvp = create(0, {1, 2}, OP::BRT::bin, OP::GBO::desc, b);
   assert(eqp(vvp->br, {0, 2}));
  }
  {GenericIntArray g(1, 3);
   CS::Void b(g);
   const ValVec* const vvp = create(0, {1, 2}, OP::BRT::enu, OP::GBO::asc, b);
   assert(eqp(vvp->br, {0, 1, 2}));
  }
  {GenericIntArray g(1, 3);
   CS::Void b(g);
   const ValVec* const vvp = create(0, {1, 2}, OP::BRT::enu, OP::GBO::desc, b);
   assert(eqp(vvp->br, {0, 2, 1}));
  }

}

// Oleg Zaikin, 31.3.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

TODOS:

0. Use more loops
   - Most of the tests in here can be simplified (and code-duplication
     removed) by using loops.

1. Urgently unit-tests are needed, for all components.
   - DONE tr() function
   - DONE wsumdomsizes() function
   - DONE new_vars() function
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
   - branch_distance() function
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
        "0.1.6",
        "8.6.2022",
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

  {for (size_t n = 1; n <= 3; ++n)
     for (size_t v = 0; v < n; ++v)
       for (size_t dom = 1; dom <= 3; ++dom)
         assert(tr(GcIntVarArray(n,dom)[v].size(),1) == dom);
  }

  {const GcIntVarArray V(1, 1);
   const OP::weights_t w = {0, 0};
   assert(wsumdomsizes(V, &w) == 0);
  }
  {const GcIntVarArray V(1, 2);
   const OP::weights_t w = {0, 0, 1};
   assert(wsumdomsizes(V, &w) == 1);
  }
  {const GcIntVarArray V(1, 3);
   const OP::weights_t w = {0, 0, 1, 2};
   assert(wsumdomsizes(V, &w) == 2);
  }
  {const GcIntVarArray V(2, 3);
   const OP::weights_t w = {0, 0, 1, 2};
   assert(wsumdomsizes(V, &w) == 2 * 2);
  }

  {const OP::weights_t w = {0, 0};
   const GcIntVarArray V(1, 2);
   assert(new_vars(V, V, &w, 1) == 0);
  }
  {const OP::weights_t w = {0, 0, 1};
   const GcIntVarArray V(1, 2);
   const GcIntVarArray nV(1, 1);
   assert(new_vars(V, nV, &w, 1) == 1);
  }
  {const GcIntVarArray V(1, 3);
   const GcIntVarArray nV(1, 2);
   const OP::weights_t w = {0, 0, 1, 2};
   assert(new_vars(V, nV, &w, 1) == 1);
  }
  {const GcIntVarArray V(2, 2);
   const GcIntVarArray nV(2, 1);
   const OP::weights_t w = {0, 0, 1};
   assert(new_vars(V, nV, &w, 1) == 2);
  }

  {GcIntArraySpace g(1, 1);
   const ValVec vv(CS::Void(g), {0, 0});
   assert(eqp(vv.br, {0, 0}));
  }
  {GcIntArraySpace g(1, 2);
   const ValVec vv(CS::Void(g), {0, 0, 1});
   assert(eqp(vv.br, {0, 0, 1}));
  }
  {GcIntArraySpace g(1, 3);
   const ValVec vv(CS::Void(g), {0, 0, 1, 2});
   assert(eqp(vv.br, {0, 0, 1, 2}));
  }

  {assert(eqp(append(0, {1}, true), {0, 1}));
   assert(eqp(append(0, {1}, false), {0, 1}));
   assert(eqp(append(0, {1, 2}, true), {0, 1, 2}));
   assert(eqp(append(0, {1, 2}, false), {0, 2, 1}));
  }

  {GcIntArraySpace g(1, 3);
   CS::Void b(g);
   const ValVec* const vvp = create(0, {1, 2}, OP::BRT::bin, OP::GBO::asc, b);
   assert(eqp(vvp->br, {0, 1}));
  }
  {GcIntArraySpace g(1, 3);
   CS::Void b(g);
   const ValVec* const vvp = create(0, {1, 2}, OP::BRT::bin, OP::GBO::desc, b);
   assert(eqp(vvp->br, {0, 2}));
  }
  {GcIntArraySpace g(1, 3);
   CS::Void b(g);
   const ValVec* const vvp = create(0, {1, 2}, OP::BRT::enu, OP::GBO::asc, b);
   assert(eqp(vvp->br, {0, 1, 2}));
  }
  {GcIntArraySpace g(1, 3);
   CS::Void b(g);
   const ValVec* const vvp = create(0, {1, 2}, OP::BRT::enu, OP::GBO::desc, b);
   assert(eqp(vvp->br, {0, 2, 1}));
  }

}

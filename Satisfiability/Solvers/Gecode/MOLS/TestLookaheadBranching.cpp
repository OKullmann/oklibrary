// Oleg Zaikin, 31.3.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

TODOS:

0. DONE (Loops are now used for wsumdomsizes(), new_vars(), and ValVec.)
   Use more loops
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

#include <Numerics/NumBasicFunctions.hpp>
#include <ProgramOptions/Environment.hpp>

#include "LookaheadBranching.hpp"
#include "Options.hpp"
#include "GcVariables.hpp"
#include "Cases.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.7",
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
  namespace FP = FloatingPoint;

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

  {const OP::weights_t w = {0, 0, 1, 2};
   for (size_t n = 1; n <= 3; ++n)
     for (size_t dom = 1; dom <= 3; ++dom) {
       const GcIntVarArray V(n, dom);
       assert(wsumdomsizes(V, &w) == n * w[dom]);
     }
  }

  {const OP::weights_t w = {0, 0, 1, 2, 3};
   for (size_t n = 1; n <= 3; ++n)
     for (size_t dom = 2; dom <= 4; ++dom)
       for (size_t depth = 1; depth <= 3; ++depth) {
         const GcIntVarArray V(n, dom);
         const GcIntVarArray nV(n, dom-1);
         const auto wval = dom==2 ? FP::exp2((w)[1] * depth) : w[1];
         assert(new_vars(V, nV, &w, depth) == n * wval);
       }
  }

  {for (size_t n = 1; n <= 3; ++n)
     for (size_t v = 0; v < n; ++v)
       for (size_t dom = 1; dom <= 3; ++dom) {
         GcIntArraySpace g(n, dom);
         values_t branching = {v};
         for (int i=0; i<n; ++i) branching.push_back(i);
         const ValVec vv(CS::Void(g), branching);
         assert(eqp(vv.br, branching));
       }
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

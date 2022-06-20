// Oleg Zaikin, 31.3.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

TODOS:

0. Urgently unit-tests are needed, for all components.
   - DONE tr() function
   - DONE wsumdomsizes() function
   - DONE new_vars() function
   - DONE ValVec struct
   - DONE append() function
   - DONE create() function
   - GcBranching struct
   - VVElim struct
   - create_la() function
   - DONE LRStoppingData struct
   - ListStoppingDate struct
   - rlaStats struct
   - RlaBranching struct
   - BranchingStatistics class
   - laStats struct
   - branch_distance() function
   - LaBranching struct

*/

#include <iostream>

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
        "0.1.14",
        "20.6.2022",
        __FILE__,
        "Oleg Zaikin and Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Solvers/Gecode/MOLS/TestLookaheadBranching.cpp",
        "GPL v3"};

  using namespace LookaheadBranching;
  using namespace GcVariables;
  using namespace Options;

  namespace GC = Gecode;
  namespace CS = Cases;
  namespace FP = FloatingPoint;
  namespace ET = Environment;

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

  {for (size_t b = 0, p = 1; p <= 64; ++b, p*=2)
     assert(wmuld(p) == b);
  }

  {const OP::weights_t w = {0, 0, 1, 2};
   for (size_t n = 1; n <= 3; ++n)
     for (size_t dom = 1; dom <= 3; ++dom) {
       const GcIntVarArray V(n, dom);
       const auto res = n * (dom-1);
       assert(wnumvars(V, &w) == res);
       assert(muap(V) == res);
       assert(muld(V) == n * FP::log2(dom));
     }
  }

  {const OP::weights_t w = {0, 0, 1, 2, 3};
   for (size_t n = 1; n <= 3; ++n)
     for (size_t dom = 2; dom <= 4; ++dom)
       for (size_t depth = 1; depth <= 3; ++depth) {
         const GcIntVarArray V(n, dom);
         const GcIntVarArray nV(n, dom-1);
         const auto wval = dom==2 ? FP::exp2((w)[1] * depth) : w[dom-1];
         assert(new_vars(V, nV, &w, depth) == n * wval);
       }
  }

  {for (size_t n = 1; n <= 3; ++n)
     for (size_t dom = 1; dom <= 3; ++dom) {
       GcIntArraySpace g(n, dom);
       for (int v = 0; v < g.n; ++v) {
         values_t branching{v};
         for (int i = 0; i < g.n; ++i) branching.push_back(i);
         const ValVec vv(CS::Void(g), branching);
         assert(vv.br == branching);
       }
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

  {LRStoppingData sd;
   assert(sd.st == OP::LRST::none);
   assert(sd.val == 0);
   assert(not sd);
  }

  {for (const LRST lrst : ET::allvals<LRST>())
     for (size_t val = 0; val <= 2; ++val) {
       LRStoppingData sd(lrst, val);
       assert(sd.st == lrst);
       assert(sd.val == val);
       assert(lrst == LRST::none ? not sd : sd);
     }
  }

}

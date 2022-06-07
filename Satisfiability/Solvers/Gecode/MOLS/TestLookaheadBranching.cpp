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

namespace {

  const Environment::ProgramInfo proginfo{
        "0.0.7",
        "7.6.2022",
        __FILE__,
        "Oleg Zaikin and Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Solvers/Gecode/MOLS/TestLookaheadBranching.cpp",
        "GPL v3"};

  using namespace LookaheadBranching;

  namespace GC = Gecode;
  namespace OP = Options;

  typedef GC::IntVarArray VarVec;
  struct GenericIntArray : GC::Space {
    VarVec V;
    GenericIntArray(const size_t varnum, const size_t domainsize = 1)
      noexcept : V(*this, varnum, 0, domainsize-1) {
      assert(varnum > 0 and domainsize > 0);
    }
  protected :
    GenericIntArray(GenericIntArray& gm) : GC::Space(gm), V(gm.V) {
      V.update(*this, gm.V);
    }
    GC::Space* copy() { return new GenericIntArray(*this); }
  };

  struct GecodeIntVarArray{
    typedef std::unique_ptr<GenericIntArray> intarr_ptr_t;
  private:
    intarr_ptr_t m;
    VarVec V;
  public:
    GecodeIntVarArray(const size_t varnum, const size_t domainsize = 1)
      noexcept {
      assert(varnum > 0 and domainsize > 0);
      m = intarr_ptr_t(new GenericIntArray(varnum, domainsize));
      V = m->V;
    }
    VarVec array() const noexcept { return V; }
  };
}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  {assert(tr(GecodeIntVarArray(1, 1).array()[0].size(), 1) == 1);
   assert(tr(GecodeIntVarArray(1, 2).array()[0].size(), 1) == 2);
   assert(tr(GecodeIntVarArray(2, 1).array()[0].size(), 1) == 1);
   assert(tr(GecodeIntVarArray(2, 1).array()[1].size(), 1) == 1);
   assert(tr(GecodeIntVarArray(2, 2).array()[0].size(), 1) == 2);
   assert(tr(GecodeIntVarArray(2, 2).array()[1].size(), 1) == 2);
  }

}

// Oleg Zaikin, 11.5.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
 Unit tests for GcVariables.hpp's helper functions.

TODOS:

1. Test functions from GcVariables
   - DONE sumdomsizes
   - DONE empty
   - DONE values
   - extract
   - set_var
   - unset_var

2. DONE (class GecodeIntVarArray was introduced)
   Class with access to array of Gecode variables
  - DONE A class must be intoduced that contains an array of
    Gecode variables and a trivial Gecode space.
  - DONE Constructor parameters: array size and domains size.
    As a result, constructor must create the space and the array.
  - DONE The class must provide public access to the array for testing
    all functions from GcVariables.

BUGS:

*/

#include <iostream>
#include <sstream>
#include <memory>

#include <cassert>

#include <gecode/int.hh>
#include <gecode/search.hh>

#include <ProgramOptions/Environment.hpp>

#include "Conditions.hpp"
#include "GcVariables.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.0.7",
        "27.5.2022",
        __FILE__,
        "Oleg Zaikin",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Solvers/Gecode/MOLS/TestGcVariables.cpp",
        "GPL v3"};

  using namespace GcVariables;

  namespace CD = Conditions;
  namespace GC = Gecode;

  using size_t = CD::size_t;

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

  template <class X>
  constexpr bool eqp(const X& lhs, const X& rhs) noexcept {
    return lhs == rhs;
  }

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  {const auto g = GecodeIntVarArray(1, 1);
   assert(sumdomsizes(g.array()) == 1 * 1);
   assert(empty(g.array()));
   assert(eqp(values(g.array()), {{0}}));
  }

  {const auto g = GecodeIntVarArray(1, 2);
   assert(sumdomsizes(g.array()) == 1 * 2);
   assert(not empty(g.array()));
   assert(eqp(values(g.array()), {{0,1}}));
  }

  {const auto g = GecodeIntVarArray(2, 1);
   assert(sumdomsizes(g.array()) == 2 * 1);
   assert(empty(g.array()));
   assert(eqp(values(g.array()), {{0}, {0}}));
  }

  {const auto g = GecodeIntVarArray(2, 2);
   assert(sumdomsizes(g.array()) == 2 * 2);
   assert(not empty(g.array()));
   assert(eqp(values(g.array()), {{0,1}, {0,1}}));
  }
}

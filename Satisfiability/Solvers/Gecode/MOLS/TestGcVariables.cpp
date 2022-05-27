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
  - A class must be intoduced that contains an array of
    Gecode variables and a trivial Gecode space.
  - Constructor parameters: array size and domains size.
    As a result, constructor must create the space and the array.
  - The class must provide public access to the array for testing
    all functions from GcVariables.

BUGS:

*/

#include <iostream>
#include <sstream>

#include <cassert>

#include <gecode/int.hh>
#include <gecode/search.hh>

#include <ProgramOptions/Environment.hpp>

#include "Conditions.hpp"
#include "GcVariables.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.0.3",
        "13.5.2022",
        __FILE__,
        "Oleg Zaikin",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Solvers/Gecode/MOLS/TestGcVariables.cpp",
        "GPL v3"};

  using namespace GcVariables;

  namespace CD = Conditions;
  namespace GC = Gecode;

  using size_t = CD::size_t;

  struct GenericIntArray : GC::Space {
    typedef GC::IntVarArray VarVec;
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

  using trivial_ptr_t = std::unique_ptr<GenericIntArray>;
}


int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  {trivial_ptr_t m(new GenericIntArray(1,1));
   assert(sumdomsizes(m->V) == 1);
  }
  {trivial_ptr_t m(new GenericIntArray(1,2));
   assert(sumdomsizes(m->V) == 2);
  }
  {trivial_ptr_t m(new GenericIntArray(2,2));
   assert(sumdomsizes(m->V) == 2 * 2);
  }
  {trivial_ptr_t m(new GenericIntArray(2,3));
   assert(sumdomsizes(m->V) == 2 * 3);
  }
}

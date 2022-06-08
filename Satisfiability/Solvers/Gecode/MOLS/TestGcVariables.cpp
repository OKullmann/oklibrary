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

2. DONE (class GcIntVarArray was introduced)
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

#include "GcVariables.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.3",
        "8.6.2022",
        __FILE__,
        "Oleg Zaikin",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Solvers/Gecode/MOLS/TestGcVariables.cpp",
        "GPL v3"};

  using namespace GcVariables;

  namespace GC = Gecode;

  template <class X>
  constexpr bool eqp(const X& lhs, const X& rhs) noexcept {
    return lhs == rhs;
  }

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  {const GcIntVarArray V(1, 1);
   assert(sumdomsizes(V) == 1 * 1);
   assert(empty(V));
   assert(eqp(values(V, 0), {0}));
   assert(eqp(values(V), {{0}}));
  }

  {const GcIntVarArray V(1, 2);
   assert(sumdomsizes(V) == 1 * 2);
   assert(not empty(V));
   assert(eqp(values(V, 0), {0,1}));
   assert(eqp(values(V), {{0,1}}));
  }

  {const GcIntVarArray V(2, 1);
   assert(sumdomsizes(V) == 2 * 1);
   assert(empty(V));
   assert(eqp(values(V, 0), {0}));
   assert(eqp(values(V, 1), {0}));
   assert(eqp(values(V), {{0}, {0}}));
  }

  {const GcIntVarArray V(2, 2);
   assert(sumdomsizes(V) == 2 * 2);
   assert(not empty(V));
   assert(eqp(values(V, 0), {0,1}));
   assert(eqp(values(V, 1), {0,1}));
   assert(eqp(values(V), {{0,1}, {0,1}}));
  }
}

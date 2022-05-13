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

#include "GcVariables.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.0.2",
        "13.5.2022",
        __FILE__,
        "Oleg Zaikin",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Solvers/Gecode/MOLS/TestGcVariables.cpp",
        "GPL v3"};

  using namespace GcVariables;

  namespace GC = Gecode;
}


int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;
}

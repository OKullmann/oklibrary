// Oleg Zaikin, 31.3.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

TODOS:

1. Urgently unit-tests are needed, for all components.
   - distance() function.
   - best_branching() function.
   - best_branchings() function.
   - post_la_branching() function.
   - Node class.
   - BinBranching class.
   - EnumBranching class.
   - BaseBrancher class.
   - BinLookahead class;
   - EnumLookahead class;

*/

#include <iostream>
#include <sstream>

#include <cassert>

#include <ProgramOptions/Environment.hpp>

#include "LookaheadBranching.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.0.3",
        "3.5.2022",
        __FILE__,
        "Oleg Zaikin and Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Solvers/Gecode/MOLS/TestLookaheadBranching.cpp",
        "GPL v3"};

  namespace LAB = LookaheadBranching;
}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

}

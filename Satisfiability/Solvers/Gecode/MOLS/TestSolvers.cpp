// Oliver Kullmann, 17.3.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <iostream>
#include <sstream>

#include <cassert>

#include <ProgramOptions/Environment.hpp>

#include "PartialSquares.hpp"
#include "Solvers.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.2.2",
        "18.4.2022",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/OKlib-MOLS/blob/master/Satisfiability/Solvers/Gecode/MOLS/TestSolvers.cpp",
        "GPL v3"};

  using namespace Solvers;
  using namespace PartialSquares;

  template <class X>
  constexpr bool eqp(const X& lhs, const X& rhs) noexcept {
    return lhs == rhs;
  }
}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  {assert(BasicSR{} == BasicSR{RT::sat_decision});
   assert((BasicSR{} == BasicSR{RT::sat_decision,0,{}}));
  }

  {assert(valid(BasicSR{}));
   assert((valid(BasicSR{RT::sat_decision,1,{}})));
   const PSquares ps(2,{});
   assert((not valid(BasicSR{RT::sat_decision,1,{ps}})));
   assert((not valid(BasicSR{RT::sat_decision,2,{}})));

   assert(valid(BasicSR{RT::sat_solving}));
   assert((valid(BasicSR{RT::sat_solving,0,{}})));
   assert((valid(BasicSR{RT::sat_solving,1,{ps}})));
   assert((not valid(BasicSR{RT::sat_solving,1,{}})));
   assert((valid(BasicSR{RT::sat_solving,1,{ps}})));
   assert((not valid(BasicSR{RT::sat_solving,2,{}})));

   assert(valid(BasicSR{RT::count_solutions}));
   assert(valid(BasicSR{RT::count_solutions,5}));
   assert(not valid(BasicSR{RT::count_solutions,1,{ps}}));
   assert(valid(BasicSR{RT::count_with_log}));
   assert(valid(BasicSR{RT::count_with_log,5}));
   assert(not valid(BasicSR{RT::count_with_log,1,{ps}}));

   assert(valid(BasicSR{RT::enumerate_solutions}));
   assert(not valid(BasicSR{RT::enumerate_solutions,1}));
   assert(valid(BasicSR{RT::enumerate_solutions,1,{ps}}));
   assert(not valid(BasicSR{RT::enumerate_solutions,2,{ps}}));
   assert(valid(BasicSR{RT::enumerate_solutions,2,{2,ps}}));
  }

}

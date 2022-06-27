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
#include "Verification.hpp"
#include "BasicLatinSquares.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.4.0",
        "27.6.2022",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Solvers/Gecode/MOLS/TestSolvers.cpp",
        "GPL v3"};

  using namespace Solvers;
  using namespace PartialSquares;
  using namespace Verification;
  using namespace BasicLatinSquares;

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
   const PSquares ps(2,psquares_t{});
   assert((not valid(BasicSR{RT::sat_decision,1,{ps}})));
   assert((valid(BasicSR{RT::sat_decision,2,{}})));

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

  {std::istringstream cd("squares A\n");
   std::istringstream ps;
   {const auto res = solver0(RT::unique_decision, 2, cd, ps);
    assert(valid(res));
    assert((eqp(res, {RT::unique_decision, 2, {}})));
   }
   cd.clear(); cd.str("squares A\n");
   ps.clear(); ps.str("A\n1 1\n1 1\n");
   {const auto res = solver0(RT::unique_decision, 2, cd, ps);
    assert(valid(res));
    assert((eqp(res, {RT::unique_decision, 1, {}})));
   }
   cd.clear(); cd.str("squares A\nls A\n");
   ps.clear(); ps.str("A\n1 1\n1 1\n");
   {const auto res = solver0(RT::unique_decision, 2, cd, ps);
    assert(valid(res));
    assert((eqp(res, {RT::unique_decision, 0, {}})));
   }
  }
  {std::istringstream cd("squares A\n");
   std::istringstream ps;
   {const auto res = solver0(RT::unique_solving, 2, cd, ps);
    assert(valid(res));
    assert(res.rt == RT::unique_solving);
    assert(res.sol_found == 2);
    assert(eqp(extract(res.list_sol), {
               {{{0,0},{0,0}}},
               {{{0,0},{0,1}}}
             }));
   }
   cd.clear(); cd.str("squares A\n");
   ps.clear(); ps.str("A\n1 1\n1 1\n");
   {const auto res = solver0(RT::unique_solving, 2, cd, ps);
    assert(valid(res));
    assert(res.rt == RT::unique_solving);
    assert(res.sol_found == 1);
    assert(eqp(extract(res.list_sol), {
               {{{1,1},{1,1}}}
             }));
   }
   cd.clear(); cd.str("squares A\nls A\n");
   ps.clear(); ps.str("A\n1 1\n1 1\n");
   {const auto res = solver0(RT::unique_solving, 2, cd, ps);
    assert(valid(res));
    assert((eqp(res, {RT::unique_solving, 0, {}})));
   }
  }

}

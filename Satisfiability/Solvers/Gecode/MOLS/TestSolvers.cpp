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

#include "Solvers.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.0",
        "25.3.2022",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/OKlib-MOLS/blob/master/Satisfiability/Solvers/Gecode/MOLS/TestSolvers.cpp",
        "GPL v3"};

  using namespace Solvers;

  template <class X>
  constexpr bool eqp(const X& lhs, const X& rhs) noexcept {
    return lhs == rhs;
  }
}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  {std::istringstream ss_cond("squares A\n");
   std::istringstream ss_ps("");
   const auto res = solver0(RT::count_solutions, 2, ss_cond, ss_ps);
   assert(res.sol_found == 16);
  }
  {std::istringstream ss_cond("squares A\n");
   std::istringstream ss_ps("");
   const auto res = solver0(RT::count_solutions, 3, ss_cond, ss_ps);
   assert(res.sol_found == 19683);
  }

}

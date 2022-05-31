// Oleg Zaikin, 23.5.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

Test test cases for solvers.

BUG:

TODOS:

*/

#include <cassert>

#include <ProgramOptions/Environment.hpp>
#include <Numerics/FloatingPoint.hpp>

#include "Cases.hpp"
#include "Options.hpp"
#include "Solvers.hpp"
#include "PartialSquares.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.4",
        "31.5.2022",
        __FILE__,
        "Oleg Zaikin and Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Solvers/Gecode/MOLS/TestCases.cpp",
        "GPL v3"};

  using namespace Cases;
  using namespace Options;
  using namespace Solvers;
  using namespace PartialSquares;

  namespace FP = FloatingPoint;

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  {Square A(2);
   assert(A.N == 2);
   assert(A.e.N == 2);
   assert(A.e.num_vars == 2 * 2);
   assert(A.numsol() == solver0(A.e, RT::count_solutions).sol_found);
   assert(A.space());
   assert(A.laredstats(LAR::eag_pr).probes() == FP::pow(2, 3));
  }
  {Square A(2, "A\n0 *\n* *\n");
   assert(A.e.num_vars == 2 * 2);
   assert(A.numsol() == solver0(A.e, RT::count_solutions).sol_found);
   assert(A == Square(2,
                      {2, psquares_t{ {
                           ps_map_t{ {{0,0}, Cell(2,0)} },
                             2, 0} } } ));
  }

  {Square A(3);
   assert(A.e.num_vars == 3 * 3);
   assert(A.laredstats(LAR::eag_pr).probes() == FP::pow(3, 3));
   assert(A.numsol() == solver0(A.e, RT::count_solutions).sol_found);
  }
  {Square A(3, "A\n0 * *\n* * *\n* * *\n");
   assert(A.e.num_vars == 3 * 3);
   assert(A.numsol() == solver0(A.e, RT::count_solutions).sol_found);
  }

  {LaSq A(2);
   assert(A.e.num_vars == 4);
   assert(A.numsol() == solver0(A.e, RT::count_solutions).sol_found);
   assert(A.space());
   assert(A.laredstats(LAR::eag_pr).solc() == 2);
   assert(A.laredstats(LAR::eag_pr).elims().size() == 2);
  }
  {LaSq A(2, "A\n0 *\n* *\n");
   assert(A.e.num_vars == 4);
   assert(A.numsol() == solver0(A.e, RT::count_solutions).sol_found);
  }

  {LaSq A(3);
   assert(A.e.num_vars == 3 * 3);
   assert(A.numsol() == solver0(A.e, RT::count_solutions).sol_found);
   assert(A.laredstats(LAR::eag_pr).solc() == 0);
  }
  {LaSq A(3, "A\n0 * *\n* * *\n* * *\n");
   assert(A.e.num_vars == 3 * 3);
   assert(A.numsol() == solver0(A.e, RT::count_solutions).sol_found);
  }

}

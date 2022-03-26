// Oliver Kullmann, 14.3.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <iostream>

#include <cassert>

#include <ProgramOptions/Environment.hpp>

#include "Encoding.hpp"
#include "Solvers.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.2.0",
        "26.3.2022",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/OKlib-MOLS/blob/master/Satisfiability/Solvers/Gecode/MOLS/TestEncoding.cpp",
        "GPL v3"};

  using namespace Encoding;
  using namespace Conditions;
  using namespace PartialSquares;
  using namespace Solvers;

  template <class X>
  constexpr bool eqp(const X& lhs, const X& rhs) noexcept {
    return lhs == rhs;
  }
}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  {const EncCond enc(AConditions(0), PSquares(2,{}));
   assert(enc.ac.k == 0);
   assert(enc.N == 2);
   assert(enc.N2 == 4);
   assert(enc.num_vars == 0);
  }

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
  {std::istringstream ss_cond("squares A\n");
   std::istringstream ss_ps("A\n* * *\n* * *\n* * *\n");
   const auto res = solver0(RT::count_solutions, 3, ss_cond, ss_ps);
   assert(res.sol_found == 19683);
  }
  {std::istringstream ss_cond("squares A\n");
   std::istringstream ss_ps("A\n1 1 1\n2 2 2\n3 3 3\n");
   const auto res = solver0(RT::count_solutions, 3, ss_cond, ss_ps);
   assert(res.sol_found == 1);
  }
  {std::istringstream ss_cond("squares A\n");
   std::istringstream ss_ps("A\n-3 1 1\n2 2 2\n3 3 3\n");
   const auto res = solver0(RT::count_solutions, 3, ss_cond, ss_ps);
   assert(res.sol_found == 2);
  }
  {std::istringstream ss_cond("squares A\n");
   std::istringstream ss_ps("A\n1,2 1,3 2,3\n1 2 3\n* - +1,2\n");
   const auto res = solver0(RT::enumerate_solutions, 3, ss_cond, ss_ps);
   assert(res.sol_found == 144);
  }
  {std::istringstream ss_cond("squares A B\n");
   std::istringstream ss_ps("");
   const auto res = solver0(RT::count_solutions, 2, ss_cond, ss_ps);
   assert(res.sol_found == 256);
  }
  {std::istringstream ss_cond("squares A B\n");
   std::istringstream ss_ps("A\n1 1 1\n- 2 2\n3 3 3\n"
                            "B\n1 1 1\n2 2 2\n3 3 *\n");
   const auto res = solver0(RT::count_solutions, 3, ss_cond, ss_ps);
   assert(res.sol_found == 9);
  }

  {std::istringstream ss_cond("squares A\ndiag A\n");
   std::istringstream ss_ps("");
   const auto res = solver0(RT::count_solutions, 2, ss_cond, ss_ps);
   assert(res.sol_found == 8);
  }
  {std::istringstream ss_cond("squares A\ndiag A\nantidiag A\n");
   std::istringstream ss_ps("");
   const auto res = solver0(RT::count_solutions, 2, ss_cond, ss_ps);
   assert(res.sol_found == 4);
  }
  {std::istringstream ss_cond("squares A\nidem A\n");
   std::istringstream ss_ps("");
   const auto res = solver0(RT::count_solutions, 2, ss_cond, ss_ps);
   assert(res.sol_found == 4);
  }
  {std::istringstream ss_cond("squares A\nantiidem A\n");
   std::istringstream ss_ps("");
   const auto res = solver0(RT::count_solutions, 2, ss_cond, ss_ps);
   assert(res.sol_found == 4);
  }
  {std::istringstream ss_cond("squares A\nidem A\nantiidem A\n");
   std::istringstream ss_ps("");
   const auto res = solver0(RT::count_solutions, 2, ss_cond, ss_ps);
   assert(res.sol_found == 1);
  }
  {std::istringstream ss_cond("squares A\nrred A\n");
   std::istringstream ss_ps("");
   const auto res = solver0(RT::count_solutions, 2, ss_cond, ss_ps);
   assert(res.sol_found == 4);
  }
  {std::istringstream ss_cond("squares A\norred A\n");
   std::istringstream ss_ps("");
   const auto res = solver0(RT::count_solutions, 2, ss_cond, ss_ps);
   assert(res.sol_found == 4);
  }
  {std::istringstream ss_cond("squares A\nrred A\norred A\n");
   std::istringstream ss_ps("");
   const auto res = solver0(RT::count_solutions, 2, ss_cond, ss_ps);
   assert(res.sol_found == 1);
  }
  {std::istringstream ss_cond("squares A\ncred A\n");
   std::istringstream ss_ps("");
   const auto res = solver0(RT::count_solutions, 2, ss_cond, ss_ps);
   assert(res.sol_found == 4);
  }
  {std::istringstream ss_cond("squares A\nocred A\n");
   std::istringstream ss_ps("");
   const auto res = solver0(RT::count_solutions, 2, ss_cond, ss_ps);
   assert(res.sol_found == 4);
  }
  {std::istringstream ss_cond("squares A\ncred A\nocred A\n");
   std::istringstream ss_ps("");
   const auto res = solver0(RT::count_solutions, 2, ss_cond, ss_ps);
   assert(res.sol_found == 1);
  }
  {std::istringstream ss_cond("squares A\nrred A\ncred A \n");
   std::istringstream ss_ps("");
   const auto res = solver0(RT::count_solutions, 2, ss_cond, ss_ps);
   assert(res.sol_found == 2);
  }
  {std::istringstream ss_cond("squares A\norred A\nocred A \n");
   std::istringstream ss_ps("");
   const auto res = solver0(RT::count_solutions, 2, ss_cond, ss_ps);
   assert(res.sol_found == 2);
  }

}

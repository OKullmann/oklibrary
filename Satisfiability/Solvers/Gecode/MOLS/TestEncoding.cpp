// Oliver Kullmann, 14.3.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <iostream>

#include <cassert>

#include <ProgramOptions/Environment.hpp>
#include <Numerics/FloatingPoint.hpp>
#include <Transformers/Generators/Random/LatinSquares.hpp>

#include "Encoding.hpp"
#include "Solvers.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.2.2",
        "26.3.2022",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/OKlib-MOLS/blob/master/Satisfiability/Solvers/Gecode/MOLS/TestEncoding.cpp",
        "GPL v3"};

  using namespace Encoding;
  using namespace Conditions;
  using namespace PartialSquares;
  using namespace Solvers;
  namespace FP = FloatingPoint;
  namespace LS = LatinSquares;

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
   assert(res.sol_found == FP::pow(2,2*2));
  }
  {std::istringstream ss_cond("squares A\n");
   std::istringstream ss_ps("");
   const auto res = solver0(RT::count_solutions, 3, ss_cond, ss_ps);
   assert(res.sol_found == FP::pow(3,3*3));
  }
  {std::istringstream ss_cond("squares A\n");
   std::istringstream ss_ps("A\n* * *\n* * *\n* * *\n");
   const auto res = solver0(RT::count_solutions, 3, ss_cond, ss_ps);
   assert(res.sol_found == FP::pow(3,3*3));
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
   assert(res.sol_found == 2*2*2*3*3*2);
  }
  {std::istringstream ss_cond("squares A B\n");
   std::istringstream ss_ps("");
   const auto res = solver0(RT::count_solutions, 2, ss_cond, ss_ps);
   assert(res.sol_found == FP::sq(FP::pow(2,2*2)));
  }
  {std::istringstream ss_cond("squares A B\n");
   std::istringstream ss_ps("A\n1 1 1\n- 2 2\n3 3 3\n"
                            "B\n1 1 1\n2 2 2\n3 3 *\n");
   const auto res = solver0(RT::count_solutions, 3, ss_cond, ss_ps);
   assert(res.sol_found == 3 * 3);
  }

  {std::istringstream ss_cond("squares A\ndiag A\n");
   std::istringstream ss_ps("");
   const auto res = solver0(RT::count_solutions, 2, ss_cond, ss_ps);
   assert(res.sol_found == FP::factorial(2)*FP::pow(2,2));
  }
  {std::istringstream ss_cond("squares A\ndiag A\nantidiag A\n");
   std::istringstream ss_ps("");
   const auto res = solver0(RT::count_solutions, 2, ss_cond, ss_ps);
   assert(res.sol_found == 2*2);
  }
  {std::istringstream ss_cond("squares A\nuni A\n");
   std::istringstream ss_ps("");
   const auto res = solver0(RT::count_solutions, 2, ss_cond, ss_ps);
   assert(res.sol_found == 2 * FP::pow(2,2));
  }
  {std::istringstream ss_cond("squares A\nantiuni A\n");
   std::istringstream ss_ps("");
   const auto res = solver0(RT::count_solutions, 2, ss_cond, ss_ps);
   assert(res.sol_found == 2 * FP::pow(2,2));
  }
  {std::istringstream ss_cond("squares A\nuni A\nantiuni A\n");
   std::istringstream ss_ps("");
   const auto res = solver0(RT::count_solutions, 2, ss_cond, ss_ps);
   assert(res.sol_found == 2*2);
  }
  {std::istringstream ss_cond("squares A\nuni A\nantiuni A\n");
   std::istringstream ss_ps("");
   const auto res = solver0(RT::count_solutions, 3, ss_cond, ss_ps);
   assert(res.sol_found == FP::pow(3,5));
  }
  {std::istringstream ss_cond("squares A\nidem A\n");
   std::istringstream ss_ps("");
   const auto res = solver0(RT::count_solutions, 2, ss_cond, ss_ps);
   assert(res.sol_found == FP::pow(2,2));
  }
  {std::istringstream ss_cond("squares A\nantiidem A\n");
   std::istringstream ss_ps("");
   const auto res = solver0(RT::count_solutions, 2, ss_cond, ss_ps);
   assert(res.sol_found == FP::pow(2,2));
  }
  {std::istringstream ss_cond("squares A\nidem A\nantiidem A\n");
   std::istringstream ss_ps("");
   const auto res = solver0(RT::count_solutions, 2, ss_cond, ss_ps);
   assert(res.sol_found == 1);
  }
  {std::istringstream ss_cond("squares A\nrred A\n");
   std::istringstream ss_ps("");
   const auto res = solver0(RT::count_solutions, 2, ss_cond, ss_ps);
   assert(res.sol_found == FP::pow(2,2));
  }
  {std::istringstream ss_cond("squares A\norred A\n");
   std::istringstream ss_ps("");
   const auto res = solver0(RT::count_solutions, 2, ss_cond, ss_ps);
   assert(res.sol_found == FP::pow(2,2));
  }
  {std::istringstream ss_cond("squares A\nrred A\norred A\n");
   std::istringstream ss_ps("");
   const auto res = solver0(RT::count_solutions, 2, ss_cond, ss_ps);
   assert(res.sol_found == 1);
  }
  {std::istringstream ss_cond("squares A\ncred A\n");
   std::istringstream ss_ps("");
   const auto res = solver0(RT::count_solutions, 2, ss_cond, ss_ps);
   assert(res.sol_found == FP::pow(2,2));
  }
  {std::istringstream ss_cond("squares A\nocred A\n");
   std::istringstream ss_ps("");
   const auto res = solver0(RT::count_solutions, 2, ss_cond, ss_ps);
   assert(res.sol_found == FP::pow(2,2));
  }
  {std::istringstream ss_cond("squares A\ncred A\nocred A\n");
   std::istringstream ss_ps("");
   const auto res = solver0(RT::count_solutions, 2, ss_cond, ss_ps);
   assert(res.sol_found == 1);
  }
  {std::istringstream ss_cond("squares A\nrred A\ncred A\n");
   std::istringstream ss_ps("");
   const auto res = solver0(RT::count_solutions, 2, ss_cond, ss_ps);
   assert(res.sol_found == 2);
  }
  {std::istringstream ss_cond("squares A\nred A\n");
   std::istringstream ss_ps("");
   const auto res = solver0(RT::count_solutions, 2, ss_cond, ss_ps);
   assert(res.sol_found == 2);
  }
  {std::istringstream ss_cond("squares A\norred A\nocred A\n");
   std::istringstream ss_ps("");
   const auto res = solver0(RT::count_solutions, 2, ss_cond, ss_ps);
   assert(res.sol_found == 2);
  }
  {std::istringstream ss_cond("squares A\nored A\n");
   std::istringstream ss_ps("");
   const auto res = solver0(RT::count_solutions, 2, ss_cond, ss_ps);
   assert(res.sol_found == 2);
  }
  {std::istringstream ss_cond("squares A\ncred A\norred A\n");
   std::istringstream ss_ps("");
   const auto res = solver0(RT::count_solutions, 3, ss_cond, ss_ps);
   assert(res.sol_found == 0);
  }
  {for (size_t n = 2; n <= 3; ++n) {
     std::istringstream ss_cond("squares A\nrls A\n");
     std::istringstream ss_ps("");
     const auto res = solver0(RT::count_solutions, n, ss_cond, ss_ps);
     assert(res.sol_found == FP::pow(FP::factorial(n), n));
   }
  }
  {for (size_t n = 2; n <= 3; ++n) {
     std::istringstream ss_cond("squares A\ncls A\n");
     std::istringstream ss_ps("");
     const auto res = solver0(RT::count_solutions, n, ss_cond, ss_ps);
     assert(res.sol_found == FP::pow(FP::factorial(n), n));
   }
  }
  {for (size_t n = 2; n <= 3; ++n) {
     std::istringstream ss_cond("squares A\ncls A\n");
     std::istringstream ss_ps("");
     const auto res = solver0(RT::count_solutions, n, ss_cond, ss_ps);
     assert(res.sol_found == FP::pow(FP::factorial(n), n));
   }
  }
  {for (size_t n = 2; n <= 4; ++n) {
     std::istringstream ss_cond("squares A\nls A\n");
     std::istringstream ss_ps("");
     const auto res = solver0(RT::count_solutions, n, ss_cond, ss_ps);
     assert(res.sol_found == LS::count_ls(n, LS::StRLS::none));
   }
  }
  {for (size_t n = 2; n <= 4; ++n) {
     std::istringstream ss_cond("squares A\nls A\nrred A\n");
     std::istringstream ss_ps("");
     const auto res = solver0(RT::count_solutions, n, ss_cond, ss_ps);
     assert(res.sol_found == LS::count_ls(n, LS::StRLS::row));
   }
  }
  {for (size_t n = 2; n <= 4; ++n) {
     std::istringstream ss_cond("squares A\nls A\ncred A\n");
     std::istringstream ss_ps("");
     const auto res = solver0(RT::count_solutions, n, ss_cond, ss_ps);
     assert(res.sol_found == LS::count_ls(n, LS::StRLS::column));
   }
  }
  {for (size_t n = 2; n <= 6; ++n) {
     std::istringstream ss_cond("squares A\nls A\nrred A\ncred A\n");
     std::istringstream ss_ps("");
     const auto res = solver0(RT::count_solutions, n, ss_cond, ss_ps);
     assert(res.sol_found == LS::count_ls(n, LS::StRLS::both));
   }
  }
  {for (size_t n = 2; n <= 3; ++n) {
     std::istringstream ss_cond("squares A\nsymm A\n");
     std::istringstream ss_ps("");
     const auto res = solver0(RT::count_solutions, n, ss_cond, ss_ps);
     assert(res.sol_found == FP::pow(n,(n*(n+1))/2));
    }
  }
  {for (size_t n = 2; n <= 3; ++n) {
     std::istringstream ss_cond("squares A\nantisymm A\n");
     std::istringstream ss_ps("");
     const auto res = solver0(RT::count_solutions, n, ss_cond, ss_ps);
     assert(res.sol_found == FP::pow(n,(n*(n+1))/2));
    }
  }
  {for (size_t n = 2; n <= 3; ++n) {
     std::istringstream ss_cond("squares A\nsymm A\nantisymm A\n");
     std::istringstream ss_ps("");
     const auto res = solver0(RT::count_solutions, n, ss_cond, ss_ps);
     assert(res.sol_found == (n==2 ? FP::pow(2,2) : FP::pow(3,4)));
    }
  }
  // https://oeis.org/A000438 reduced symmetric unipotent ls's of order 2n
  // (https://oeis.org/A003191 seems nonsense):
  constexpr std::array<size_t,6> su{1,1,1,6,6240,1225566720};
  {for (size_t n = 1; n <= 4; ++n) {
     std::istringstream ss_cond("squares A\nls A\nsymm A\nuni A\nred A\n");
     std::istringstream ss_ps("");
     const auto res = solver0(RT::count_solutions, 2*n, ss_cond, ss_ps);
     assert(res.sol_found == su[n]);
    }
  }
  {for (size_t n = 1; n <= 3; ++n) {
     std::istringstream ss_cond("squares A\nls A\nsymm A\nuni A\nored A\n");
     std::istringstream ss_ps("");
     const auto res = solver0(RT::count_solutions, 2*n, ss_cond, ss_ps);
     assert(res.sol_found == su[n]);
    }
  }
}

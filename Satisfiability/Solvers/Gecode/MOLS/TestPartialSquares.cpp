// Oliver Kullmann, 21.3.2022 (Swansea)
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

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.2",
        "23.3.2022",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/OKlib-MOLS/blob/master/Satisfiability/Solvers/Gecode/MOLS/TestPartialSquares.cpp",
        "GPL v3"};

  using namespace PartialSquares;

  template <class X>
  constexpr bool eqp(const X& lhs, const X& rhs) noexcept {
    return lhs == rhs;
  }
}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  {assert(valid(prow_t(0)));
   assert(not valid(prow_t(1)));
   assert(valid(psquare_t(0)));
   assert(not valid(psquare_t(1)));
   for (size_t N = 0; N <= 5; ++N) {
     assert(valid(empty_prow(N), N));
     assert(valid(empty_psquare(N), N));
     assert(valid(PSquare(N),N));
   }
  }

  {std::istringstream ss;
   PSquares p(2, ss);
   assert(p.N == 2);
   assert(p.psqs.empty());
  }
  {std::istringstream ss("\n\t\n # \n #");
   PSquares p(2, ss);
   assert(p.N == 2);
   assert(p.psqs.empty());
  }
  {std::istringstream ss("\n\t\n # \n #\n gh");
   PSquares p(2, ss);
   assert(p.N == 2);
   assert(p.psqs.empty());
  }

}

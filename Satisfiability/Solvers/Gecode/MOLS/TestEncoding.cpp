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

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.0",
        "25.3.2022",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/OKlib-MOLS/blob/master/Satisfiability/Solvers/Gecode/MOLS/TestEncoding.cpp",
        "GPL v3"};

  using namespace Encoding;
  using namespace Conditions;
  using namespace PartialSquares;

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
}

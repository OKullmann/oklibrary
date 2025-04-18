// Oliver Kullmann, 16.3.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <iostream>
#include <sstream>

#include <cassert>

#include <ProgramOptions/Environment.hpp>

#include "Constraints.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.0.6",
        "10.8.2022",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Solvers/Gecode/MOLS/TestConstraints.cpp",
        "GPL v3"};

  using namespace Constraints;

  template <class X>
  constexpr bool eqp(const X& lhs, const X& rhs) noexcept {
    return lhs == rhs;
  }
}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  {NodeData nd;
   assert(nd.id == 18446744073709551615UL);
   assert(nd.pid == 0);
   assert(nd.branch == 18446744073709551615UL);
   assert(nd.depth == 0);
   assert(nd == NodeData{});
   std::stringstream ss;
   ss << nd;
   assert(ss.str() == "18446744073709551615 0 18446744073709551615 0");
  }

}

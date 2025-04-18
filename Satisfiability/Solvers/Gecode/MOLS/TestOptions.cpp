// Oliver Kullmann, 25.5.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <iostream>

#include <cassert>

#include <ProgramOptions/Environment.hpp>

#include "Options.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.0",
        "25.5.2022",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Solvers/Gecode/MOLS/TestOptions.cpp",
        "GPL v3"};

  using namespace Options;

  template <class X>
  constexpr bool eqp(const X& lhs, const X& rhs) noexcept {
    return lhs == rhs;
  }
}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  {assert(Environment::width_policy<RT>() == 8);
   assert((Environment::width_policy<RT,true>() == 8));
   assert(Environment::width_policy<PropO>() == 3);
   assert((Environment::width_policy<PropO,true>() == 3));
   assert((Environment::width_policy<PropO,false,false>() == 12));
   assert((Environment::width_policy<PropO,true,false>() == 17));
  }

}

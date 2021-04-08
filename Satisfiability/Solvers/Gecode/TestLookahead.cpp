// Oleg Zaikin, 6.4.2020 (Irkutsk)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <iostream>

#include <cassert>

#include <ProgramOptions/Environment.hpp>

#include "Trivial.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.0.6",
        "8.4.2021",
        __FILE__,
        "Oleg Zaikin and Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Solvers/Gecode/TestLookahead.cpp",
        "GPL v3"};

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  {Trivial::IntArr m(1, 0, 0);
   assert(m.size() == 1);
   assert(m.mu0() == 0);
   assert(m.mu1() == 0);
   Trivial::IntArr m2(m);
   assert(m2.size() == 1);
   assert(m2.mu0() == 0);
   assert(m2.mu1() == 0);
  }

  {Trivial::IntArr m(2, 0, 2);
   assert(m.size() == 2);
   assert(m.mu0() == 4);
   assert(m.mu1() == 2);
   Trivial::IntArr m2(m);
   assert(m2.size() == 2);
   assert(m2.mu0() == 4);
   assert(m2.mu1() == 2);
  }
}

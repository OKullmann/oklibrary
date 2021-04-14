// Oleg Zaikin, 6.4.2020 (Irkutsk)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <iostream>

#include <cassert>
#include <cmath>

#include <ProgramOptions/Environment.hpp>
#include <Numerics/FloatingPoint.hpp>

#include "Trivial.hpp"

namespace LA = Lookahead;

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.5",
        "14.4.2021",
        __FILE__,
        "Oleg Zaikin and Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Solvers/Gecode/TestLookahead.cpp",
        "GPL v3"};

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  {Trivial::Sum m(1, 0, 0);
   assert(m.size() == 1);
   assert(m.valid(0));
   assert(not m.valid(1));
   assert(m.mu0() == 0);
   assert(m.mu1() == 0);
   Trivial::Sum m2(m);
   assert(m == m2);
   Trivial::Sum m3(2, 0, 2);
   assert(m != m3);
  }

  {Trivial::Sum m(2, 0, 2);
   assert(m.size() == 2);
   assert(m.valid(0));
   assert(m.valid(1));
   assert(not m.valid(2));
   assert(m.mu0() == 4);
   assert(m.mu1() == 2*FloatingPoint::log2(3));
   Trivial::Sum m2(1, 0, 0);
   assert(m != m2);
  }

  {Trivial::Sum* const m = new Trivial::Sum(1, 0, 0);
   assert(m->size() == 1);
   assert(m->valid(0));
   assert(not m->valid(1));
   m->status();
   assert(m->propagate(0, 0) == 0);
   delete m;
  }

  {Trivial::Sum* const m = new Trivial::Sum(3, 0, 2);
   assert(m->size() == 3);
   assert(m->valid(0));
   assert(m->valid(1));
   assert(m->valid(2));
   assert(not m->valid(3));
   m->status();
   assert(m->propagate(0, 0) == 4);
   assert(m->propagate(0, 1) == 2);
   assert(m->propagate(0, 2) == 0);
   assert(m->propagate(1, 0) == 4);
   assert(m->propagate(1, 1) == 2);
   assert(m->propagate(1, 2) == 0);
   assert(m->propagate(2, 0) == 0);
   assert(m->propagate(2, 1) == 2);
   assert(m->propagate(2, 2) == 4);
   delete m;
  }
}

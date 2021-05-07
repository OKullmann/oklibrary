// Oleg Zaikin, 6.4.2020 (Irkutsk)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

TODOS:

1. DONE Update to new pointer-management.

*/

#include <iostream>
#include <memory>
#include <cassert>
#include <cmath>

#include <ProgramOptions/Environment.hpp>
#include <Numerics/FloatingPoint.hpp>

#include "Trivial.hpp"

namespace LA = Lookahead;

namespace {

  typedef std::shared_ptr<Trivial::Sum> trivial_sum_ptr;

  const Environment::ProgramInfo proginfo{
        "0.2.2",
        "7.5.2021",
        __FILE__,
        "Oleg Zaikin and Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Solvers/Gecode/TestLookahead.cpp",
        "GPL v3"};

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  {const trivial_sum_ptr m(new Trivial::Sum(1, 0, 0));
   assert(m->valid());
   assert(m->size() == 1);
   assert(m->valid(0));
   assert(not m->valid(1));
   m->branching_min_var_size();
   LA::SearchStat stat = LA::find_all_solutions<Trivial::Sum>(m);
   assert(stat.solutions == 1);
   m->status();
   assert(m->la_measure(0, 0) == 0);
  }

  {const trivial_sum_ptr m(new Trivial::Sum(2, 0, 1));
   assert(m->valid());
   assert(m->size() == 2);
   assert(m->valid(0));
   assert(m->valid(1));
   assert(not m->valid(2));
   m->branching_min_var_size();
   LA::SearchStat stat = LA::find_all_solutions<Trivial::Sum>(m);
   assert(stat.solutions == 2);
   m->status();
   assert(m->la_measure(0, 0) == 0);
   assert(m->la_measure(0, 1) == 0);
   assert(m->la_measure(1, 0) == 0);
   assert(m->la_measure(1, 1) == 0);
  }

  {const trivial_sum_ptr m(new Trivial::Sum(2, 0, 2));
   assert(m->valid());
   assert(m->size() == 2);
   assert(m->valid(0));
   assert(m->valid(1));
   assert(not m->valid(2));
   assert(m->mu0() == 4);
   assert(m->mu1() == 2*FloatingPoint::log2(3));
   m->branching_min_var_size();
   LA::SearchStat stat = LA::find_all_solutions<Trivial::Sum>(m);
   assert(stat.solutions == 3);
  }

  {const trivial_sum_ptr m(new Trivial::Sum(3, 0, 1));
   assert(m->valid());
   assert(m->size() == 3);
   assert(m->valid(0));
   assert(m->valid(1));
   assert(m->valid(2));
   assert(not m->valid(3));
   m->branching_min_var_size();
   LA::SearchStat stat = LA::find_all_solutions<Trivial::Sum>(m);
   assert(stat.solutions == 3);
   m->status();
   assert(m->la_measure(0, 0) == 2);
   assert(m->la_measure(0, 1) == 0);
   assert(m->la_measure(1, 0) == 2);
   assert(m->la_measure(1, 1) == 0);
   assert(m->la_measure(2, 0) == 0);
   assert(m->la_measure(2, 1) == 2);
  }

  {const trivial_sum_ptr m(new Trivial::Sum(3, 0, 2));
   assert(m->valid());
   assert(m->size() == 3);
   assert(m->valid(0));
   assert(m->valid(1));
   assert(m->valid(2));
   assert(not m->valid(3));
   m->branching_min_var_size();
   LA::SearchStat stat = LA::find_all_solutions<Trivial::Sum>(m);
   assert(stat.solutions == 6);
   m->status();
   assert(m->la_measure(0, 0) == 4);
   assert(m->la_measure(0, 1) == 2);
   assert(m->la_measure(0, 2) == 0);
   assert(m->la_measure(1, 0) == 4);
   assert(m->la_measure(1, 1) == 2);
   assert(m->la_measure(1, 2) == 0);
   assert(m->la_measure(2, 0) == 0);
   assert(m->la_measure(2, 1) == 2);
   assert(m->la_measure(2, 2) == 4);
  }
}

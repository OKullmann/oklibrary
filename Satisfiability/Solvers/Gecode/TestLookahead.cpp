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

#include <gecode/int.hh>
#include <gecode/search.hh>

#include "Trivial.hpp"

namespace LA = Lookahead;

namespace {

  namespace GC = Gecode;

  typedef std::shared_ptr<Trivial::Sum> trivial_sum_ptr;

  const Environment::ProgramInfo proginfo{
        "0.2.10",
        "17.5.2021",
        __FILE__,
        "Oleg Zaikin and Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Solvers/Gecode/TestLookahead.cpp",
        "GPL v3"};

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  {const auto b = LA::BranchingO::binarysizeminvalmin;
   const trivial_sum_ptr m(new Trivial::Sum(1, 0, 0, b));
   assert(m->valid());
   assert(m->size() == 1);
   assert(m->valid(0));
   assert(not m->valid(1));
   [[maybe_unused]] auto const st = m->status();
   assert(st == GC::SS_SOLVED);
   LA::SearchStat stat = LA::find_all_solutions<Trivial::Sum>(m);
   assert(stat.solutions == 1);

   const auto b2 = LA::BranchingO::narysizeminvalmin;
   const trivial_sum_ptr m2(new Trivial::Sum(1, 0, 0, b2));
   assert(m2->valid());
   assert(m2->size() == 1);
   assert(m2->valid(0));
   assert(not m2->valid(1));
   [[maybe_unused]] auto const st2 = m2->status();
   assert(st2 == GC::SS_SOLVED);
   LA::SearchStat stat2 = LA::find_all_solutions<Trivial::Sum>(m2);
   assert(stat2.solutions == stat.solutions);

   const auto b3 = LA::BranchingO::narylookahead;
   const trivial_sum_ptr m3(new Trivial::Sum(1, 0, 0, b3));
   assert(m3->valid());
   assert(m3->size() == 1);
   assert(m3->valid(0));
   assert(not m3->valid(1));
   [[maybe_unused]] auto const st3 = m3->status();
   assert(st3 == GC::SS_SOLVED);
   const auto r = LA::la_measure<Trivial::Sum>(m3.get(), 0, 0);
   assert(r.measure == -1.0 and r.status == GC::SS_SOLVED);
   LA::SearchStat stat3 = LA::find_all_solutions<Trivial::Sum>(m3);
   assert(stat3.solutions == stat.solutions);
  }

  {const auto b = LA::BranchingO::binarysizeminvalmin;
   const trivial_sum_ptr m(new Trivial::Sum(2, 0, 1, b));
   assert(m->valid());
   assert(m->size() == 2);
   assert(m->valid(0));
   assert(m->valid(1));
   assert(not m->valid(2));
   [[maybe_unused]] auto const st = m->status();
   assert(st == GC::SS_BRANCH);
   LA::SearchStat stat = LA::find_all_solutions<Trivial::Sum>(m);
   assert(stat.solutions == 2);

   const auto b2 = LA::BranchingO::narysizeminvalmin;
   const trivial_sum_ptr m2(new Trivial::Sum(2, 0, 1, b2));
   assert(m2->valid());
   assert(m2->size() == 2);
   assert(m2->valid(0));
   assert(m2->valid(1));
   assert(not m2->valid(2));
   [[maybe_unused]] auto const st2 = m2->status();
   assert(st2 == GC::SS_BRANCH);
   LA::SearchStat stat2 = LA::find_all_solutions<Trivial::Sum>(m2);
   assert(stat2.solutions == stat.solutions);

   const auto b3 = LA::BranchingO::narylookahead;
   const trivial_sum_ptr m3(new Trivial::Sum(2, 0, 1, b3));
   assert(m3->valid());
   assert(m3->size() == 2);
   assert(m3->valid(0));
   assert(m3->valid(1));
   assert(not m3->valid(2));
   [[maybe_unused]] auto const st3 = m3->status();
   assert(st3 == GC::SS_BRANCH);
   {const auto r = LA::la_measure<Trivial::Sum>(m3.get(), 0, 0);
    assert(r.measure == -1.0 and r.status == GC::SS_SOLVED);}
   {const auto r = LA::la_measure<Trivial::Sum>(m3.get(), 0, 1);
    assert(r.measure == -1.0 and r.status == GC::SS_SOLVED);}
   {const auto r = LA::la_measure<Trivial::Sum>(m3.get(), 1, 0);
    assert(r.measure == -1.0 and r.status == GC::SS_SOLVED);}
   {const auto r = LA::la_measure<Trivial::Sum>(m3.get(), 1, 1);
    assert(r.measure == -1.0 and r.status == GC::SS_SOLVED);}
   LA::SearchStat stat3 = LA::find_all_solutions<Trivial::Sum>(m3);
   assert(stat3.solutions == stat.solutions);
  }

  {const auto b = LA::BranchingO::binarysizeminvalmin;
   const trivial_sum_ptr m(new Trivial::Sum(2, 0, 2, b));
   assert(m->valid());
   assert(m->size() == 2);
   assert(m->valid(0));
   assert(m->valid(1));
   assert(not m->valid(2));
   [[maybe_unused]] auto const st = m->status();
   assert(st == GC::SS_BRANCH);
   LA::SearchStat stat = LA::find_all_solutions<Trivial::Sum>(m);
   assert(stat.solutions == 3);

   const auto b2 = LA::BranchingO::narysizeminvalmin;
   const trivial_sum_ptr m2(new Trivial::Sum(2, 0, 2, b2));
   assert(m2->valid());
   assert(m2->size() == 2);
   assert(m2->valid(0));
   assert(m2->valid(1));
   assert(not m2->valid(2));
   [[maybe_unused]] auto const st2 = m2->status();
   assert(st2 == GC::SS_BRANCH);
   LA::SearchStat stat2 = LA::find_all_solutions<Trivial::Sum>(m2);
   assert(stat2.solutions == stat.solutions);
  }

  {const auto b = LA::BranchingO::binarysizeminvalmin;
   const trivial_sum_ptr m(new Trivial::Sum(3, 0, 1, b));
   assert(m->valid());
   assert(m->size() == 3);
   assert(m->valid(0));
   assert(m->valid(1));
   assert(m->valid(2));
   assert(not m->valid(3));
   LA::SearchStat stat = LA::find_all_solutions<Trivial::Sum>(m);
   assert(stat.solutions == 3);
   [[maybe_unused]] auto const st = m->status();
   assert(st == GC::SS_BRANCH);
   {const auto r = LA::la_measure<Trivial::Sum>(m.get(), 0, 0);
    assert(r.measure == 1 and r.status == GC::SS_BRANCH);}
   {const auto r = LA::la_measure<Trivial::Sum>(m.get(), 0, 1);
    assert(r.measure == -1.0 and r.status == GC::SS_SOLVED);}
   {const auto r = LA::la_measure<Trivial::Sum>(m.get(), 1, 0);
    assert(r.measure == 1 and r.status == GC::SS_BRANCH);}
   {const auto r = LA::la_measure<Trivial::Sum>(m.get(), 1, 1);
    assert(r.measure == -1.0 and r.status == GC::SS_SOLVED);}
   {const auto r = LA::la_measure<Trivial::Sum>(m.get(), 2, 0);
    assert(r.measure == -1.0 and r.status == GC::SS_SOLVED);}
   {const auto r = LA::la_measure<Trivial::Sum>(m.get(), 2, 1);
    assert(r.measure == 1 and r.status == GC::SS_BRANCH);}

   const auto b2 = LA::BranchingO::narysizeminvalmin;
   const trivial_sum_ptr m2(new Trivial::Sum(3, 0, 1, b2));
   assert(m2->valid());
   assert(m2->size() == 3);
   assert(m2->valid(0));
   assert(m2->valid(1));
   assert(m2->valid(2));
   assert(not m2->valid(3));
   [[maybe_unused]] auto const st2 = m2->status();
   assert(st2 == GC::SS_BRANCH);
   LA::SearchStat stat2 = LA::find_all_solutions<Trivial::Sum>(m2);
   assert(stat2.solutions == stat.solutions);
  }

  {const auto b = LA::BranchingO::binarysizeminvalmin;
   const trivial_sum_ptr m(new Trivial::Sum(3, 0, 2, b));
   assert(m->valid());
   assert(m->size() == 3);
   assert(m->valid(0));
   assert(m->valid(1));
   assert(m->valid(2));
   assert(not m->valid(3));
   LA::SearchStat stat = LA::find_all_solutions<Trivial::Sum>(m);
   assert(stat.solutions == 6);
   [[maybe_unused]] auto const st = m->status();
   assert(st == GC::SS_BRANCH);
   {const auto r = LA::la_measure<Trivial::Sum>(m.get(), 0, 0);
    assert(r.measure == 2 and r.status == GC::SS_BRANCH);}
   {const auto r = LA::la_measure<Trivial::Sum>(m.get(), 0, 1);
    assert(r.measure == 4 and r.status == GC::SS_BRANCH);}
   {const auto r = LA::la_measure<Trivial::Sum>(m.get(), 0, 2);
    assert(r.measure == -1.0 and r.status == GC::SS_SOLVED);}
   {const auto r = LA::la_measure<Trivial::Sum>(m.get(), 1, 0);
    assert(r.measure == 2 and r.status == GC::SS_BRANCH);}
   {const auto r = LA::la_measure<Trivial::Sum>(m.get(), 1, 1);
    assert(r.measure == 4 and r.status == GC::SS_BRANCH);}
   {const auto r = LA::la_measure<Trivial::Sum>(m.get(), 1, 2);
    assert(r.measure == -1.0 and r.status == GC::SS_SOLVED);}
   {const auto r = LA::la_measure<Trivial::Sum>(m.get(), 2, 0);
    assert(r.measure == -1.0 and r.status == GC::SS_SOLVED);}
   {const auto r = LA::la_measure<Trivial::Sum>(m.get(), 2, 1);
    assert(r.measure == 4 and r.status == GC::SS_BRANCH);}
   {const auto r = LA::la_measure<Trivial::Sum>(m.get(), 2, 2);
    assert(r.measure == 2 and r.status == GC::SS_BRANCH);}

   const auto b2 = LA::BranchingO::narysizeminvalmin;
   const trivial_sum_ptr m2(new Trivial::Sum(3, 0, 2, b2));
   assert(m2->valid());
   assert(m2->size() == 3);
   assert(m2->valid(0));
   assert(m2->valid(1));
   assert(m2->valid(2));
   assert(not m2->valid(3));
   [[maybe_unused]] auto const st2 = m2->status();
   assert(st2 == GC::SS_BRANCH);
   LA::SearchStat stat2 = LA::find_all_solutions<Trivial::Sum>(m2);
   assert(stat2.solutions == stat.solutions);
  }
}

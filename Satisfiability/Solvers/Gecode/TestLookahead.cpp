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

#include "./Examples/Trivial.hpp"

namespace LA = Lookahead;

namespace {

  namespace GC = Gecode;

  typedef std::shared_ptr<Trivial::Sum> trivial_sum_ptr;
  typedef std::shared_ptr<Trivial::OneNodeOneSolution> trivial_onesol_ptr;
  typedef std::shared_ptr<Trivial::OneNodeNoSolution> trivial_nosol_ptr;
  typedef LA::BrTypeO BrTypeO;
  typedef LA::BrSourceO BrSourceO;
  typedef LA::BrMeasureO BrMeasureO;

  const Environment::ProgramInfo proginfo{
        "0.3.9",
        "9.6.2021",
        __FILE__,
        "Oleg Zaikin and Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Solvers/Gecode/TestLookahead.cpp",
        "GPL v3"};

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  {const auto brt = BrTypeO::mind;
   const auto brs = BrSourceO::eq;
   const trivial_nosol_ptr m(new Trivial::OneNodeNoSolution(brt, brs, BrMeasureO::mu0));
   assert(m->valid());
   [[maybe_unused]] auto const st = m->status();
   assert(st == GC::SS_FAILED);
   LA::SearchStat stat = LA::find_all_solutions<Trivial::OneNodeNoSolution>(m);
   assert(stat.valid());
   // In this case Gecode statistics gives wrong number of nodes (0):
   assert(stat.nodes == 1 and stat.engine.node == 0);
   assert(stat.inner_nodes == 0);
   assert(stat.failed_leaves == 1 and stat.engine.fail == stat.failed_leaves);
   assert(stat.solutions == 0);
   assert(stat.br_type == brt);
   assert(stat.br_source == brs);
  }

  {const auto brt = BrTypeO::mind;
   const auto brs = BrSourceO::eq;
   const trivial_onesol_ptr m(new Trivial::OneNodeOneSolution(brt, brs, BrMeasureO::mu0));
   assert(m->valid());
   [[maybe_unused]] auto const st = m->status();
   assert(st == GC::SS_SOLVED);
   LA::SearchStat stat = LA::find_all_solutions<Trivial::OneNodeOneSolution>(m);
   assert(stat.valid());
   assert(stat.nodes == 1 and stat.engine.node == stat.nodes);
   assert(stat.inner_nodes == 0);
   assert(stat.failed_leaves == 0 and stat.engine.fail == stat.failed_leaves);
   assert(stat.solutions == 1);
   assert(stat.br_type == brt);
   assert(stat.br_source == brs);
  }

  {const auto brt = BrTypeO::mind;
   const auto brs = BrSourceO::eq;
   const auto brm = BrMeasureO::mu0;
   const trivial_sum_ptr m(new Trivial::Sum(1, 0, 0, brt, brs, brm));
   assert(m->valid());
   assert(m->size() == 1);
   assert(m->valid(0));
   assert(not m->valid(1));
   [[maybe_unused]] auto const st = m->status();
   assert(st == GC::SS_SOLVED);
   LA::SearchStat stat = LA::find_all_solutions<Trivial::Sum>(m);
   assert(stat.valid());
   assert(stat.nodes == 1 and stat.engine.node == stat.nodes);
   assert(stat.inner_nodes == 0);
   assert(stat.failed_leaves == 0 and stat.engine.fail == stat.failed_leaves);
   assert(stat.solutions == 1);
   assert(stat.br_type == brt);
   assert(stat.br_source == brs);
   assert(stat.br_measure == brm);

   const auto brt2 = BrTypeO::mind;
   const auto brs2 = BrSourceO::v;
   const trivial_sum_ptr m2(new Trivial::Sum(1, 0, 0, brt2, brs2, BrMeasureO::mu0));
   assert(m2->valid());
   assert(m2->size() == 1);
   assert(m2->valid(0));
   assert(not m2->valid(1));
   [[maybe_unused]] auto const st2 = m2->status();
   assert(st2 == GC::SS_SOLVED);
   LA::SearchStat stat2 = LA::find_all_solutions<Trivial::Sum>(m2);
   assert(stat2.valid());
   assert(stat2 == stat);
   assert(stat2.br_type == brt2);
   assert(stat2.br_source == brs2);

   const auto brt3 = BrTypeO::la;
   const auto brs3 = BrSourceO::v;
   const trivial_sum_ptr m3(new Trivial::Sum(1, 0, 0, brt3, brs3, BrMeasureO::mu0));
   assert(m3->valid());
   assert(m3->size() == 1);
   assert(m3->valid(0));
   assert(not m3->valid(1));
   [[maybe_unused]] auto const st3 = m3->status();
   assert(st3 == GC::SS_SOLVED);
   LA::SearchStat stat3 = LA::find_all_solutions<Trivial::Sum>(m3);
   assert(stat3.valid());
   assert(stat3 == stat);
   assert(stat3.br_type == brt3);
   assert(stat3.br_source == brs3);
  }

  {const auto brt = BrTypeO::mind;
   const auto brs = BrSourceO::eq;
   const trivial_sum_ptr m(new Trivial::Sum(2, 0, 1, brt, brs, BrMeasureO::mu0));
   assert(m->valid());
   assert(m->size() == 2);
   assert(m->valid(0));
   assert(m->valid(1));
   assert(not m->valid(2));
   [[maybe_unused]] auto const st = m->status();
   assert(st == GC::SS_BRANCH);
   LA::SearchStat stat = LA::find_all_solutions<Trivial::Sum>(m);
   assert(stat.valid());
   assert(stat.engine.node == 3);
   assert(stat.engine.fail == 0);
   assert(stat.solutions == 2);
   assert(stat.br_type == brt);
   assert(stat.br_source == brs);

   const auto brt2 = BrTypeO::mind;
   const auto brs2 = BrSourceO::v;
   const trivial_sum_ptr m2(new Trivial::Sum(2, 0, 1, brt2, brs2, BrMeasureO::mu0));
   assert(m2->valid());
   assert(m2->size() == 2);
   assert(m2->valid(0));
   assert(m2->valid(1));
   assert(not m2->valid(2));
   [[maybe_unused]] auto const st2 = m2->status();
   assert(st2 == GC::SS_BRANCH);
   LA::SearchStat stat2 = LA::find_all_solutions<Trivial::Sum>(m2);
   assert(stat2.valid());
   assert(stat2.nodes == 3);
   assert(stat2.inner_nodes == 1);
   assert(stat2.failed_leaves == 0);
   assert(stat2.solutions == stat.solutions);
   assert(stat2.br_type == brt2);
   assert(stat2.br_source == brs2);

   const auto brt3 = BrTypeO::la;
   const auto brs3 = BrSourceO::v;
   const trivial_sum_ptr m3(new Trivial::Sum(2, 0, 1, brt3, brs3, BrMeasureO::mu0));
   assert(m3->valid());
   assert(m3->size() == 2);
   assert(m3->valid(0));
   assert(m3->valid(1));
   assert(not m3->valid(2));
   [[maybe_unused]] auto const st3 = m3->status();
   assert(st3 == GC::SS_BRANCH);
   {const auto c = LA::subproblem<Trivial::Sum>(m3.get(), 0, 0);
    assert(c.get()->status() == GC::SS_SOLVED);}
   {const auto c = LA::subproblem<Trivial::Sum>(m3.get(), 0, 1);
    assert(c.get()->status() == GC::SS_SOLVED);}
   {const auto c = LA::subproblem<Trivial::Sum>(m3.get(), 1, 0);
    assert(c.get()->status() == GC::SS_SOLVED);}
   {const auto c = LA::subproblem<Trivial::Sum>(m3.get(), 1, 1);
    assert(c.get()->status() == GC::SS_SOLVED);}
   LA::SearchStat stat3 = LA::find_all_solutions<Trivial::Sum>(m3);
   assert(stat3.valid());
   assert(stat3 == stat2);
   assert(stat3.br_type == brt3);
   assert(stat3.br_source == brs3);
  }

  {const auto brt = BrTypeO::mind;
   const auto brs = BrSourceO::eq;
   const trivial_sum_ptr m(new Trivial::Sum(2, 0, 2, brt, brs, BrMeasureO::mu0));
   assert(m->valid());
   assert(m->size() == 2);
   assert(m->valid(0));
   assert(m->valid(1));
   assert(not m->valid(2));
   [[maybe_unused]] auto const st = m->status();
   assert(st == GC::SS_BRANCH);
   LA::SearchStat stat = LA::find_all_solutions<Trivial::Sum>(m);
   assert(stat.valid());
   assert(stat.engine.node == 5);
   assert(stat.engine.fail == 0);
   assert(stat.solutions == 3);
   assert(stat.br_type == brt);
   assert(stat.br_source == brs);

   const auto brt2 = BrTypeO::mind;
   const auto brs2 = BrSourceO::v;
   const trivial_sum_ptr m2(new Trivial::Sum(2, 0, 2, brt2, brs2, BrMeasureO::mu0));
   assert(m2->valid());
   assert(m2->size() == 2);
   assert(m2->valid(0));
   assert(m2->valid(1));
   assert(not m2->valid(2));
   [[maybe_unused]] auto const st2 = m2->status();
   assert(st2 == GC::SS_BRANCH);
   LA::SearchStat stat2 = LA::find_all_solutions<Trivial::Sum>(m2);
   assert(stat2.valid());
   assert(stat2.nodes == 4);
   assert(stat2.inner_nodes == 1);
   assert(stat2.failed_leaves == 0);
   assert(stat2.solutions == stat.solutions);
   assert(stat2.br_type == brt2);
   assert(stat2.br_source == brs2);

   const auto brt3 = BrTypeO::la;
   const auto brs3 = BrSourceO::v;
   const trivial_sum_ptr m3(new Trivial::Sum(2, 0, 2, brt3, brs3, BrMeasureO::mu0));
   assert(m3->valid());
   assert(m3->size() == 2);
   assert(m3->valid(0));
   assert(m3->valid(1));
   assert(not m3->valid(2));
   [[maybe_unused]] auto const st3 = m3->status();
   assert(st3 == GC::SS_BRANCH);
   {const auto c = LA::subproblem<Trivial::Sum>(m3.get(), 0, 0);
    assert(c.get()->status() == GC::SS_SOLVED);}
   {const auto c = LA::subproblem<Trivial::Sum>(m3.get(), 0, 1);
    assert(c.get()->status() == GC::SS_SOLVED);}
   {const auto c = LA::subproblem<Trivial::Sum>(m3.get(), 0, 2);
    assert(c.get()->status() == GC::SS_SOLVED);}
   {const auto c = LA::subproblem<Trivial::Sum>(m3.get(), 1, 0);
    assert(c.get()->status() == GC::SS_SOLVED);}
   {const auto c = LA::subproblem<Trivial::Sum>(m3.get(), 1, 1);
    assert(c.get()->status() == GC::SS_SOLVED);}
   {const auto c = LA::subproblem<Trivial::Sum>(m3.get(), 1, 2);
    assert(c.get()->status() == GC::SS_SOLVED);}
   LA::SearchStat stat3 = LA::find_all_solutions<Trivial::Sum>(m3);
   assert(stat3.valid());
   assert(stat3 == stat2);
   assert(stat3.br_type == brt3);
   assert(stat3.br_source == brs3);
  }

  {const auto brt = BrTypeO::mind;
   const auto brs = BrSourceO::eq;
   const trivial_sum_ptr m(new Trivial::Sum(3, 0, 1, brt, brs, BrMeasureO::mu0));
   assert(m->valid());
   assert(m->size() == 3);
   assert(m->valid(0));
   assert(m->valid(1));
   assert(m->valid(2));
   assert(not m->valid(3));
   [[maybe_unused]] auto const st = m->status();
   assert(st == GC::SS_BRANCH);
   LA::SearchStat stat = LA::find_all_solutions<Trivial::Sum>(m);
   assert(stat.valid());
   assert(stat.engine.node == 5);
   assert(stat.engine.fail == 0);
   assert(stat.solutions == 3);
   assert(stat.br_type == brt);
   assert(stat.br_source == brs);

   const auto brt2 = BrTypeO::mind;
   const auto brs2 = BrSourceO::v;
   const trivial_sum_ptr m2(new Trivial::Sum(3, 0, 1, brt2, brs2, BrMeasureO::mu0));
   assert(m2->valid());
   assert(m2->size() == 3);
   assert(m2->valid(0));
   assert(m2->valid(1));
   assert(m2->valid(2));
   assert(not m2->valid(3));
   [[maybe_unused]] auto const st2 = m2->status();
   assert(st2 == GC::SS_BRANCH);
   LA::SearchStat stat2 = LA::find_all_solutions<Trivial::Sum>(m2);
   assert(stat2.valid());
   assert(stat2.nodes == 5);
   assert(stat2.inner_nodes == 2);
   assert(stat2.failed_leaves == 0);
   assert(stat2.solutions == stat.solutions);
   assert(stat2.br_type == brt2);
   assert(stat2.br_source == brs2);

   const auto brt3 = BrTypeO::la;
   const auto brs3 = BrSourceO::v;
   const trivial_sum_ptr m3(new Trivial::Sum(3, 0, 1, brt3, brs3, BrMeasureO::mu0));
   assert(m3->valid());
   assert(m3->size() == 3);
   assert(m3->valid(0));
   assert(m3->valid(1));
   assert(m3->valid(2));
   assert(not m3->valid(3));
   [[maybe_unused]] auto const st3 = m3->status();
   assert(st3 == GC::SS_BRANCH);
   {const auto m3m = m3.get();
    const auto c = LA::subproblem<Trivial::Sum>(m3m, 0, 0);
    const auto cm = c.get();
    assert(cm->status() == GC::SS_BRANCH);
    assert(LA::mu0(m3m->at()) == 3);
    assert(LA::mu0(cm->at()) == 2);
    assert(LA::mu1(m3m->at()) == 3);
    assert(LA::mu1(cm->at()) == 2);}
   {const auto c = LA::subproblem<Trivial::Sum>(m3.get(), 0, 1);
    assert(c.get()->status() == GC::SS_SOLVED);}
   {const auto m3m = m3.get();
    const auto c = LA::subproblem<Trivial::Sum>(m3m, 1, 0);
    const auto cm = c.get();
    assert(cm->status() == GC::SS_BRANCH);
    assert(LA::mu0(m3m->at()) == 3);
    assert(LA::mu0(cm->at()) == 2);
    assert(LA::mu1(m3m->at()) == 3);
    assert(LA::mu1(cm->at()) == 2);}
   {const auto c = LA::subproblem<Trivial::Sum>(m3.get(), 1, 1);
    assert(c.get()->status() == GC::SS_SOLVED);}
   {const auto c = LA::subproblem<Trivial::Sum>(m3.get(), 2, 0);
    assert(c.get()->status() == GC::SS_SOLVED);}
   {const auto m3m = m3.get();
    const auto c = LA::subproblem<Trivial::Sum>(m3m, 2, 1);
    const auto cm = c.get();
    assert(cm->status() == GC::SS_BRANCH);
    assert(LA::mu0(m3m->at()) == 3);
    assert(LA::mu0(cm->at()) == 2);
    assert(LA::mu1(m3m->at()) == 3);
    assert(LA::mu1(cm->at()) == 2);}
   LA::SearchStat stat3 = LA::find_all_solutions<Trivial::Sum>(m3);
   assert(stat3.valid());
   assert(stat3 == stat2);
   assert(stat3.br_type == brt3);
   assert(stat3.br_source == brs3);
  }

  {const auto brt = BrTypeO::mind;
   const auto brs = BrSourceO::eq;
   const trivial_sum_ptr m(new Trivial::Sum(3, 0, 2, brt, brs, BrMeasureO::mu0));
   assert(m->valid());
   assert(m->size() == 3);
   assert(m->valid(0));
   assert(m->valid(1));
   assert(m->valid(2));
   assert(not m->valid(3));
   [[maybe_unused]] auto const st = m->status();
   assert(st == GC::SS_BRANCH);
   LA::SearchStat stat = LA::find_all_solutions<Trivial::Sum>(m);
   assert(stat.valid());
   assert(stat.engine.node == 11);
   assert(stat.engine.fail == 0);
   assert(stat.solutions == 6);
   assert(stat.br_type == brt);
   assert(stat.br_source == brs);

   const auto brt2 = BrTypeO::mind;
   const auto brs2 = BrSourceO::v;
   const trivial_sum_ptr m2(new Trivial::Sum(3, 0, 2, brt2, brs2, BrMeasureO::mu0));
   assert(m2->valid());
   assert(m2->size() == 3);
   assert(m2->valid(0));
   assert(m2->valid(1));
   assert(m2->valid(2));
   assert(not m2->valid(3));
   [[maybe_unused]] auto const st2 = m2->status();
   assert(st2 == GC::SS_BRANCH);
   LA::SearchStat stat2 = LA::find_all_solutions<Trivial::Sum>(m2);
   assert(stat2.valid());
   assert(stat2.nodes == 9);
   assert(stat2.inner_nodes == 3);
   assert(stat2.failed_leaves == 0);
   assert(stat2.solutions == stat.solutions);
   assert(stat2.br_type == brt2);
   assert(stat2.br_source == brs2);

   const auto brt3 = BrTypeO::la;
   const auto brs3 = BrSourceO::v;
   const trivial_sum_ptr m3(new Trivial::Sum(3, 0, 2, brt3, brs3, BrMeasureO::mu0));
   assert(m3->valid());
   assert(m3->size() == 3);
   assert(m3->valid(0));
   assert(m3->valid(1));
   assert(m3->valid(2));
   assert(not m3->valid(3));
   [[maybe_unused]] auto const st3 = m3->status();
   assert(st3 == GC::SS_BRANCH);
   {const auto m3m = m3.get();
    const auto c = LA::subproblem<Trivial::Sum>(m3m, 0, 0);
    const auto cm = c.get();
    assert(cm->status() == GC::SS_BRANCH);
    assert(LA::mu0(m3m->at()) == 6);
    assert(LA::mu0(cm->at()) == 4);
    assert(LA::mu1(m3m->at()) == 3*FloatingPoint::log2(3));
    assert(LA::mu1(cm->at()) == 2*FloatingPoint::log2(3));}
   {const auto m3m = m3.get();
    const auto c = LA::subproblem<Trivial::Sum>(m3m, 0, 1);
    const auto cm = c.get();
    assert(cm->status() == GC::SS_BRANCH);
    assert(LA::mu0(m3m->at()) == 6);
    assert(LA::mu0(cm->at()) == 2);
    assert(LA::mu1(m3m->at()) == 3*FloatingPoint::log2(3));
    assert(LA::mu1(cm->at()) == 2);}
   {const auto c = LA::subproblem<Trivial::Sum>(m3.get(), 0, 2);
    assert(c.get()->status() == GC::SS_SOLVED);}
   {const auto m3m = m3.get();
    const auto c = LA::subproblem<Trivial::Sum>(m3m, 1, 0);
    const auto cm = c.get();
    assert(cm->status() == GC::SS_BRANCH);
    assert(LA::mu0(m3m->at()) == 6);
    assert(LA::mu0(cm->at()) == 4);
    assert(LA::mu1(m3m->at()) == 3*FloatingPoint::log2(3));
    assert(LA::mu1(cm->at()) == 2*FloatingPoint::log2(3));}
   {const auto m3m = m3.get();
    const auto c = LA::subproblem<Trivial::Sum>(m3m, 1, 1);
    const auto cm = c.get();
    assert(cm->status() == GC::SS_BRANCH);
    assert(LA::mu0(m3m->at()) == 6);
    assert(LA::mu0(cm->at()) == 2);
    assert(LA::mu1(m3m->at()) == 3*FloatingPoint::log2(3));
    assert(LA::mu1(cm->at()) == 2);}
   {const auto c = LA::subproblem<Trivial::Sum>(m3.get(), 1, 2);
    assert(c.get()->status() == GC::SS_SOLVED);}
   {const auto c = LA::subproblem<Trivial::Sum>(m3.get(), 2, 0);
    assert(c.get()->status() == GC::SS_SOLVED);}
   {const auto m3m = m3.get();
    const auto c = LA::subproblem<Trivial::Sum>(m3m, 2, 1);
    const auto cm = c.get();
    assert(cm->status() == GC::SS_BRANCH);
    assert(LA::mu0(m3m->at()) == 6);
    assert(LA::mu0(cm->at()) == 2);
    assert(LA::mu1(m3m->at()) == 3*FloatingPoint::log2(3));
    assert(LA::mu1(cm->at()) == 2);}
   {const auto m3m = m3.get();
    const auto c = LA::subproblem<Trivial::Sum>(m3m, 2, 2);
    const auto cm = c.get();
    assert(cm->status() == GC::SS_BRANCH);
    assert(LA::mu0(m3m->at()) == 6);
    assert(LA::mu0(cm->at()) == 4);
    assert(LA::mu1(m3m->at()) == 3*FloatingPoint::log2(3));
    assert(LA::mu1(cm->at()) == 2*FloatingPoint::log2(3));}
   LA::SearchStat stat3 = LA::find_all_solutions<Trivial::Sum>(m3);
   assert(stat3.valid());
   assert(stat3 == stat2);
   assert(stat3.br_type == brt3);
   assert(stat3.br_source == brs3);
  }
}

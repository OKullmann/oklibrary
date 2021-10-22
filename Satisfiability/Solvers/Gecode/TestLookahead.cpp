// Oleg Zaikin, 6.4.2020 (Irkutsk)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

TODOS:

1. Test the one-solution mode.

2. Test the usage of lookahed with mu1.

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
  typedef LA::BrTypeO BrTpO;
  typedef LA::BrSourceO BrSrcO;
  typedef LA::BrMeasureO BrMsrO;
  typedef LA::BrSolutionO BrSltnO;
  typedef LA::option_t option_t;
  typedef LA::Branching Branching;
  typedef LA::BrStatus BrStatus;

  const Environment::ProgramInfo proginfo{
        "0.4.9",
        "22.10.2021",
        __FILE__,
        "Oleg Zaikin and Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Solvers/Gecode/TestLookahead.cpp",
        "GPL v3"};

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  {Branching br;
   assert(br.valid());}
  {Branching br(BrStatus::unsat);
   assert(br.valid());}
  {Branching br(BrStatus::unsat, 0);
   assert(br.valid());}
  {Branching br(BrStatus::unsat, -1);
   assert(not br.valid());}
  {Branching br(BrStatus::unsat, 0, {});
   assert(br.valid());}
  {Branching br(BrStatus::unsat, 0, {0});
   assert(not br.valid());}
  {Branching br(BrStatus::unsat, 0, {}, {false});
   assert(not br.valid());}
  {Branching br(BrStatus::unsat, 0, {0}, {false});
   assert(not br.valid());}
  {Branching br(BrStatus::branching);
   assert(not br.valid());}
  {Branching br(BrStatus::branching, 0);
   assert(not br.valid());}
  {Branching br(BrStatus::branching, -1);
   assert(not br.valid());}
  {Branching br(BrStatus::branching, 0, {});
   assert(not br.valid());}
  {Branching br(BrStatus::branching, 0, {0});
   assert(br.valid());}
  {Branching br(BrStatus::branching, 0, {0}, {}, {1});
   assert(br.valid());}
  {Branching br(BrStatus::branching, 0, {0}, {}, {1}, {1});
   assert(not br.valid());}
  {Branching br(BrStatus::branching, 0, {0}, {}, {1,1});
   assert(not br.valid());}
  {Branching br(BrStatus::branching, 0, {}, {false});
   assert(not br.valid());}
  {Branching br(BrStatus::branching, 0, {0}, {false});
   assert(br.valid());}
  {Branching br(BrStatus::branching, 0, {0}, {true});
   assert(br.valid());}
  {Branching br(BrStatus::branching, 0, {0}, {false, true});
   assert(br.valid());}
  {Branching br(BrStatus::branching, 0, {0}, {false, false});
   assert(not br.valid());}
  {Branching br(BrStatus::branching, 0, {0}, {true, true});
   assert(not br.valid());}
  {Branching br(BrStatus::branching, 0, {0}, {false, true, false});
   assert(not br.valid());}
  {Branching br(BrStatus::branching, 0, {0}, {false}, {1});
   assert(br.valid());}
  {Branching br(BrStatus::branching, 0, {0}, {false}, {1,1});
   assert(not br.valid());}
  {Branching br(BrStatus::branching, 0, {0}, {false}, {}, {1});
   assert(br.valid());}
  {Branching br(BrStatus::branching, 0, {0}, {false}, {}, {1,1});
   assert(not br.valid());}
  {Branching br(BrStatus::branching, 0, {0}, {false}, {1}, {1});
   assert(br.valid());}
  {Branching br(BrStatus::sat);
   assert(not br.valid());}
  {Branching br(BrStatus::sat, 0);
   assert(not br.valid());}
  {Branching br(BrStatus::sat, -1);
   assert(not br.valid());}
  {Branching br(BrStatus::sat, 0, {});
   assert(not br.valid());}
  {Branching br(BrStatus::sat, 0, {0});
   assert(br.valid());}
  {Branching br(BrStatus::sat, 0, {0}, {}, {1});
   assert(br.valid());}
  {Branching br(BrStatus::sat, 0, {0}, {}, {1}, {1});
   assert(not br.valid());}
  {Branching br(BrStatus::sat, 0, {0}, {}, {1,1});
   assert(not br.valid());}
  {Branching br(BrStatus::sat, 0, {}, {false});
   assert(not br.valid());}
  {Branching br(BrStatus::sat, 0, {0}, {false});
   assert(br.valid());}
  {Branching br(BrStatus::sat, 0, {0}, {true});
   assert(br.valid());}
  {Branching br(BrStatus::sat, 0, {0}, {false, true});
   assert(br.valid());}
  {Branching br(BrStatus::sat, 0, {0}, {false, false});
   assert(not br.valid());}
  {Branching br(BrStatus::sat, 0, {0}, {true, true});
   assert(not br.valid());}
  {Branching br(BrStatus::sat, 0, {0}, {false, true, false});
   assert(not br.valid());}
  {Branching br(BrStatus::sat, 0, {0}, {false}, {1});
   assert(br.valid());}
  {Branching br(BrStatus::sat, 0, {0}, {false}, {1,1});
   assert(not br.valid());}
  {Branching br(BrStatus::sat, 0, {0}, {false}, {}, {1});
   assert(br.valid());}
  {Branching br(BrStatus::sat, 0, {0}, {false}, {}, {1,1});
   assert(not br.valid());}
  {Branching br(BrStatus::sat, 0, {0}, {false}, {1}, {1});
   assert(br.valid());}

  {Branching br(BrStatus::sat);
   assert(not br.valid());}

  {const option_t options = {BrTpO::mind, BrSrcO::eq, BrMsrO::mu0, BrSltnO::all};
   const trivial_nosol_ptr m(new Trivial::OneNodeNoSolution(options));
   assert(m->valid());
   [[maybe_unused]] auto const st = m->status();
   assert(st == GC::SS_FAILED);
   LA::SearchStat stat = LA::solve<Trivial::OneNodeNoSolution>(m);
   assert(stat.valid());
   // In this case Gecode statistics gives wrong number of nodes (0):
   assert(stat.nodes == 1 and stat.gecode_stat.node == 0);
   assert(stat.inner_nodes == 0);
   assert(stat.unsat_leaves == 1 and stat.gecode_stat.fail == stat.unsat_leaves);
   assert(stat.solutions == 0);
  }

  {const option_t options = {BrTpO::mind, BrSrcO::eq, BrMsrO::mu0, BrSltnO::all};
   const trivial_onesol_ptr m(new Trivial::OneNodeOneSolution(options));
   assert(m->valid());
   [[maybe_unused]] auto const st = m->status();
   assert(st == GC::SS_SOLVED);
   LA::SearchStat stat = LA::solve<Trivial::OneNodeOneSolution>(m);
   assert(stat.valid());
   assert(stat.nodes == 1 and stat.gecode_stat.node == stat.nodes);
   assert(stat.inner_nodes == 0);
   assert(stat.unsat_leaves == 0 and stat.gecode_stat.fail == stat.unsat_leaves);
   assert(stat.solutions == 1);
  }

  {const option_t options = {BrTpO::mind, BrSrcO::eq, BrMsrO::mu0, BrSltnO::all};
   const trivial_sum_ptr m(new Trivial::Sum(1, 0, 0, options));
   assert(m->valid());
   assert(m->size() == 1);
   assert(m->valid(0));
   assert(not m->valid(1));
   [[maybe_unused]] auto const st = m->status();
   assert(st == GC::SS_SOLVED);
   LA::SearchStat stat = LA::solve<Trivial::Sum>(m);
   assert(stat.valid());
   assert(stat.nodes == 1 and stat.gecode_stat.node == stat.nodes);
   assert(stat.inner_nodes == 0);
   assert(stat.unsat_leaves == 0 and stat.gecode_stat.fail == stat.unsat_leaves);
   assert(stat.solutions == 1);

   const option_t options2 = {BrTpO::mind, BrSrcO::val, BrMsrO::mu0, BrSltnO::all};
   const trivial_sum_ptr m2(new Trivial::Sum(1, 0, 0, options2));
   assert(m2->valid());
   assert(m2->size() == 1);
   assert(m2->valid(0));
   assert(not m2->valid(1));
   [[maybe_unused]] auto const st2 = m2->status();
   assert(st2 == GC::SS_SOLVED);
   LA::SearchStat stat2 = LA::solve<Trivial::Sum>(m2);
   assert(stat2.valid());

   const option_t options3 = {BrTpO::la, BrSrcO::val, BrMsrO::mu0, BrSltnO::all};
   const trivial_sum_ptr m3(new Trivial::Sum(1, 0, 0, options3));
   assert(m3->valid());
   assert(m3->size() == 1);
   assert(m3->valid(0));
   assert(not m3->valid(1));
   [[maybe_unused]] auto const st3 = m3->status();
   assert(st3 == GC::SS_SOLVED);
   LA::SearchStat stat3 = LA::solve<Trivial::Sum>(m3);
   assert(stat3.valid());
  }

  {const option_t options = {BrTpO::mind, BrSrcO::eq, BrMsrO::mu0, BrSltnO::all};
   const trivial_sum_ptr m(new Trivial::Sum(2, 0, 1, options));
   assert(m->valid());
   assert(m->size() == 2);
   assert(m->valid(0));
   assert(m->valid(1));
   assert(not m->valid(2));
   [[maybe_unused]] auto const st = m->status();
   assert(st == GC::SS_BRANCH);
   LA::SearchStat stat = LA::solve<Trivial::Sum>(m);
   assert(stat.valid());
   assert(stat.gecode_stat.node == 3);
   assert(stat.gecode_stat.fail == 0);
   assert(stat.solutions == 2);

   const option_t options2 = {BrTpO::mind, BrSrcO::val, BrMsrO::mu0, BrSltnO::all};
   const trivial_sum_ptr m2(new Trivial::Sum(2, 0, 1, options2));
   assert(m2->valid());
   assert(m2->size() == 2);
   assert(m2->valid(0));
   assert(m2->valid(1));
   assert(not m2->valid(2));
   [[maybe_unused]] auto const st2 = m2->status();
   assert(st2 == GC::SS_BRANCH);
   LA::SearchStat stat2 = LA::solve<Trivial::Sum>(m2);
   assert(stat2.valid());
   assert(stat2.nodes == 3);
   assert(stat2.inner_nodes == 1);
   assert(stat2.unsat_leaves == 0);
   assert(stat2.solutions == stat.solutions);

   const option_t options3 = {BrTpO::la, BrSrcO::val, BrMsrO::mu0, BrSltnO::all};
   const trivial_sum_ptr m3(new Trivial::Sum(2, 0, 1, options3));
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
   LA::SearchStat stat3 = LA::solve<Trivial::Sum>(m3);
   assert(stat3.valid());
  }

  {const option_t options = {BrTpO::mind, BrSrcO::eq, BrMsrO::mu0, BrSltnO::all};
   const trivial_sum_ptr m(new Trivial::Sum(2, 0, 2, options));
   assert(m->valid());
   assert(m->size() == 2);
   assert(m->valid(0));
   assert(m->valid(1));
   assert(not m->valid(2));
   [[maybe_unused]] auto const st = m->status();
   assert(st == GC::SS_BRANCH);
   LA::SearchStat stat = LA::solve<Trivial::Sum>(m);
   assert(stat.valid());
   assert(stat.gecode_stat.node == 5);
   assert(stat.gecode_stat.fail == 0);
   assert(stat.solutions == 3);

   const option_t options2 = {BrTpO::mind, BrSrcO::val, BrMsrO::mu0, BrSltnO::all};
   const trivial_sum_ptr m2(new Trivial::Sum(2, 0, 2, options2));
   assert(m2->valid());
   assert(m2->size() == 2);
   assert(m2->valid(0));
   assert(m2->valid(1));
   assert(not m2->valid(2));
   [[maybe_unused]] auto const st2 = m2->status();
   assert(st2 == GC::SS_BRANCH);
   LA::SearchStat stat2 = LA::solve<Trivial::Sum>(m2);
   assert(stat2.valid());
   assert(stat2.nodes == 4);
   assert(stat2.inner_nodes == 1);
   assert(stat2.unsat_leaves == 0);
   assert(stat2.solutions == stat.solutions);

   const option_t options3 = {BrTpO::la, BrSrcO::val, BrMsrO::mu0, BrSltnO::all};
   const trivial_sum_ptr m3(new Trivial::Sum(2, 0, 2, options3));
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
   LA::SearchStat stat3 = LA::solve<Trivial::Sum>(m3);
   assert(stat3.valid());
  }

  {const option_t options = {BrTpO::mind, BrSrcO::eq, BrMsrO::mu0, BrSltnO::all};
   const trivial_sum_ptr m(new Trivial::Sum(3, 0, 1, options));
   assert(m->valid());
   assert(m->size() == 3);
   assert(m->valid(0));
   assert(m->valid(1));
   assert(m->valid(2));
   assert(not m->valid(3));
   [[maybe_unused]] auto const st = m->status();
   assert(st == GC::SS_BRANCH);
   LA::SearchStat stat = LA::solve<Trivial::Sum>(m);
   assert(stat.valid());
   assert(stat.gecode_stat.node == 5);
   assert(stat.gecode_stat.fail == 0);
   assert(stat.solutions == 3);

   const option_t options2 = {BrTpO::mind, BrSrcO::val, BrMsrO::mu0, BrSltnO::all};
   const trivial_sum_ptr m2(new Trivial::Sum(3, 0, 1, options2));
   assert(m2->valid());
   assert(m2->size() == 3);
   assert(m2->valid(0));
   assert(m2->valid(1));
   assert(m2->valid(2));
   assert(not m2->valid(3));
   [[maybe_unused]] auto const st2 = m2->status();
   assert(st2 == GC::SS_BRANCH);
   LA::SearchStat stat2 = LA::solve<Trivial::Sum>(m2);
   assert(stat2.valid());
   assert(stat2.nodes == 5);
   assert(stat2.inner_nodes == 2);
   assert(stat2.unsat_leaves == 0);
   assert(stat2.solutions == stat.solutions);

   const option_t options3 = {BrTpO::la, BrSrcO::val, BrMsrO::mu0, BrSltnO::all};
   const trivial_sum_ptr m3(new Trivial::Sum(3, 0, 1, options3));
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
   LA::SearchStat stat3 = LA::solve<Trivial::Sum>(m3);
   assert(stat3.valid());
  }

  {const option_t options = {BrTpO::mind, BrSrcO::eq, BrMsrO::mu0, BrSltnO::all};
   const trivial_sum_ptr m(new Trivial::Sum(3, 0, 2, options));
   assert(m->valid());
   assert(m->size() == 3);
   assert(m->valid(0));
   assert(m->valid(1));
   assert(m->valid(2));
   assert(not m->valid(3));
   [[maybe_unused]] auto const st = m->status();
   assert(st == GC::SS_BRANCH);
   LA::SearchStat stat = LA::solve<Trivial::Sum>(m);
   assert(stat.valid());
   assert(stat.gecode_stat.node == 11);
   assert(stat.gecode_stat.fail == 0);
   assert(stat.solutions == 6);

   const option_t options2 = {BrTpO::mind, BrSrcO::val, BrMsrO::mu0, BrSltnO::all};
   const trivial_sum_ptr m2(new Trivial::Sum(3, 0, 2, options2));
   assert(m2->valid());
   assert(m2->size() == 3);
   assert(m2->valid(0));
   assert(m2->valid(1));
   assert(m2->valid(2));
   assert(not m2->valid(3));
   [[maybe_unused]] auto const st2 = m2->status();
   assert(st2 == GC::SS_BRANCH);
   LA::SearchStat stat2 = LA::solve<Trivial::Sum>(m2);
   assert(stat2.valid());
   assert(stat2.nodes == 9);
   assert(stat2.inner_nodes == 3);
   assert(stat2.unsat_leaves == 0);
   assert(stat2.solutions == stat.solutions);

   const option_t options3 = {BrTpO::la, BrSrcO::val, BrMsrO::mu0, BrSltnO::all};
   const trivial_sum_ptr m3(new Trivial::Sum(3, 0, 2, options3));
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
   LA::SearchStat stat3 = LA::solve<Trivial::Sum>(m3);
   assert(stat3.valid());
  }
}

// Oleg Zaikin, 29.3.2022 (Swansea)
/* Copyright 2022, 2023, 2024 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

Testing of look-ahead reduction for the Gecode library.

BUG:

TODOS:

1. DONE (A partially filled Latin square of order 4 fits this condition)
   Test case with 3 rounds of lookahead reduction.

*/

#include <iostream>
#include <sstream>
#include <string>
#include <memory>

#include <cassert>

#include <gecode/int.hh>
#include <gecode/search.hh>

#include <ProgramOptions/Environment.hpp>

#include "LookaheadReduction.hpp"
#include "Options.hpp"
#include "Solvers.hpp"
#include "GcVariables.hpp"
#include "Conditions.hpp"
#include "Cases.hpp"
#include "BasicLatinSquares.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.8.3",
        "7.2.2024",
        __FILE__,
        "Oleg Zaikin and Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Solvers/Gecode/MOLS/TestLookaheadReduction.cpp",
        "GPL v3"};

  using namespace LookaheadReduction;
  using namespace Options;
  using namespace Solvers;
  using namespace GcVariables;
  using namespace BasicLatinSquares;

  namespace ET = Environment;
  namespace CS = Cases;
  namespace CD = Conditions;
  namespace GC = Gecode;
  namespace LR = LookaheadReduction;

  typedef Options::RT RT;
  using listsol_t = Solvers::listsol_t;
  using size_t = CD::size_t;

  template <class X>
  constexpr bool eqp(const X& lhs, const X& rhs) noexcept {
    return lhs == rhs;
  }

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  {const CS::Square A(2);
    const std::unique_ptr<CS::GenericMolsNB> m = A.space();
    const auto ch = child_node<CS::GenericMolsNB>(m.get(), 0, 0);
    assert(eqp(values(ch->V), {{0}, {0,1}, {0,1}, {0,1}}));
    const auto st = ch->status();
    assert(st == GC::SS_BRANCH);
  }

  {const CS::LaSq A(2);
    const std::unique_ptr<CS::GenericMolsNB> m = A.space();
    const auto ch = child_node<CS::GenericMolsNB>(m.get(), 0, 0);
    assert(eqp(values(ch->V), {{0}, {0,1}, {0,1}, {0,1}}));
    const auto st = ch->status();
    assert(st == GC::SS_SOLVED);
  }

  {const CS::Square A(3);
    const std::unique_ptr<CS::GenericMolsNB> m = A.space();
    const auto ch = child_node<CS::GenericMolsNB>(m.get(), 0, 0);
    assert(eqp(values(ch->V), {{0}, {0,1,2}, {0,1,2}, {0,1,2}, {0,1,2},
      {0,1,2}, {0,1,2}, {0,1,2}, {0,1,2}}));
    const auto st = ch->status();
    assert(st == GC::SS_BRANCH);
  }

  {const CS::Square A(3, "A\n0 * *\n* * *\n* * *\n");
    const std::unique_ptr<CS::GenericMolsNB> m = A.space();
    const auto ch = child_node<CS::GenericMolsNB>(m.get(), 0, 0);
    assert(eqp(values(ch->V), {{0}, {0,1,2}, {0,1,2}, {0,1,2}, {0,1,2},
      {0,1,2}, {0,1,2}, {0,1,2}, {0,1,2}}));
    const auto st = ch->status();
    assert(st == GC::SS_BRANCH);
  }

  {const CS::Square A(3, "A\n* * *\n* 1 *\n* * *\n");
   const std::unique_ptr<CS::GenericMolsNB> m = A.space();
   const auto ch = child_node<CS::GenericMolsNB>(m.get(), 0, 0);
   assert(eqp(values(ch->V), {{0}, {0,1,2}, {0,1,2}, {0,1,2}, {1},
      {0,1,2}, {0,1,2}, {0,1,2}, {0,1,2}}));
   const auto st = ch->status();
   assert(st == GC::SS_BRANCH);
  }

  {const CS::Square A(2);
   const std::unique_ptr<CS::GenericMolsNB> m = A.space();
   ReductionStatistics stats0(m->V, 0);
   assert(probe(m.get(), 0, 0, stats0, false) == GC::SS_BRANCH);
   assert(probe(m.get(), 0, 1, stats0, false) == GC::SS_BRANCH);
   assert(probe(m.get(), 1, 0, stats0, false) == GC::SS_BRANCH);
   assert(probe(m.get(), 1, 1, stats0, false) == GC::SS_BRANCH);
  }
  {const CS::Square A(2);
   const std::unique_ptr<CS::GenericMolsNB> m = A.space();
   ReductionStatistics stats0(m->V, 0);
   pruning_table_t PT;
   assert(probe(m.get(), 0, 0, PT, stats0, false) == GC::SS_BRANCH);
   assert(eqp(PT, {{0,0}}));
   assert(probe(m.get(), 0, 1, PT, stats0, false) == GC::SS_BRANCH);
   assert(eqp(PT, {{0,0}, {0,1}}));
   assert(probe(m.get(), 1, 0, PT, stats0, false) == GC::SS_BRANCH);
   assert(eqp(PT, {{0,0}, {0,1}, {1,0}}));
   assert(probe(m.get(), 1, 1, PT, stats0, false) == GC::SS_BRANCH);
   assert(eqp(PT, {{0,0}, {0,1}, {1,0}, {1,1}}));
  }

  {const CS::LaSq A(2);
   const std::unique_ptr<CS::GenericMolsNB> m = A.space();
   ReductionStatistics stats0(m->V, 0);
   assert(probe(m.get(), 0, 0, stats0, false) == GC::SS_SOLVED);
   assert(probe(m.get(), 0, 1, stats0, false) == GC::SS_SOLVED);
   assert(probe(m.get(), 1, 0, stats0, false) == GC::SS_SOLVED);
   assert(probe(m.get(), 1, 1, stats0, false) == GC::SS_SOLVED);
  }
  {const CS::LaSq A(2);
   const std::unique_ptr<CS::GenericMolsNB> m = A.space();
   ReductionStatistics stats0(m->V, 0);
   pruning_table_t PT;
   assert(probe(m.get(), 0, 0, PT, stats0, false) == GC::SS_SOLVED);
   assert(PT.empty());
   assert(probe(m.get(), 0, 1, PT, stats0, false) == GC::SS_SOLVED);
   assert(PT.empty());
   assert(probe(m.get(), 1, 0, PT, stats0, false) == GC::SS_SOLVED);
   assert(PT.empty());
   assert(probe(m.get(), 1, 1, PT, stats0, false) == GC::SS_SOLVED);
   assert(PT.empty());
  }

  {const CS::Square A(3);
   const std::unique_ptr<CS::GenericMolsNB> m = A.space();
   ReductionStatistics stats0(m->V, 0);
   assert(probe(m.get(), 0, 0, stats0, false) == GC::SS_BRANCH);
   assert(probe(m.get(), 0, 1, stats0, false) == GC::SS_BRANCH);
   assert(probe(m.get(), 0, 2, stats0, false) == GC::SS_BRANCH);
   assert(probe(m.get(), 1, 0, stats0, false) == GC::SS_BRANCH);
   assert(probe(m.get(), 1, 1, stats0, false) == GC::SS_BRANCH);
   assert(probe(m.get(), 1, 2, stats0, false) == GC::SS_BRANCH);
   assert(probe(m.get(), 2, 0, stats0, false) == GC::SS_BRANCH);
   assert(probe(m.get(), 2, 1, stats0, false) == GC::SS_BRANCH);
   assert(probe(m.get(), 2, 2, stats0, false) == GC::SS_BRANCH);
  }
  {const CS::Square A(3);
   const std::unique_ptr<CS::GenericMolsNB> m = A.space();
   ReductionStatistics stats0(m->V, 0);
   pruning_table_t PT;
   assert(probe(m.get(), 0, 0, PT, stats0, false) == GC::SS_BRANCH);
   assert(eqp(PT, {{0,0}}));
   assert(probe(m.get(), 0, 1, PT, stats0, false) == GC::SS_BRANCH);
   assert(eqp(PT, {{0,0}, {0,1}}));
   assert(probe(m.get(), 0, 2, PT, stats0, false) == GC::SS_BRANCH);
   assert(eqp(PT, {{0,0}, {0,1}, {0,2}}));
   assert(probe(m.get(), 1, 0, PT, stats0, false) == GC::SS_BRANCH);
   assert(eqp(PT, {{0,0}, {0,1}, {0,2}, {1,0}}));
   assert(probe(m.get(), 1, 1, PT, stats0, false) == GC::SS_BRANCH);
   assert(eqp(PT, {{0,0}, {0,1}, {0,2}, {1,0}, {1,1}}));
   assert(probe(m.get(), 1, 2, PT, stats0, false) == GC::SS_BRANCH);
   assert(eqp(PT, {{0,0}, {0,1}, {0,2}, {1,0}, {1,1}, {1,2}}));
   assert(probe(m.get(), 2, 0, PT, stats0, false) == GC::SS_BRANCH);
   assert(eqp(PT, {{0,0}, {0,1}, {0,2}, {1,0}, {1,1}, {1,2}, {2,0}}));
   assert(probe(m.get(), 2, 1, PT, stats0, false) == GC::SS_BRANCH);
   assert(eqp(PT, {{0,0}, {0,1}, {0,2}, {1,0}, {1,1}, {1,2}, {2,0}, {2,1}}));
   assert(probe(m.get(), 2, 2, PT, stats0, false) == GC::SS_BRANCH);
   assert(eqp(PT, {{0,0}, {0,1}, {0,2}, {1,0}, {1,1}, {1,2}, {2,0}, {2,1},
                   {2,2}}));
  }

  {const CS::LaSq A(3, "A\n0 * *\n* * *\n* * *\n");
   const std::unique_ptr<CS::GenericMolsNB> m = A.space();
   ReductionStatistics stats0(m->V, 0);
   assert(probe(m.get(), 0, 0, stats0, false) == GC::SS_BRANCH);
   assert(probe(m.get(), 0, 1, stats0, false) == GC::SS_FAILED);
   assert(probe(m.get(), 0, 2, stats0, false) == GC::SS_FAILED);
   assert(probe(m.get(), 1, 0, stats0, false) == GC::SS_FAILED);
   assert(probe(m.get(), 1, 1, stats0, false) == GC::SS_BRANCH);
   assert(probe(m.get(), 1, 2, stats0, false) == GC::SS_BRANCH);
   assert(probe(m.get(), 2, 0, stats0, false) == GC::SS_FAILED);
   assert(probe(m.get(), 2, 1, stats0, false) == GC::SS_BRANCH);
   assert(probe(m.get(), 2, 2, stats0, false) == GC::SS_BRANCH);
  }
  {const CS::LaSq A(3, "A\n0 * *\n* * *\n* * *\n");
   const std::unique_ptr<CS::GenericMolsNB> m = A.space();
   ReductionStatistics stats0(m->V, 0);
   pruning_table_t PT;
   assert(probe(m.get(), 0, 0, PT, stats0, false) == GC::SS_BRANCH);
   assert(PT.empty());
   assert(probe(m.get(), 0, 1, PT, stats0, false) == GC::SS_FAILED);
   assert(PT.empty());
   assert(probe(m.get(), 0, 2, PT, stats0, false) == GC::SS_FAILED);
   assert(PT.empty());
   assert(probe(m.get(), 1, 0, PT, stats0, false) == GC::SS_FAILED);
   assert(PT.empty());
   assert(probe(m.get(), 1, 1, PT, stats0, false) == GC::SS_BRANCH);
   assert(eqp(PT, {{1,1}, {2,2}}));
   assert(probe(m.get(), 1, 2, PT, stats0, false) == GC::SS_BRANCH);
   assert(eqp(PT, {{1,1}, {1,2}, {2,1}, {2,2}}));
   assert(probe(m.get(), 2, 0, PT, stats0, false) == GC::SS_FAILED);
   assert(eqp(PT, {{1,1}, {1,2}, {2,1}, {2,2}}));
   assert(probe(m.get(), 2, 1, PT, stats0, false) == GC::SS_BRANCH);
   assert(eqp(PT, {{1,1}, {1,2}, {2,1}, {2,2}}));
   assert(probe(m.get(), 2, 2, PT, stats0, false) == GC::SS_BRANCH);
   assert(eqp(PT, {{1,1}, {1,2}, {2,1}, {2,2}}));
  }

  {const CS::LaSq A(3, "A\n* * *\n* 1 *\n* * *\n");
   const std::unique_ptr<CS::GenericMolsNB> m = A.space();
   ReductionStatistics stats0(m->V, 0);
   assert(probe(m.get(), 0, 0, stats0, false) == GC::SS_SOLVED);
   assert(probe(m.get(), 0, 1, stats0, false) == GC::SS_BRANCH);
   assert(probe(m.get(), 0, 2, stats0, false) == GC::SS_SOLVED);
   assert(probe(m.get(), 1, 0, stats0, false) == GC::SS_BRANCH);
   assert(probe(m.get(), 1, 1, stats0, false) == GC::SS_FAILED);
   assert(probe(m.get(), 1, 2, stats0, false) == GC::SS_BRANCH);
   assert(probe(m.get(), 2, 0, stats0, false) == GC::SS_SOLVED);
   assert(probe(m.get(), 2, 1, stats0, false) == GC::SS_BRANCH);
   assert(probe(m.get(), 2, 2, stats0, false) == GC::SS_SOLVED);
  }
  {const CS::LaSq A(3, "A\n* * *\n* 1 *\n* * *\n");
   const std::unique_ptr<CS::GenericMolsNB> m = A.space();
   ReductionStatistics stats0(m->V, 0);
   pruning_table_t PT;
   assert(probe(m.get(), 0, 0, PT, stats0, false) == GC::SS_SOLVED);
   assert(PT.empty());
   assert(probe(m.get(), 0, 1, PT, stats0, false) == GC::SS_BRANCH);
   assert(eqp(PT, {{0,1}}));
   assert(probe(m.get(), 0, 2, PT, stats0, false) == GC::SS_SOLVED);
   assert(eqp(PT, {{0,1}}));
   assert(probe(m.get(), 1, 0, PT, stats0, false) == GC::SS_BRANCH);
   assert(eqp(PT, {{0,1}, {1,0}, {7,2}}));
   assert(probe(m.get(), 1, 1, PT, stats0, false) == GC::SS_FAILED);
   assert(eqp(PT, {{0,1}, {1,0}, {7,2}}));
   assert(probe(m.get(), 1, 2, PT, stats0, false) == GC::SS_BRANCH);
   assert(eqp(PT, {{0,1}, {1,0}, {1,2}, {7,0}, {7,2}}));
   assert(probe(m.get(), 2, 0, PT, stats0, false) == GC::SS_SOLVED);
   assert(eqp(PT, {{0,1}, {1,0}, {1,2}, {7,0}, {7,2}}));
   assert(probe(m.get(), 2, 1, PT, stats0, false) == GC::SS_BRANCH);
   assert(eqp(PT, {{0,1}, {1,0}, {1,2}, {2,1}, {7,0}, {7,2}}));
   assert(probe(m.get(), 2, 2, PT, stats0, false) == GC::SS_SOLVED);
   assert(eqp(PT, {{0,1}, {1,0}, {1,2}, {2,1}, {7,0}, {7,2}}));
  }

  {for (size_t N = 2; N <= 5; ++N)
     for (const RDL rdl : ET::allvals<RDL>())
     for (const LAR lar : ET::allvals<LAR>())
       for (const RT rt : ET::allvals<RT>()) {
         const CS::Square A(N);
         const std::unique_ptr<CS::GenericMolsNB> m = A.space();
         const ReductionStatistics stats =
           lareduction<CS::GenericMolsNB>(m.get(), rt, rdl, lar);
         assert(eqwt(stats, A.laredstats(rdl,lar,rt)));
       }
  }

  {const CS::Square A(3, "A\n0 * *\n* * *\n* * *\n");
   for (const RDL rdl : ET::allvals<RDL>())
   for (const LAR lar : ET::allvals<LAR>())
     for (const RT rt : ET::allvals<RT>()) {
       const std::unique_ptr<CS::GenericMolsNB> m = A.space();
       assert(sumdomsizes(m->V) == 27 - 2);
       const ReductionStatistics s =
         lareduction<CS::GenericMolsNB>(m.get(), rt, rdl, lar);
       assert(eqwt(s, A.laredstats(rdl,lar,rt)));
     }
  }
  {const CS::Square A(3, "A\n* * *\n* 1 *\n* * *\n");
   for (const RDL rdl : ET::allvals<RDL>())
   for (const LAR lar : ET::allvals<LAR>())
     for (const RT rt : ET::allvals<RT>()) {
       const std::unique_ptr<CS::GenericMolsNB> m = A.space();
       assert(sumdomsizes(m->V) == 27 - 2);
       const ReductionStatistics s =
         lareduction<CS::GenericMolsNB>(m.get(), rt, rdl, lar);
       assert(eqwt(s, A.laredstats(rdl,lar,rt)));
     }
  }
  {const CS::Square A(3, "A\n0 * *\n* 1 *\n* * 2\n");
   for (const RDL rdl : ET::allvals<RDL>())
   for (const LAR lar : ET::allvals<LAR>())
     for (const RT rt : ET::allvals<RT>()) {
       const std::unique_ptr<CS::GenericMolsNB> m = A.space();
       assert(sumdomsizes(m->V) == 27 - 6);
       const ReductionStatistics s =
         lareduction<CS::GenericMolsNB>(m.get(), rt, rdl, lar);
       assert(eqwt(s, A.laredstats(rdl,lar,rt)));
     }
  }

  {for (size_t N = 2; N <= 5; ++N)
     for (const RDL rdl : ET::allvals<RDL>())
     for (const LAR lar : ET::allvals<LAR>())
       for (const RT rt : ET::allvals<RT>()) {
         if (test_sat(rt) or test_unique(rt)) continue;
         const CS::LaSq A(N);
         const std::unique_ptr<CS::GenericMolsNB> m = A.space();
         const ReductionStatistics stats =
           lareduction<CS::GenericMolsNB>(m.get(), rt, rdl, lar);
         assert(eqwt(stats, A.laredstats(lar, rt)));
       }
  }

  {const CS::LaSq A(3, "A\n0 * *\n* * *\n* * *\n");
   const std::unique_ptr<CS::GenericMolsNB> m = A.space();
   assert(sumdomsizes(m->V) == 27 - 2 - 2*2);
   const ReductionStatistics s =
     lareduction<CS::GenericMolsNB>(m.get(), RT::enumerate_solutions,
       RDL::basic, LAR::rel_npr);
   assert(s.mu0() == (27-9) - 2 - 2*2); // = 12
   assert(s.numvars() == 8);
   assert(s.numvals() == 8*3 - 2*2); // = 20
   assert(s.props() == 2);
   assert(s.rounds() == 1);
   assert(s.solc() == 4);
   assert(s.leaf());
   assert(s.elimvals() == 2*2);
   assert(s.prunes() == 0);
   assert(s.maxprune() == 0);
   assert(s.probes() == 3*2 + 3 + 2);
   assert(s.quotelimvals() == 100 * LR::float_t(7) / 20);
   assert(s.quotprune() == 0);
   const auto list_sol = extract(A.e.ldecode(s.sollist()));
   assert(eqp(list_sol, {
              {{{0,2,1},{2,1,0},{1,0,2}}}, {{{0,1,2},{1,2,0},{2,0,1}}},
              {{{0,1,2},{2,0,1},{1,2,0}}}, {{{0,2,1},{1,0,2},{2,1,0}}}
            }));
  }
  {const CS::LaSq A(3, "A\n0 * *\n* * *\n* * *\n");
   const std::unique_ptr<CS::GenericMolsNB> m = A.space();
   assert(sumdomsizes(m->V) == 27 - 2 - 2*2);
   const ReductionStatistics s =
     lareduction<CS::GenericMolsNB>(m.get(), RT::enumerate_solutions,
       RDL::basic, LAR::rel_pr);
   assert(s.mu0() == (27-9) - 2 - 2*2); // = 12
   assert(s.numvars() == 8);
   assert(s.numvals() == (12 + 8)); // = 20
   assert(s.props() == 2);
   assert(s.rounds() == 1);
   assert(s.solc() == 4);
   assert(s.leaf());
   assert(s.elimvals() == 2*2);
   assert(s.probes() == 2*2 + 3 + 2);
   assert(s.prunes() == 2);
   assert(s.maxprune() == 4*2);
   assert(s.quotelimvals() == 100 * LR::float_t(7) / 20);
   assert(s.quotprune() == 100 * LR::float_t(2) / 9);
   const auto list_sol = extract(A.e.ldecode(s.sollist()));
   assert(eqp(list_sol, {
              {{{0,2,1},{2,1,0},{1,0,2}}}, {{{0,1,2},{1,2,0},{2,0,1}}},
              {{{0,1,2},{2,0,1},{1,2,0}}}, {{{0,2,1},{1,0,2},{2,1,0}}}
            }));
  }
  {const CS::LaSq A(3, "A\n0 * *\n* * *\n* * *\n");
   const std::unique_ptr<CS::GenericMolsNB> m = A.space();
   assert(sumdomsizes(m->V) == 27 - 2 - 2*2);
   const ReductionStatistics s =
     lareduction<CS::GenericMolsNB>(m.get(), RT::enumerate_solutions,
       RDL::basic, LAR::eag_npr);
   assert(s.mu0() == (27-9) - 2 - 2*2);
   assert(s.props() == 2);
   assert(s.rounds() == 2);
   assert(s.solc() == 4);
   assert(s.leaf());
   assert(s.elimvals() == 2*2);
   assert(s.prunes() == 0);
   assert(s.maxprune() == 0);
   assert(s.probes() == 3*2 + 3 + 2);
   assert(s.quotelimvals() == 100 * LR::float_t(7) / 20);
   assert(s.quotprune() == 0);
   const auto list_sol = extract(A.e.ldecode(s.sollist()));
   assert(eqp(list_sol, {
              {{{0,2,1},{2,1,0},{1,0,2}}}, {{{0,1,2},{1,2,0},{2,0,1}}},
              {{{0,1,2},{2,0,1},{1,2,0}}}, {{{0,2,1},{1,0,2},{2,1,0}}}
            }));
  }
  {const CS::LaSq A(3, "A\n0 * *\n* * *\n* * *\n");
   const std::unique_ptr<CS::GenericMolsNB> m = A.space();
   assert(sumdomsizes(m->V) == 27 - 2 - 2*2);
   const ReductionStatistics s =
     lareduction<CS::GenericMolsNB>(m.get(), RT::enumerate_solutions,
       RDL::basic, LAR::eag_pr);
   assert(s.mu0() == (27-9) - 2 - 2*2);
   assert(s.props() == 2);
   assert(s.rounds() == 2);
   assert(s.solc() == 4);
   assert(s.leaf());
   assert(s.elimvals() == 2*2);
   assert(s.probes() == 2*2 + 3 + 2);
   assert(s.prunes() == 2);
   assert(s.maxprune() == 4*2);
   assert(s.quotelimvals() == 100 * LR::float_t(7) / 20);
   assert(s.quotprune() == 100 * LR::float_t(2) / 9);
   const auto list_sol = extract(A.e.ldecode(s.sollist()));
   assert(eqp(list_sol, {
              {{{0,2,1},{2,1,0},{1,0,2}}}, {{{0,1,2},{1,2,0},{2,0,1}}},
              {{{0,1,2},{2,0,1},{1,2,0}}}, {{{0,2,1},{1,0,2},{2,1,0}}}
            }));
  }

  {const CS::LaSq A(3, "A\n* * *\n* 1 *\n* * *\n");
   for (const RT rt : ET::allvals<RT>()) {
     const std::unique_ptr<CS::GenericMolsNB> m = A.space();
     assert(sumdomsizes(m->V) == 27 - 2 - 2*2);
     const ReductionStatistics s =
       lareduction<CS::GenericMolsNB>(m.get(), rt, RDL::basic, LAR::rel_npr);
     assert(s.mu0() == (27-9) - 2 - 2*2);
     assert(s.rounds() == 1);
     assert(s.prunes() == 0);
     assert(s.maxprune() == 0);
     assert(s.quotprune() == 0);
     assert(s.leaf());
     assert(s.props() == 2);
     assert(s.solc() == 4);
     assert(s.elimvals() == 2*2);
     assert(s.probes() == 3 + 2);
     assert(s.quotelimvals() == 100 * LR::float_t(7) / 20);
     if (with_solutions(rt)) {
       const auto list_sol = extract(A.e.ldecode(s.sollist()));
       assert(eqp(list_sol, {
                  {{{0,2,1},{2,1,0},{1,0,2}}}, {{{2,0,1},{0,1,2},{1,2,0}}},
                  {{{1,0,2},{2,1,0},{0,2,1}}}, {{{1,2,0},{0,1,2},{2,0,1}}}
                }));
     }
     else {
       assert(s.sollist().empty());
     }
   }
  }

  {const CS::LaSq A(4, "A\n0 * * *\n* 0 3 *\n* 3 0 *\n* * * 0\n");
/*
  0 * * *
  * 0 3 *
  * 3 0 *
  * * * 0

direct value-propagation:

  0     [-03] [-03] [-0]
  [-03] 0     3     [-03]
  [-03] 3     0     [-03]
  [-0]  [-03] [-03] 0

6 cells with domsize = 1
8 cells with domsize = 2
2 cells with domsize = 3

sumdomsizes = 6 + 8*2 + 2*3 = 28
numvars = 8 + 2 = 10
numvals = 28 - 6 = 22
mu0 = 28 - 16 = 12

4 solutions:

  0   1 2 3
  1/2 0 3 2/1
  2/1 3 0 1/2
  3   2 1 0

  0   2 1 3
  1/2 0 3 2/1
  2/1 3 0 1/2
  3   1 2 0

Thus forced are two cells:

  x x x 3
  x x x x
  x x x x
  3 x x x

la-reduction (eager, no pruning):

round 1:

2*2+1=5 probings without contradiction in first row:

  x 12 12 3

Plus 2 successful probes:

  x x x 12

yielding

  x x x 3

round 2:

6*2+1=13 probings without contradiction:

  x 12 12 x
  12 x x 12
  12 x x 12
  3

Then 2 successful probes:

  x x x x
  x x x x
  x x x x
  12

yielding again 3 in that cell.

round 3:

8*2=16 probings without contradiction.

Direct value-propagation yields finally

  0     [-03] [-03] 3
  [-03] 0     3     [-03]
  [-03] 3     0     [-03]
  3     [-03] [-03] 0

8 cells with domsize = 1
8 cells with domsize = 2

numvars = 8
numvals = 16
mu0 = 8


So altogether (5+2) + (13 + 2) + 16 = 38 probes.

Yielding

pprobes = 100 * 38 / 22 = 100 * 19/11 ~ 172.7273

Delta n = 2.
Delta mu0 = 4.
Delta numvalues = 6

pelvals = 100 * 6 / 22 = 100 * 3 / 11 ~ 27.27273

*/
   const std::unique_ptr<CS::GenericMolsNB> m = A.space();
   assert(sumdomsizes(m->V) == 64 - 3*6 - 2*8 - 2*1); // 64 - 18 - 16 - 2 = 28
   const ReductionStatistics s =
     lareduction<CS::GenericMolsNB>(m.get(), RT::enumerate_solutions,
                                    RDL::basic, LAR::eag_npr);
   assert(s.mu0() == (64-16) - 3*6 - 2*8 - 2*1); // = 12
   assert(s.numvars() == 16 - 6); // = 10
   assert(s.numvals() == 22);
   assert(s.props() == 2);
   assert(s.rounds() == 3);
   assert(s.solc() == 0);
   assert(not s.leaf());
   assert(s.elimvals() == 4);
   assert(s.probes() == 38);
   assert(s.prunes() == 0);
   assert(s.maxprune() == 0);
   assert(s.quotelimvals() == 100 * LR::float_t(6) / 22);
   assert(s.quotprune() == 0);
   assert(s.finalnumvars() == 8);
   assert(s.finalmu0() == 8);
   assert(s.sollist().empty());
  }

}

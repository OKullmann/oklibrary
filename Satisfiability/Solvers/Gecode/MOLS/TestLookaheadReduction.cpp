// Oleg Zaikin, 29.3.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

Testing of look-ahead reduction for the Gecode library.

BUG:

1. Wrong tests for eager reduction (marked with XXX)
    - If a reduction was found, then obviously at least two rounds are needed.

2. The lareduction-loop was never entered (marked with YYY)
    - The asserts seem completely off.

3. A complete revision regarding rounds etc. is needed.

TODOS:

0. Collection of test-scenarios
    - Some general structure is needed, which supports some kind of "database"
      of testcases.
    - So that it becomes easy to construct them (again and again in different
      test-environments, for testing *one* single function).
    - Properties are discussed of these cases, and possibly explained in text.
    - But best to have functions computing the various numbers.
    - At least the simpler cases should have N as parameter; possibly also
      additional parameters (so that a range of possibilities can be examined
      in loops).

1. Loop over the four possibilities of LAR (for lareduction):
    - Most cases should test all four possibilities -- it makes no sense to
      test only one of them, and not the others, on the same example.
    - Most testcases will have all these four possibilites different.

2. All possibilities of RT need testing (for lareduction):
    - Possibly also a loop should be used, for each testcase.

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
        "0.6.3",
        "28.5.2022",
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

  {const CS::LS A(2);
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
   ReductionStatistics stats0(m->V);
   assert(probe(m.get(), 0, 0, stats0, false) == GC::SS_BRANCH);
   assert(probe(m.get(), 0, 1, stats0, false) == GC::SS_BRANCH);
   assert(probe(m.get(), 1, 0, stats0, false) == GC::SS_BRANCH);
   assert(probe(m.get(), 1, 1, stats0, false) == GC::SS_BRANCH);
  }
  {const CS::Square A(2);
   const std::unique_ptr<CS::GenericMolsNB> m = A.space();
   ReductionStatistics stats0(m->V);
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

  {const CS::LS A(2);
   const std::unique_ptr<CS::GenericMolsNB> m = A.space();
   ReductionStatistics stats0(m->V);
   assert(probe(m.get(), 0, 0, stats0, false) == GC::SS_SOLVED);
   assert(probe(m.get(), 0, 1, stats0, false) == GC::SS_SOLVED);
   assert(probe(m.get(), 1, 0, stats0, false) == GC::SS_SOLVED);
   assert(probe(m.get(), 1, 1, stats0, false) == GC::SS_SOLVED);
  }
  {const CS::LS A(2);
   const std::unique_ptr<CS::GenericMolsNB> m = A.space();
   ReductionStatistics stats0(m->V);
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
   ReductionStatistics stats0(m->V);
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
   ReductionStatistics stats0(m->V);
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

  {const CS::LS A(3, "A\n0 * *\n* * *\n* * *\n");
   const std::unique_ptr<CS::GenericMolsNB> m = A.space();
   ReductionStatistics stats0(m->V);
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
  {const CS::LS A(3, "A\n0 * *\n* * *\n* * *\n");
   const std::unique_ptr<CS::GenericMolsNB> m = A.space();
   ReductionStatistics stats0(m->V);
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

  {const CS::LS A(3, "A\n* * *\n* 1 *\n* * *\n");
   const std::unique_ptr<CS::GenericMolsNB> m = A.space();
   ReductionStatistics stats0(m->V);
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
  {const CS::LS A(3, "A\n* * *\n* 1 *\n* * *\n");
   const std::unique_ptr<CS::GenericMolsNB> m = A.space();
   ReductionStatistics stats0(m->V);
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

  {for (size_t N = 2; N <= 5; ++N) {
     const CS::Square A(N);
     for (const LAR lar : ET::allvals<LAR>()) {
       const std::unique_ptr<CS::GenericMolsNB> m = A.space();
       const ReductionStatistics stats =
         lareduction<CS::GenericMolsNB>(m.get(), RT::enumerate_solutions, lar);
       assert(eqwt(stats, A.laredstats(lar)));
     }
   }
  }

  {const CS::LS A(3, "A\n0 * *\n* * *\n* * *\n");
   const std::unique_ptr<CS::GenericMolsNB> m = A.space();
   assert(sumdomsizes(m->V) == 27 - 2 - 2*2);
   const ReductionStatistics s =
     lareduction<CS::GenericMolsNB>(m.get(), RT::enumerate_solutions,
       LAR::rel_npr);
   assert(s.vals() == 27 - 2 - 2*2);
   assert(s.props() == 2);
   assert(s.rounds() == 1);
   assert(s.solc() == 4);
   assert(s.leafcount() == 1);
   assert(s.elimvals() == 2*2);
   assert(s.prunes() == 0);
   assert(s.maxprune() == 0);
   assert(s.probes() == 3*2 + 3 + 2);
   assert(s.quotelimvals() == LR::float_t(4) / 21);
   assert(s.quotprun() == 0);
   const auto list_sol = extract(A.e.ldecode(s.sollist()));
   assert(eqp(list_sol, {
              {{{0,2,1},{2,1,0},{1,0,2}}}, {{{0,1,2},{1,2,0},{2,0,1}}},
              {{{0,1,2},{2,0,1},{1,2,0}}}, {{{0,2,1},{1,0,2},{2,1,0}}}
            }));
  }

  {const CS::LS A(3, "A\n* * *\n* 1 *\n* * *\n");
   const std::unique_ptr<CS::GenericMolsNB> m = A.space();
   const ReductionStatistics stats =
     lareduction<CS::GenericMolsNB>(m.get(), RT::enumerate_solutions,
       LAR::eag_npr);
   /* ERROR FALSE TESTS
   assert(stats.props() == 1);
   assert(stats.elimvals() == 2);
   assert(stats.prunes() == 0);
   assert(stats.maxprune() == 0);
   assert(stats.probes() == 3);
   assert(stats.rounds() == 1);
   assert(stats.solc() == 2);
   assert(stats.leafcount() == 0);
   const auto list_sol = extract(enc.ldecode(stats.sollist()));
   assert(eqp(list_sol, {
              {{{0,2,1},{2,1,0},{1,0,2}}},
              {{{2,0,1},{0,1,2},{1,2,0}}}}));
   */
  }

}

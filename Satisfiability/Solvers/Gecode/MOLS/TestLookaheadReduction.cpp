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

-1. Move some functionality to proper place.
    - The helper function encoding() should be moved from here.
      Perhaps, TestEncoding.hpp or a new header TestCases.hpp.

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

1, Clear structure of tests
    - DONE A test should always test *one thing*.
    - DONE That means, exactly *one* of "child_node" or "probe" or "lareduction".
    - The current copy-and-pasting of some "child_node" and some "probe" seems
      to make little sense (not much care is spent on each candidate to be
      tested, and there are interferences).
    - DONE (First, child_node is tested, then probe, and finally, lareduction)
      The order of tests usually should follow the order in the code to be
      tested; here that means *first* one establishes that child_node is
      correct, *then* one establishes that probe is correct, and finally one
      establishes that lareduction is correct.

2. Loop over the four possibilities of LAR (for lareduction):
    - Most cases should test all four possibilities -- it makes no sense to
      test only one of them, and not the others, on the same example.
    - Most testcases will have all these four possibilites different.

3. All possibilities of RT need testing (for lareduction):
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
#include "Conditions.hpp"
#include "Parsing.hpp"
#include "PartialSquares.hpp"
#include "Encoding.hpp"
#include "Options.hpp"
#include "Constraints.hpp"
#include "Solvers.hpp"
#include "BasicLatinSquares.hpp"
#include "GcVariables.hpp"
#include "Cases.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.4.8",
        "21.5.2022",
        __FILE__,
        "Oleg Zaikin and Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Solvers/Gecode/MOLS/TestLookaheadReduction.cpp",
        "GPL v3"};

  using namespace LookaheadReduction;
  using namespace Conditions;
  using namespace Parsing;
  using namespace PartialSquares;
  using namespace Encoding;
  using namespace Options;
  using namespace Constraints;
  using namespace BasicLatinSquares;
  using namespace GcVariables;

  namespace CS = Cases;
  namespace GC = Gecode;

  typedef Options::RT RT;
  using listsol_t = Solvers::listsol_t;

  class GenericMolsNB : public GenericMols0 {
    struct Void : GC::Brancher {
      Void(const GC::Home home) : GC::Brancher(home) {}
      Void(GC::Space& home, Void& b) : GC::Brancher(home,b) {}
      GC::Brancher* copy(GC::Space& home) {return new (home) Void(home,*this);}
      bool status(const GC::Space& s) const noexcept {
        return not GcVariables::empty(static_cast<const GenericMols0&>(s).V);
      }
      GC::Choice* choice(GC::Space&) { assert(0); return nullptr; }
      GC::Choice* choice(const GC::Space&, GC::Archive&) {
        assert(0); return nullptr;
      }
      GC::ExecStatus commit(GC::Space&, const GC::Choice&, unsigned) {
        assert(0); return GC::ExecStatus(0);
      }
    };
  public :
    GenericMolsNB(const EncCond& enc) : GenericMols0(enc) {
      new (*this) Void(*this);
    }
  };

  EncCond encoding(const std::string condstr, const std::string psstr,
    const size_t N, const GC::IntPropLevel pl) noexcept {
    std::istringstream in_cond(condstr);
    std::istringstream in_ps(psstr);
    const AConditions ac = ReadAC()(in_cond);
    const PSquares ps = PSquares(N, in_ps);
    const EncCond enc(ac, ps, pl);
    return enc;
  }

  std::unique_ptr<GenericMolsNB> space(const EncCond enc) noexcept {
    std::unique_ptr<GenericMolsNB> m(new GenericMolsNB(enc));
    m->status();
    return m;
  }

  template <class X>
  constexpr bool eqp(const X& lhs, const X& rhs) noexcept {
    return lhs == rhs;
  }

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  {CS::Square A(2);
   const std::unique_ptr<GenericMolsNB> m = space(A.enc());
   const auto pl = GC::IPL_VAL;
   const auto ch = child_node<GenericMolsNB>(m.get(), 0, 0, pl, true);
   assert(eqp(values(ch->V, 0), {0}));
   assert(eqp(values(ch->V, 1), {0,1}));
   assert(eqp(values(ch->V, 2), {0,1}));
   assert(eqp(values(ch->V, 3), {0,1}));
   const auto st = ch->status();
   assert(st == Gecode::SS_BRANCH);
   const auto ch2 = child_node<GenericMolsNB>(m.get(), 0, 0, pl, false);
   assert(eqp(values(ch2->V, 0), {1}));
   assert(eqp(values(ch2->V, 1), {0,1}));
   assert(eqp(values(ch2->V, 2), {0,1}));
   assert(eqp(values(ch2->V, 3), {0,1}));
   const auto st2 = ch2->status();
   assert(st2 == Gecode::SS_BRANCH);
  }

  {const CS::TrivialLatinSquare A(2);
   const std::unique_ptr<GenericMolsNB> m = space(A.enc());
   const auto pl = GC::IPL_VAL;
   const auto ch = child_node<GenericMolsNB>(m.get(), 0, 0, pl, true);
   assert(eqp(values(ch->V, 0), {0}));
   assert(eqp(values(ch->V, 1), {0,1}));
   assert(eqp(values(ch->V, 2), {0,1}));
   assert(eqp(values(ch->V, 3), {0,1}));
   const auto st = ch->status();
   assert(st == Gecode::SS_SOLVED);
   const auto ch2 = child_node<GenericMolsNB>(m.get(), 0, 0, pl, false);
   assert(eqp(values(ch2->V, 0), {1}));
   assert(eqp(values(ch2->V, 1), {0,1}));
   assert(eqp(values(ch2->V, 2), {0,1}));
   assert(eqp(values(ch2->V, 3), {0,1}));
   const auto st2 = ch2->status();
   assert(st2 == Gecode::SS_SOLVED);
  }

  {CS::Square A(3);
   const std::unique_ptr<GenericMolsNB> m = space(A.enc());
   const auto pl = GC::IPL_VAL;
   const auto ch = child_node<GenericMolsNB>(m.get(), 0, 0, pl, true);
   assert(eqp(values(ch->V, 0), {0}));
   assert(eqp(values(ch->V, 1), {0,1,2}));
   assert(eqp(values(ch->V, 2), {0,1,2}));
   assert(eqp(values(ch->V, 3), {0,1,2}));
   assert(eqp(values(ch->V, 4), {0,1,2}));
   assert(eqp(values(ch->V, 5), {0,1,2}));
   assert(eqp(values(ch->V, 6), {0,1,2}));
   assert(eqp(values(ch->V, 7), {0,1,2}));
   assert(eqp(values(ch->V, 8), {0,1,2}));
   const auto st = ch->status();
   assert(st == Gecode::SS_BRANCH);
   const auto ch2 = child_node<GenericMolsNB>(m.get(), 0, 0, pl, false);
   assert(eqp(values(ch2->V, 0), {1,2}));
   assert(eqp(values(ch2->V, 1), {0,1,2}));
   assert(eqp(values(ch2->V, 2), {0,1,2}));
   assert(eqp(values(ch2->V, 3), {0,1,2}));
   assert(eqp(values(ch2->V, 4), {0,1,2}));
   assert(eqp(values(ch2->V, 5), {0,1,2}));
   assert(eqp(values(ch2->V, 6), {0,1,2}));
   assert(eqp(values(ch2->V, 7), {0,1,2}));
   assert(eqp(values(ch2->V, 8), {0,1,2}));
   const auto st2 = ch2->status();
   assert(st2 == Gecode::SS_BRANCH);
  }

  {const EncCond enc = encoding("squares A\n", "A\n0 * *\n* * *\n* * *\n",
     3, GC::IPL_VAL);
   const std::unique_ptr<GenericMolsNB> m = space(enc);
   const auto pl = GC::IPL_VAL;
   const auto ch = child_node<GenericMolsNB>(m.get(), 0, 0, pl, true);
   assert(eqp(values(ch->V, 0), {0}));
   assert(eqp(values(ch->V, 1), {0,1,2}));
   assert(eqp(values(ch->V, 2), {0,1,2}));
   assert(eqp(values(ch->V, 3), {0,1,2}));
   assert(eqp(values(ch->V, 4), {0,1,2}));
   assert(eqp(values(ch->V, 5), {0,1,2}));
   assert(eqp(values(ch->V, 6), {0,1,2}));
   assert(eqp(values(ch->V, 7), {0,1,2}));
   assert(eqp(values(ch->V, 8), {0,1,2}));
   const auto st = ch->status();
   assert(st == Gecode::SS_BRANCH);
   const auto ch2 = child_node<GenericMolsNB>(m.get(), 0, 0, pl, false);
   assert(eqp(values(ch2->V, 0), {0}));
   assert(eqp(values(ch2->V, 1), {0,1,2}));
   assert(eqp(values(ch2->V, 2), {0,1,2}));
   assert(eqp(values(ch2->V, 3), {0,1,2}));
   assert(eqp(values(ch2->V, 4), {0,1,2}));
   assert(eqp(values(ch2->V, 5), {0,1,2}));
   assert(eqp(values(ch2->V, 6), {0,1,2}));
   assert(eqp(values(ch2->V, 7), {0,1,2}));
   assert(eqp(values(ch2->V, 8), {0,1,2}));
   const auto st2 = ch2->status();
   assert(st2 == Gecode::SS_FAILED);
  }

  {const EncCond enc = encoding("squares A\n", "A\n* * *\n* 1 *\n* * *\n",
     3, GC::IPL_VAL);
   const std::unique_ptr<GenericMolsNB> m = space(enc);
   const auto pl = GC::IPL_VAL;
   const auto ch = child_node<GenericMolsNB>(m.get(), 0, 0, pl, true);
   assert(eqp(values(ch->V, 0), {0}));
   assert(eqp(values(ch->V, 1), {0,1,2}));
   assert(eqp(values(ch->V, 2), {0,1,2}));
   assert(eqp(values(ch->V, 3), {0,1,2}));
   assert(eqp(values(ch->V, 4), {1}));
   assert(eqp(values(ch->V, 5), {0,1,2}));
   assert(eqp(values(ch->V, 6), {0,1,2}));
   assert(eqp(values(ch->V, 7), {0,1,2}));
   assert(eqp(values(ch->V, 8), {0,1,2}));
   const auto st = ch->status();
   assert(st == Gecode::SS_BRANCH);
   const auto ch2 = child_node<GenericMolsNB>(m.get(), 0, 0, pl, false);
   assert(eqp(values(ch2->V, 0), {1,2}));
   assert(eqp(values(ch2->V, 1), {0,1,2}));
   assert(eqp(values(ch2->V, 2), {0,1,2}));
   assert(eqp(values(ch2->V, 3), {0,1,2}));
   assert(eqp(values(ch2->V, 4), {1}));
   assert(eqp(values(ch2->V, 5), {0,1,2}));
   assert(eqp(values(ch2->V, 6), {0,1,2}));
   assert(eqp(values(ch2->V, 7), {0,1,2}));
   assert(eqp(values(ch2->V, 8), {0,1,2}));
   const auto st2 = ch2->status();
   assert(st2 == Gecode::SS_BRANCH);
  }

  {CS::Square A(2);
   const std::unique_ptr<GenericMolsNB> m = space(A.enc());
   ReductionStatistics stats0(m->V);
   const auto pl = GC::IPL_VAL;
   assert(probe(m.get(), 0, 0, pl, stats0, false) == Gecode::SS_BRANCH);
   assert(probe(m.get(), 0, 1, pl, stats0, false) == Gecode::SS_BRANCH);
   assert(probe(m.get(), 1, 0, pl, stats0, false) == Gecode::SS_BRANCH);
   assert(probe(m.get(), 1, 1, pl, stats0, false) == Gecode::SS_BRANCH);
  }
  {CS::Square A(2);
   const std::unique_ptr<GenericMolsNB> m = space(A.enc());
   ReductionStatistics stats0(m->V);
   pruning_table_t PT;
   const auto pl = GC::IPL_VAL;
   assert(probe(m.get(), 0,0, pl, PT, stats0, false) == Gecode::SS_BRANCH);
   assert(eqp(PT, {{0,0}}));
   assert(probe(m.get(), 0,1, pl, PT, stats0, false) == Gecode::SS_BRANCH);
   assert(eqp(PT, {{0,0}, {0,1}}));
   assert(probe(m.get(), 1,0, pl, PT, stats0, false) == Gecode::SS_BRANCH);
   assert(eqp(PT, {{0,0}, {0,1}, {1,0}}));
   assert(probe(m.get(), 1,1, pl, PT, stats0, false) == Gecode::SS_BRANCH);
   assert(eqp(PT, {{0,0}, {0,1}, {1,0}, {1,1}}));
  }

  {const CS::TrivialLatinSquare A(2);
   const std::unique_ptr<GenericMolsNB> m = space(A.enc());
   ReductionStatistics stats0(m->V);
   const auto pl = GC::IPL_VAL;
   assert(probe(m.get(), 0, 0, pl, stats0, false) == Gecode::SS_SOLVED);
   assert(probe(m.get(), 0, 1, pl, stats0, false) == Gecode::SS_SOLVED);
   assert(probe(m.get(), 1, 0, pl, stats0, false) == Gecode::SS_SOLVED);
   assert(probe(m.get(), 1, 1, pl, stats0, false) == Gecode::SS_SOLVED);
  }
  {const EncCond enc = encoding("squares A\nls A\n", "", 2, GC::IPL_VAL);
   const std::unique_ptr<GenericMolsNB> m = space(enc);
   ReductionStatistics stats0(m->V);
   pruning_table_t PT;
   const auto pl = GC::IPL_VAL;
   assert(probe(m.get(), 0,0, pl, PT, stats0, false) == Gecode::SS_SOLVED);
   assert(PT.empty());
   assert(probe(m.get(), 0,1, pl, PT, stats0, false) == Gecode::SS_SOLVED);
   assert(PT.empty());
   assert(probe(m.get(), 1,0, pl, PT, stats0, false) == Gecode::SS_SOLVED);
   assert(PT.empty());
   assert(probe(m.get(), 1,1, pl, PT, stats0, false) == Gecode::SS_SOLVED);
   assert(PT.empty());
  }

  {CS::Square A(3);
   const std::unique_ptr<GenericMolsNB> m = space(A.enc());
   ReductionStatistics stats0(m->V);
   const auto pl = GC::IPL_VAL;
   assert(probe(m.get(), 0, 0, pl, stats0, false) == Gecode::SS_BRANCH);
   assert(probe(m.get(), 0, 1, pl, stats0, false) == Gecode::SS_BRANCH);
   assert(probe(m.get(), 0, 2, pl, stats0, false) == Gecode::SS_BRANCH);
   assert(probe(m.get(), 1, 0, pl, stats0, false) == Gecode::SS_BRANCH);
   assert(probe(m.get(), 1, 1, pl, stats0, false) == Gecode::SS_BRANCH);
   assert(probe(m.get(), 1, 2, pl, stats0, false) == Gecode::SS_BRANCH);
   assert(probe(m.get(), 2, 0, pl, stats0, false) == Gecode::SS_BRANCH);
   assert(probe(m.get(), 2, 1, pl, stats0, false) == Gecode::SS_BRANCH);
   assert(probe(m.get(), 2, 2, pl, stats0, false) == Gecode::SS_BRANCH);
  }
  {CS::Square A(3);
   const std::unique_ptr<GenericMolsNB> m = space(A.enc());
   ReductionStatistics stats0(m->V);
   pruning_table_t PT;
   const auto pl = GC::IPL_VAL;
   assert(probe(m.get(), 0,0, pl, PT, stats0, false) == Gecode::SS_BRANCH);
   assert(eqp(PT, {{0,0}}));
   assert(probe(m.get(), 0,1, pl, PT, stats0, false) == Gecode::SS_BRANCH);
   assert(eqp(PT, {{0,0}, {0,1}}));
   assert(probe(m.get(), 0,2, pl, PT, stats0, false) == Gecode::SS_BRANCH);
   assert(eqp(PT, {{0,0}, {0,1}, {0,2}}));
   assert(probe(m.get(), 1,0, pl, PT, stats0, false) == Gecode::SS_BRANCH);
   assert(eqp(PT, {{0,0}, {0,1}, {0,2}, {1,0}}));
   assert(probe(m.get(), 1,1, pl, PT, stats0, false) == Gecode::SS_BRANCH);
   assert(eqp(PT, {{0,0}, {0,1}, {0,2}, {1,0}, {1,1}}));
   assert(probe(m.get(), 1,2, pl, PT, stats0, false) == Gecode::SS_BRANCH);
   assert(eqp(PT, {{0,0}, {0,1}, {0,2}, {1,0}, {1,1}, {1,2}}));
   assert(probe(m.get(), 2,0, pl, PT, stats0, false) == Gecode::SS_BRANCH);
   assert(eqp(PT, {{0,0}, {0,1}, {0,2}, {1,0}, {1,1}, {1,2}, {2,0}}));
   assert(probe(m.get(), 2,1, pl, PT, stats0, false) == Gecode::SS_BRANCH);
   assert(eqp(PT, {{0,0}, {0,1}, {0,2}, {1,0}, {1,1}, {1,2}, {2,0}, {2,1}}));
   assert(probe(m.get(), 2,2, pl, PT, stats0, false) == Gecode::SS_BRANCH);
   assert(eqp(PT, {{0,0}, {0,1}, {0,2}, {1,0}, {1,1}, {1,2}, {2,0}, {2,1},
     {2,2}}));
  }

  {const EncCond enc = encoding("squares A\nls A\n",
     "A\n0 * *\n* * *\n* * *\n", 3, GC::IPL_VAL);
   const std::unique_ptr<GenericMolsNB> m = space(enc);
   ReductionStatistics stats0(m->V);
   const auto pl = GC::IPL_VAL;
   assert(probe(m.get(), 0, 0, pl, stats0, false) == Gecode::SS_BRANCH);
   assert(probe(m.get(), 0, 1, pl, stats0, false) == Gecode::SS_FAILED);
   assert(probe(m.get(), 0, 2, pl, stats0, false) == Gecode::SS_FAILED);
   assert(probe(m.get(), 1, 0, pl, stats0, false) == Gecode::SS_FAILED);
   assert(probe(m.get(), 1, 1, pl, stats0, false) == Gecode::SS_BRANCH);
   assert(probe(m.get(), 1, 2, pl, stats0, false) == Gecode::SS_BRANCH);
   assert(probe(m.get(), 2, 0, pl, stats0, false) == Gecode::SS_FAILED);
   assert(probe(m.get(), 2, 1, pl, stats0, false) == Gecode::SS_BRANCH);
   assert(probe(m.get(), 2, 2, pl, stats0, false) == Gecode::SS_BRANCH);
  }
  {const EncCond enc = encoding("squares A\nls A\n",
     "A\n0 * *\n* * *\n* * *\n", 3, GC::IPL_VAL);
   const std::unique_ptr<GenericMolsNB> m = space(enc);
   ReductionStatistics stats0(m->V);
   pruning_table_t PT;
   const auto pl = GC::IPL_VAL;
   assert(probe(m.get(), 0,0, pl, PT, stats0, false) == Gecode::SS_BRANCH);
   assert(PT.empty());
   assert(probe(m.get(), 0,1, pl, PT, stats0, false) == Gecode::SS_FAILED);
   assert(PT.empty());
   assert(probe(m.get(), 0,2, pl, PT, stats0, false) == Gecode::SS_FAILED);
   assert(PT.empty());
   assert(probe(m.get(), 1,0, pl, PT, stats0, false) == Gecode::SS_FAILED);
   assert(PT.empty());
   assert(probe(m.get(), 1,1, pl, PT, stats0, false) == Gecode::SS_BRANCH);
   assert(eqp(PT, {{1,1}, {2,2}}));
   assert(probe(m.get(), 1,2, pl, PT, stats0, false) == Gecode::SS_BRANCH);
   assert(eqp(PT, {{1,1}, {1,2}, {2,1}, {2,2}}));
   assert(probe(m.get(), 2,0, pl, PT, stats0, false) == Gecode::SS_FAILED);
   assert(eqp(PT, {{1,1}, {1,2}, {2,1}, {2,2}}));
   assert(probe(m.get(), 2,1, pl, PT, stats0, false) == Gecode::SS_BRANCH);
   assert(eqp(PT, {{1,1}, {1,2}, {2,1}, {2,2}}));
   assert(probe(m.get(), 2,2, pl, PT, stats0, false) == Gecode::SS_BRANCH);
   assert(eqp(PT, {{1,1}, {1,2}, {2,1}, {2,2}}));
  }

  {const EncCond enc = encoding("squares A\nls A\n",
     "A\n* * *\n* 1 *\n* * *\n", 3, GC::IPL_VAL);
   const std::unique_ptr<GenericMolsNB> m = space(enc);
   ReductionStatistics stats0(m->V);
   const auto pl = GC::IPL_VAL;
   assert(probe(m.get(), 0, 0, pl, stats0, false) == Gecode::SS_SOLVED);
   assert(probe(m.get(), 0, 1, pl, stats0, false) == Gecode::SS_BRANCH);
   assert(probe(m.get(), 0, 2, pl, stats0, false) == Gecode::SS_SOLVED);
   assert(probe(m.get(), 1, 0, pl, stats0, false) == Gecode::SS_BRANCH);
   assert(probe(m.get(), 1, 1, pl, stats0, false) == Gecode::SS_FAILED);
   assert(probe(m.get(), 1, 2, pl, stats0, false) == Gecode::SS_BRANCH);
   assert(probe(m.get(), 2, 0, pl, stats0, false) == Gecode::SS_SOLVED);
   assert(probe(m.get(), 2, 1, pl, stats0, false) == Gecode::SS_BRANCH);
   assert(probe(m.get(), 2, 2, pl, stats0, false) == Gecode::SS_SOLVED);
  }
  {const EncCond enc = encoding("squares A\nls A\n",
     "A\n* * *\n* 1 *\n* * *\n", 3, GC::IPL_VAL);
   const std::unique_ptr<GenericMolsNB> m = space(enc);
   ReductionStatistics stats0(m->V);
   pruning_table_t PT;
   const auto pl = GC::IPL_VAL;
   assert(probe(m.get(), 0,0, pl, PT, stats0, false) == Gecode::SS_SOLVED);
   assert(PT.empty());
   assert(probe(m.get(), 0,1, pl, PT, stats0, false) == Gecode::SS_BRANCH);
   assert(eqp(PT, {{0,1}}));
   assert(probe(m.get(), 0,2, pl, PT, stats0, false) == Gecode::SS_SOLVED);
   assert(eqp(PT, {{0,1}}));
   assert(probe(m.get(), 1,0, pl, PT, stats0, false) == Gecode::SS_BRANCH);
   assert(eqp(PT, {{0,1}, {1,0}, {7,2}}));
   assert(probe(m.get(), 1,1, pl, PT, stats0, false) == Gecode::SS_FAILED);
   assert(eqp(PT, {{0,1}, {1,0}, {7,2}}));
   assert(probe(m.get(), 1,2, pl, PT, stats0, false) == Gecode::SS_BRANCH);
   assert(eqp(PT, {{0,1}, {1,0}, {1,2}, {7,0}, {7,2}}));
   assert(probe(m.get(), 2,0, pl, PT, stats0, false) == Gecode::SS_SOLVED);
   assert(eqp(PT, {{0,1}, {1,0}, {1,2}, {7,0}, {7,2}}));
   assert(probe(m.get(), 2,1, pl, PT, stats0, false) == Gecode::SS_BRANCH);
   assert(eqp(PT, {{0,1}, {1,0}, {1,2}, {2,1}, {7,0}, {7,2}}));
   assert(probe(m.get(), 2,2, pl, PT, stats0, false) == Gecode::SS_SOLVED);
   assert(eqp(PT, {{0,1}, {1,0}, {1,2}, {2,1}, {7,0}, {7,2}}));
  }

  {const CS::Square A(2);
   const std::unique_ptr<GenericMolsNB> m = space(A.enc());
   const auto pl = GC::IPL_VAL;
   const ReductionStatistics stats =
     lareduction<GenericMolsNB>(m.get(), RT::enumerate_solutions, pl,
       LAR::eag_npr);
   assert(stats.props() == 0);
   assert(stats.elimvals() == 0);
   assert(stats.prunes() == 0);
   assert(stats.maxprune() == 0);
   // assert(stats.probes() == 8); XXX needs revision
   // assert(stats.rounds() == 1); XXX needs revision
   assert(stats.solc() == 0);
   assert(stats.leafcount() == 0);
   assert(stats.sollist().empty());
   const std::unique_ptr<GenericMolsNB> m2 = space(A.enc());
   const ReductionStatistics stats2 =
     lareduction<GenericMolsNB>(m2.get(), RT::enumerate_solutions, pl,
       LAR::eag_pr);
   // assert(eqwt(stats2, stats)); YYY
   const std::unique_ptr<GenericMolsNB> m3 = space(A.enc());
   const ReductionStatistics stats3 =
     lareduction<GenericMolsNB>(m3.get(), RT::enumerate_solutions, pl,
       LAR::rel_npr);
   assert(eqwt(stats3, stats));
   const std::unique_ptr<GenericMolsNB> m4 = space(A.enc());
   const ReductionStatistics stats4 =
     lareduction<GenericMolsNB>(m4.get(), RT::enumerate_solutions, pl,
       LAR::rel_pr);
   // assert(eqwt(stats4, stats)); YYY
 }

 {CS::Square A(2);
  const std::unique_ptr<GenericMolsNB> m = space(A.enc());
  const GC::IntPropLevel pl = GC::IPL_VAL;
  const ReductionStatistics stats =
    lareduction<GenericMolsNB>(m.get(), RT::enumerate_solutions, pl,
      LAR::eag_npr);
  // assert(stats.props() == 1); XXX needs revision
  // assert(stats.elimvals() == 2); XXX needs revision
  assert(stats.prunes() == 0);
  assert(stats.maxprune() == 0);
  // assert(stats.probes() == 2); XXX needs revision
  // assert(stats.rounds() == 1); XXX needs revision
   /* needs complete revision XXX
   assert(stats.solc() == 2);
   assert(stats.leafcount() == 1);
   const auto list_sol = extract(enc.ldecode(stats.sollist()));
   assert(eqp(list_sol, {
              {{{0,1},{1,0}}},
              {{{1,0},{0,1}}}}));
   const std::unique_ptr<GenericMolsNB> m2(new GenericMolsNB(enc));
   const ReductionStatistics stats2 =
     lareduction<GenericMolsNB>(m2.get(), RT::enumerate_solutions, pl,
       LAR::eag_pr);
   assert(eqwt(stats2, stats));
   const std::unique_ptr<GenericMolsNB> m3(new GenericMolsNB(enc));
   const ReductionStatistics stats3 =
     lareduction<GenericMolsNB>(m3.get(), RT::enumerate_solutions, pl,
       LAR::rel_npr);
   assert(eqwt(stats3, stats));
   const std::unique_ptr<GenericMolsNB> m4(new GenericMolsNB(enc));
   const ReductionStatistics stats4 =
     lareduction<GenericMolsNB>(m4.get(), RT::enumerate_solutions, pl,
       LAR::rel_pr);
   assert(eqwt(stats4, stats));
   */
  }

  {CS::Square A(3);
   const std::unique_ptr<GenericMolsNB> m = space(A.enc());
   const GC::IntPropLevel pl = GC::IPL_VAL;
   const ReductionStatistics stats =
     lareduction<GenericMolsNB>(m.get(), RT::enumerate_solutions, pl,
       LAR::eag_npr);
   assert(stats.props() == 0);
   assert(stats.elimvals() == 0);
   assert(stats.prunes() == 0);
   assert(stats.maxprune() == 0);
   /* needs complete revision XXX
   assert(stats.probes() == 27);
   assert(stats.rounds() == 1);
   assert(stats.solc() == 0);
   assert(stats.leafcount() == 0);
   assert(stats.sollist().empty());
   */
  }

  {const GC::IntPropLevel pl = GC::IPL_VAL;
   const EncCond enc = encoding("squares A\nls A\n",
     "A\n0 * *\n* * *\n* * *\n", 3, pl);
   const std::unique_ptr<GenericMolsNB> m = space(enc);
   const ReductionStatistics stats =
     lareduction<GenericMolsNB>(m.get(), RT::enumerate_solutions, pl,
       LAR::eag_npr);
   /* ERROR FALSE TESTS
   assert(stats.props() == 1);
   assert(stats.elimvals() == 2);
   assert(stats.prunes() == 0);
   assert(stats.maxprune() == 0);
   assert(stats.probes() == 9);
   assert(stats.rounds() == 1);
   assert(stats.solc() == 2);
   assert(stats.leafcount() == 0);
   const auto list_sol = extract(enc.ldecode(stats.sollist()));
   assert(eqp(list_sol, {
              {{{0,2,1},{2,1,0},{1,0,2}}},
              {{{0,1,2},{1,2,0},{2,0,1}}}}));
   */
  }

  {const GC::IntPropLevel pl = GC::IPL_VAL;
   const EncCond enc = encoding("squares A\nls A\n",
     "A\n* * *\n* 1 *\n* * *\n", 3, pl);
   const std::unique_ptr<GenericMolsNB> m = space(enc);
   const ReductionStatistics stats =
     lareduction<GenericMolsNB>(m.get(), RT::enumerate_solutions, pl,
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

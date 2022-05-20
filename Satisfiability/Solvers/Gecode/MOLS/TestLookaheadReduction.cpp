// Oleg Zaikin, 29.3.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

Testing of look-ahead reduction for the Gecode library.

BUG:

1. Wrong tests for eager reduction
    - If a reduction was found, then obviously at least two rounds are needed.

2. A complete revision regarding rounds etc. is needed.

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

1, Clear structure of tests
    - A test should always test *one thing*.
    - That means, exactly *one* of "child_node" or "probe" or "lareduction".
    - The current copy-and-pasting of some "child_node" and some "probe" seems
      to make little sense (not much care is spent on each candidate to be
      tested, and there are interferences).
    - The order of tests usually should follow the order in the code to be
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

namespace {

  const Environment::ProgramInfo proginfo{
        "0.4.1",
        "20.5.2022",
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

  template <class X>
  constexpr bool eqp(const X& lhs, const X& rhs) noexcept {
    return lhs == rhs;
  }

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  {// An empty square or order 2, four variables, each with domain {0,1}:
   const GC::IntPropLevel pl = GC::IPL_VAL;
   const EncCond enc = encoding("squares A\n", "", 2, pl);
   const std::unique_ptr<GenericMolsNB> m(new GenericMolsNB(enc));
   assert(m->status() == Gecode::SS_BRANCH);
   assert(m->V.size() == 4);
   assert(assignedvars(m->V) == 0);
   assert(sumdomsizes(m->V) == 8);
   const auto ch = child_node<GenericMolsNB>(m.get(), 0, 0, pl, true);
   assert(ch->status() == Gecode::SS_BRANCH);
   assert(ch->V.size() == m->V.size());
   assert(assignedvars(ch->V) == 1);
   assert(sumdomsizes(ch->V) == 7);
   assert(assignedval(ch->V, 0) == 0);
   const auto ch2 = child_node<GenericMolsNB>(m.get(), 0, 0, pl, false);
   assert(ch2->status() == Gecode::SS_BRANCH);
   assert(ch2->V.size() == m->V.size());
   assert(assignedvars(ch2->V) == 1);
   assert(sumdomsizes(ch2->V) == 7);
   assert(assignedval(ch2->V, 0) == 1);
   ReductionStatistics stats0(m->V);
   assert(probe(m.get(), 0, 0, pl, stats0, false) == Gecode::SS_BRANCH);
   assert(probe(m.get(), 0, 1, pl, stats0, false) == Gecode::SS_BRANCH);
   assert(probe(m.get(), 1, 0, pl, stats0, false) == Gecode::SS_BRANCH);
   assert(probe(m.get(), 1, 1, pl, stats0, false) == Gecode::SS_BRANCH);
   pruning_table_t PT;
   assert(probe(m.get(), 0, 0, pl, PT, stats0, false) == Gecode::SS_BRANCH);
   assert(eqp(PT, {{0,0}}));
   assert(probe(m.get(), 0, 1, pl, PT, stats0, false) == Gecode::SS_BRANCH);
   assert(eqp(PT, {{0,0}, {0,1}}));
   assert(probe(m.get(), 1, 0, pl, PT, stats0, false) == Gecode::SS_BRANCH);
   assert(eqp(PT, {{0,0}, {0,1}, {1,0}}));
   assert(probe(m.get(), 1, 1, pl, PT, stats0, false) == Gecode::SS_BRANCH);
   assert(eqp(PT, {{0,0}, {0,1}, {1,0}, {1,1}}));
   assert(assignedvars(m->V) == 0);
   assert(sumdomsizes(m->V) == 8);
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
  }

  {// All Latin square of order 2:
   const GC::IntPropLevel pl = GC::IPL_VAL;
   const EncCond enc = encoding("squares A\nls A\n", "", 2, pl);
   const std::unique_ptr<GenericMolsNB> m(new GenericMolsNB(enc));
   assert(m->status() == Gecode::SS_BRANCH);
   assert(m->V.size() == 4);
   assert(assignedvars(m->V) == 0);
   assert(sumdomsizes(m->V) == 8);
   const auto ch = child_node<GenericMolsNB>(m.get(), 0, 0, pl, true);
   assert(ch->status() == Gecode::SS_SOLVED);
   assert(ch->V.size() == m->V.size());
   assert(assignedvars(ch->V) == 4);
   assert(sumdomsizes(ch->V) == 4);
   assert(assignedval(ch->V,0) == 0);
   const auto ch2 = child_node<GenericMolsNB>(m.get(), 0, 0, pl, false);
   assert(ch2->status() == Gecode::SS_SOLVED);
   assert(ch2->V.size() == m->V.size());
   assert(assignedvars(ch2->V) == 4);
   assert(sumdomsizes(ch2->V) == 4);
   assert(assignedval(ch2->V,0) == 1);
   ReductionStatistics stats0(m->V);
   assert(probe(m.get(), 0, 0, pl, stats0, false) == Gecode::SS_SOLVED);
   assert(probe(m.get(), 0, 1, pl, stats0, false) == Gecode::SS_SOLVED);
   assert(probe(m.get(), 1, 0, pl, stats0, false) == Gecode::SS_SOLVED);
   assert(probe(m.get(), 1, 1, pl, stats0, false) == Gecode::SS_SOLVED);
   pruning_table_t PT;
   assert(probe(m.get(), 0, 0, pl, PT, stats0, false) == Gecode::SS_SOLVED);
   assert(PT.empty());
   assert(probe(m.get(), 0, 1, pl, PT, stats0, false) == Gecode::SS_SOLVED);
   assert(PT.empty());
   assert(probe(m.get(), 1, 0, pl, PT, stats0, false) == Gecode::SS_SOLVED);
   assert(PT.empty());
   assert(probe(m.get(), 1, 1, pl, PT, stats0, false) == Gecode::SS_SOLVED);
   assert(PT.empty());
   assert(assignedvars(m->V) == 0);
   assert(sumdomsizes(m->V) == 8);
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

  {// An empty square of order 3:
   const GC::IntPropLevel pl = GC::IPL_VAL;
   const EncCond enc = encoding("squares A\n", "", 3, pl);
   const std::unique_ptr<GenericMolsNB> m(new GenericMolsNB(enc));
   assert(m->status() == Gecode::SS_BRANCH);
   assert(m->V.size() == 9);
   assert(assignedvars(m->V) == 0);
   assert(sumdomsizes(m->V) == 27);
   const auto ch = child_node<GenericMolsNB>(m.get(), 0, 0, pl, true);
   assert(ch->status() == Gecode::SS_BRANCH);
   assert(ch->V.size() == m->V.size());
   assert(assignedvars(ch->V) == 1);
   assert(sumdomsizes(ch->V) == 25);
   assert(assignedval(ch->V,0) == 0);
   const auto ch2 = child_node<GenericMolsNB>(m.get(), 0, 0, pl, false);
   assert(ch2->status() == Gecode::SS_BRANCH);
   assert(ch2->V.size() == m->V.size());
   assert(assignedvars(ch2->V) == 0);
   assert(sumdomsizes(ch2->V) == 26);
   assert(ch2->V[0].size() == 2);
   ReductionStatistics stats0(m->V);
   assert(probe(m.get(), 0, 0, pl, stats0, false) == Gecode::SS_BRANCH);
   assert(probe(m.get(), 0, 1, pl, stats0, false) == Gecode::SS_BRANCH);
   assert(probe(m.get(), 0, 2, pl, stats0, false) == Gecode::SS_BRANCH);
   assert(probe(m.get(), 1, 0, pl, stats0, false) == Gecode::SS_BRANCH);
   assert(probe(m.get(), 1, 1, pl, stats0, false) == Gecode::SS_BRANCH);
   assert(probe(m.get(), 1, 2, pl, stats0, false) == Gecode::SS_BRANCH);
   assert(probe(m.get(), 2, 0, pl, stats0, false) == Gecode::SS_BRANCH);
   assert(probe(m.get(), 2, 1, pl, stats0, false) == Gecode::SS_BRANCH);
   assert(probe(m.get(), 2, 2, pl, stats0, false) == Gecode::SS_BRANCH);
   pruning_table_t PT;
   assert(probe(m.get(), 0, 0, pl, PT, stats0, false) == Gecode::SS_BRANCH);
   assert(eqp(PT, {{0,0}}));
   assert(probe(m.get(), 0, 1, pl, PT, stats0, false) == Gecode::SS_BRANCH);
   assert(eqp(PT, {{0,0}, {0,1}}));
   assert(probe(m.get(), 0, 2, pl, PT, stats0, false) == Gecode::SS_BRANCH);
   assert(eqp(PT, {{0,0}, {0,1}, {0,2}}));
   assert(probe(m.get(), 1, 0, pl, PT, stats0, false) == Gecode::SS_BRANCH);
   assert(eqp(PT, {{0,0}, {0,1}, {0,2}, {1,0}}));
   assert(probe(m.get(), 1, 1, pl, PT, stats0, false) == Gecode::SS_BRANCH);
   assert(eqp(PT, {{0,0}, {0,1}, {0,2}, {1,0}, {1,1}}));
   assert(probe(m.get(), 1, 2, pl, PT, stats0, false) == Gecode::SS_BRANCH);
   assert(eqp(PT, {{0,0}, {0,1}, {0,2}, {1,0}, {1,1}, {1,2}}));
   assert(probe(m.get(), 2, 0, pl, PT, stats0, false) == Gecode::SS_BRANCH);
   assert(eqp(PT, {{0,0}, {0,1}, {0,2}, {1,0}, {1,1}, {1,2}, {2,0}}));
   assert(probe(m.get(), 2, 1, pl, PT, stats0, false) == Gecode::SS_BRANCH);
   assert(eqp(PT, {{0,0}, {0,1}, {0,2}, {1,0}, {1,1}, {1,2}, {2,0}, {2,1}}));
   assert(probe(m.get(), 2, 2, pl, PT, stats0, false) == Gecode::SS_BRANCH);
   assert(eqp(PT, {{0,0}, {0,1}, {0,2}, {1,0}, {1,1}, {1,2}, {2,0}, {2,1}, {2,2}}));
   assert(assignedvars(m->V) == 0);
   assert(sumdomsizes(m->V) == 27);
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

  {// A Latin square of order 3 with A[0,0] == 0:
   const GC::IntPropLevel pl = GC::IPL_VAL;
   const EncCond enc = encoding("squares A\nls A\n",
     "A\n0 * *\n* * *\n* * *\n", 3, pl);
   const std::unique_ptr<GenericMolsNB> m(new GenericMolsNB(enc));
   assert(m->status() == Gecode::SS_BRANCH);
   assert(m->V.size() == 9);
   assert(assignedvars(m->V) == 1);
   assert(sumdomsizes(m->V) == 21);
   assert(assignedval(m->V,0) == 0);
   const auto ch = child_node<GenericMolsNB>(m.get(), 0, 0, pl, true);
   assert(ch->status() == Gecode::SS_BRANCH);
   assert(ch->V.size() == m->V.size());
   assert(assignedvars(ch->V) == 1);
   assert(sumdomsizes(ch->V) == 21);
   assert(assignedval(ch->V,0) == 0);
   const auto ch2 = child_node<GenericMolsNB>(m.get(), 0, 0, pl, false);
   assert(ch2->status() == Gecode::SS_FAILED);
   assert(ch2->V.size() == m->V.size());
   assert(assignedvars(ch2->V) == 1);
   assert(sumdomsizes(ch2->V) == 21);
   assert(assignedval(ch2->V,0) == 0);
   ReductionStatistics stats0(m->V);
   assert(probe(m.get(), 0, 0, pl, stats0, false) == Gecode::SS_BRANCH);
   assert(probe(m.get(), 0, 1, pl, stats0, false) == Gecode::SS_FAILED);
   assert(probe(m.get(), 0, 2, pl, stats0, false) == Gecode::SS_FAILED);
   assert(probe(m.get(), 1, 0, pl, stats0, false) == Gecode::SS_FAILED);
   assert(probe(m.get(), 1, 1, pl, stats0, false) == Gecode::SS_BRANCH);
   assert(probe(m.get(), 1, 2, pl, stats0, false) == Gecode::SS_BRANCH);
   assert(probe(m.get(), 2, 0, pl, stats0, false) == Gecode::SS_FAILED);
   assert(probe(m.get(), 2, 1, pl, stats0, false) == Gecode::SS_BRANCH);
   assert(probe(m.get(), 2, 2, pl, stats0, false) == Gecode::SS_BRANCH);
   pruning_table_t PT;
   assert(probe(m.get(), 0, 0, pl, PT, stats0, false) == Gecode::SS_BRANCH);
   assert(PT.empty());
   assert(probe(m.get(), 0, 1, pl, PT, stats0, false) == Gecode::SS_FAILED);
   assert(PT.empty());
   assert(probe(m.get(), 0, 2, pl, PT, stats0, false) == Gecode::SS_FAILED);
   assert(PT.empty());
   assert(probe(m.get(), 1, 0, pl, PT, stats0, false) == Gecode::SS_FAILED);
   assert(PT.empty());
   assert(probe(m.get(), 1, 1, pl, PT, stats0, false) == Gecode::SS_BRANCH);
   assert(eqp(PT, {{1,1}, {2,2}}));
   assert(probe(m.get(), 1, 2, pl, PT, stats0, false) == Gecode::SS_BRANCH);
   assert(eqp(PT, {{1,1}, {1,2}, {2,1}, {2,2}}));
   assert(probe(m.get(), 2, 0, pl, PT, stats0, false) == Gecode::SS_FAILED);
   assert(eqp(PT, {{1,1}, {1,2}, {2,1}, {2,2}}));
   assert(probe(m.get(), 2, 1, pl, PT, stats0, false) == Gecode::SS_BRANCH);
   assert(eqp(PT, {{1,1}, {1,2}, {2,1}, {2,2}}));
   assert(probe(m.get(), 2, 2, pl, PT, stats0, false) == Gecode::SS_BRANCH);
   assert(eqp(PT, {{1,1}, {1,2}, {2,1}, {2,2}}));
   assert(assignedvars(m->V) == 1);
   assert(sumdomsizes(m->V) == 21);
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

  {// A Latin square of order 3 with A[1,1] == 1:
   const GC::IntPropLevel pl = GC::IPL_VAL;
   const EncCond enc = encoding("squares A\nls A\n",
     "A\n* * *\n* 1 *\n* * *\n", 3, pl);
   const std::unique_ptr<GenericMolsNB> m(new GenericMolsNB(enc));
   assert(m->status() == Gecode::SS_BRANCH);
   assert(m->V.size() == 9);
   assert(assignedvars(m->V) == 1);
   assert(sumdomsizes(m->V) == 21);
   assert(assignedval(m->V,4) == 1);
   const auto ch = child_node<GenericMolsNB>(m.get(), 0, 0, pl, true);
   assert(ch->status() == Gecode::SS_SOLVED);
   assert(ch->V.size() == m->V.size());
   assert(assignedvars(ch->V) == 9);
   assert(sumdomsizes(ch->V) == 9);
   assert(assignedval(ch->V,0) == 0);
   assert(assignedval(ch->V,4) == 1);
   const auto ch2 = child_node<GenericMolsNB>(m.get(), 0, 0, pl, false);
   assert(ch2->status() == Gecode::SS_BRANCH);
   assert(ch2->V.size() == m->V.size());
   assert(assignedvars(ch2->V) == 1);
   assert(sumdomsizes(ch2->V) == 20);
   assert(assignedval(ch2->V,4) == 1);
   ReductionStatistics stats0(m->V);
   assert(probe(m.get(), 0, 0, pl, stats0, false) == Gecode::SS_SOLVED);
   assert(probe(m.get(), 0, 1, pl, stats0, false) == Gecode::SS_BRANCH);
   assert(probe(m.get(), 0, 2, pl, stats0, false) == Gecode::SS_SOLVED);
   assert(probe(m.get(), 1, 0, pl, stats0, false) == Gecode::SS_BRANCH);
   assert(probe(m.get(), 1, 1, pl, stats0, false) == Gecode::SS_FAILED);
   assert(probe(m.get(), 1, 2, pl, stats0, false) == Gecode::SS_BRANCH);
   assert(probe(m.get(), 2, 0, pl, stats0, false) == Gecode::SS_SOLVED);
   assert(probe(m.get(), 2, 1, pl, stats0, false) == Gecode::SS_BRANCH);
   assert(probe(m.get(), 2, 2, pl, stats0, false) == Gecode::SS_SOLVED);
   pruning_table_t PT;
   assert(probe(m.get(), 0, 0, pl, PT, stats0, false) == Gecode::SS_SOLVED);
   assert(PT.empty());
   assert(probe(m.get(), 0, 1, pl, PT, stats0, false) == Gecode::SS_BRANCH);
   assert(eqp(PT, {{0,1}}));
   assert(probe(m.get(), 0, 2, pl, PT, stats0, false) == Gecode::SS_SOLVED);
   assert(eqp(PT, {{0,1}}));
   assert(probe(m.get(), 1, 0, pl, PT, stats0, false) == Gecode::SS_BRANCH);
   assert(eqp(PT, {{0,1}, {1,0}, {7,2}}));
   assert(probe(m.get(), 1, 1, pl, PT, stats0, false) == Gecode::SS_FAILED);
   assert(eqp(PT, {{0,1}, {1,0}, {7,2}}));
   assert(probe(m.get(), 1, 2, pl, PT, stats0, false) == Gecode::SS_BRANCH);
   assert(eqp(PT, {{0,1}, {1,0}, {1,2}, {7,0}, {7,2}}));
   assert(probe(m.get(), 2, 0, pl, PT, stats0, false) == Gecode::SS_SOLVED);
   assert(eqp(PT, {{0,1}, {1,0}, {1,2}, {7,0}, {7,2}}));
   assert(probe(m.get(), 2, 1, pl, PT, stats0, false) == Gecode::SS_BRANCH);
   assert(eqp(PT, {{0,1}, {1,0}, {1,2}, {2,1}, {7,0}, {7,2}}));
   assert(probe(m.get(), 2, 2, pl, PT, stats0, false) == Gecode::SS_SOLVED);
   assert(eqp(PT, {{0,1}, {1,0}, {1,2}, {2,1}, {7,0}, {7,2}}));
   assert(assignedvars(m->V) == 1);
   assert(sumdomsizes(m->V) == 21);
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

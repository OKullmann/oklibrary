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

TODOS:

-1.DONE (lareduction is tested, including satysfying assignments)
   Testing lareduction is URGENTLY needed:
  - DONE The space provided should be just GenericMols0.
  - DONE More precisely, a class say "GenericMolsNB" should be derived from
    GenericMols0, with "NB" for "no branching", which posts a brancher
    which immediately throws an exception -- this class is only to be
    used for testing.
  - DONE The functions are to be tested exactly as they are.
  - DONE The parameters of lareduction are *exactly* appropriate -- global
    variables must be avoided (and class-variables are just global variables).

0. DONE (enumeration-modes are used)
   Tests should mostly used enumeration-modes:
  - DONE So that also the satisfying assignments can be tested.
  - DONE The various modes of function probe need to be tested.

*/

#include <iostream>
#include <sstream>
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
        "0.3.7",
        "17.5.2022",
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

  template <class X>
  constexpr bool eqp(const X& lhs, const X& rhs) noexcept {
    return lhs == rhs;
  }

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  {// An empty square or order 2, four variables, each with domain {0,1}:
   std::istringstream in_cond("squares A\n");
   std::istringstream in_ps("");
   const AConditions ac = ReadAC()(in_cond);
   const PSquares ps = PSquares(2, in_ps);
   const GC::IntPropLevel pl = GC::IPL_VAL;
   const EncCond enc(ac, ps, pl);
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
   ReductionStatistics stat0(m->V);
   assert(probe(m.get(), 0, 0, pl, stat0, false) == Gecode::SS_BRANCH);
   assert(probe(m.get(), 0, 1, pl, stat0, false) == Gecode::SS_BRANCH);
   assert(probe(m.get(), 1, 0, pl, stat0, false) == Gecode::SS_BRANCH);
   assert(probe(m.get(), 1, 1, pl, stat0, false) == Gecode::SS_BRANCH);
   pruning_table_t PT;
   assert(probe(m.get(), 0, 0, pl, PT, stat0, false) == Gecode::SS_BRANCH);
   assert(eqp(PT, {{0,0}}));
   assert(probe(m.get(), 0, 1, pl, PT, stat0, false) == Gecode::SS_BRANCH);
   assert(eqp(PT, {{0,0}, {0,1}}));
   assert(probe(m.get(), 1, 0, pl, PT, stat0, false) == Gecode::SS_BRANCH);
   assert(eqp(PT, {{0,0}, {0,1}, {1,0}}));
   assert(probe(m.get(), 1, 1, pl, PT, stat0, false) == Gecode::SS_BRANCH);
   assert(eqp(PT, {{0,0}, {0,1}, {1,0}, {1,1}}));
   assert(assignedvars(m->V) == 0);
   assert(sumdomsizes(m->V) == 8);
   const ReductionStatistics stat =
     lareduction<GenericMolsNB>(m.get(), RT::enumerate_solutions, pl,
       LAR::eag_npr);
   assert(stat.props() == 0);
   assert(stat.elimvals() == 0);
   assert(stat.prunes() == 0);
   assert(stat.maxprune() == 0);
   assert(stat.probes() == 8);
   assert(stat.rounds() == 1);
   assert(stat.solc() == 0);
   assert(stat.leafcount() == 0);
   assert(stat.sollist().empty());
   const std::unique_ptr<GenericMolsNB> m2(new GenericMolsNB(enc));
   const ReductionStatistics stat2 =
     lareduction<GenericMolsNB>(m2.get(), RT::enumerate_solutions, pl,
       LAR::eag_pr);
   assert(eqwt(stat2, stat));
   const std::unique_ptr<GenericMolsNB> m3(new GenericMolsNB(enc));
   const ReductionStatistics stat3 =
     lareduction<GenericMolsNB>(m3.get(), RT::enumerate_solutions, pl,
       LAR::rel_npr);
   assert(eqwt(stat3, stat));
   const std::unique_ptr<GenericMolsNB> m4(new GenericMolsNB(enc));
   const ReductionStatistics stat4 =
     lareduction<GenericMolsNB>(m4.get(), RT::enumerate_solutions, pl,
       LAR::rel_pr);
   assert(eqwt(stat4, stat));
  }

  {// All Latin square of order 2:
   std::istringstream in_cond("squares A\nls A\n");
   std::istringstream in_ps("");
   const AConditions ac = ReadAC()(in_cond);
   const PSquares ps = PSquares(2, in_ps);
   const GC::IntPropLevel pl = GC::IPL_VAL;
   const EncCond enc(ac, ps, pl);
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
   ReductionStatistics stat0(m->V);
   assert(probe(m.get(), 0, 0, pl, stat0, false) == Gecode::SS_SOLVED);
   assert(probe(m.get(), 0, 1, pl, stat0, false) == Gecode::SS_SOLVED);
   assert(probe(m.get(), 1, 0, pl, stat0, false) == Gecode::SS_SOLVED);
   assert(probe(m.get(), 1, 1, pl, stat0, false) == Gecode::SS_SOLVED);
   pruning_table_t PT;
   assert(probe(m.get(), 0, 0, pl, PT, stat0, false) == Gecode::SS_SOLVED);
   assert(PT.empty());
   assert(probe(m.get(), 0, 1, pl, PT, stat0, false) == Gecode::SS_SOLVED);
   assert(PT.empty());
   assert(probe(m.get(), 1, 0, pl, PT, stat0, false) == Gecode::SS_SOLVED);
   assert(PT.empty());
   assert(probe(m.get(), 1, 1, pl, PT, stat0, false) == Gecode::SS_SOLVED);
   assert(PT.empty());
   assert(assignedvars(m->V) == 0);
   assert(sumdomsizes(m->V) == 8);
   const ReductionStatistics stat =
     lareduction<GenericMolsNB>(m.get(), RT::enumerate_solutions, pl,
       LAR::eag_npr);
   assert(stat.props() == 1);
   assert(stat.elimvals() == 2);
   assert(stat.prunes() == 0);
   assert(stat.maxprune() == 0);
   assert(stat.probes() == 2);
   assert(stat.rounds() == 1);
   assert(stat.solc() == 2);
   assert(stat.leafcount() == 1);
   const auto list_sol = extract(enc.ldecode(stat.sollist()));
   assert(eqp(list_sol, {
              {{{0,1},{1,0}}},
              {{{1,0},{0,1}}}}));
   const std::unique_ptr<GenericMolsNB> m2(new GenericMolsNB(enc));
   const ReductionStatistics stat2 =
     lareduction<GenericMolsNB>(m2.get(), RT::enumerate_solutions, pl,
       LAR::eag_pr);
   assert(eqwt(stat2, stat));
   const std::unique_ptr<GenericMolsNB> m3(new GenericMolsNB(enc));
   const ReductionStatistics stat3 =
     lareduction<GenericMolsNB>(m3.get(), RT::enumerate_solutions, pl,
       LAR::rel_npr);
   assert(eqwt(stat3, stat));
   const std::unique_ptr<GenericMolsNB> m4(new GenericMolsNB(enc));
   const ReductionStatistics stat4 =
     lareduction<GenericMolsNB>(m4.get(), RT::enumerate_solutions, pl,
       LAR::rel_pr);
   assert(eqwt(stat4, stat));
  }

  {// An empty square of order 3:
   std::istringstream in_cond("squares A\n");
   std::istringstream in_ps("");
   const AConditions ac = ReadAC()(in_cond);
   const PSquares ps = PSquares(3, in_ps);
   const GC::IntPropLevel pl = GC::IPL_VAL;
   const EncCond enc(ac, ps, pl);
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
   ReductionStatistics stat0(m->V);
   assert(probe(m.get(), 0, 0, pl, stat0, false) == Gecode::SS_BRANCH);
   assert(probe(m.get(), 0, 1, pl, stat0, false) == Gecode::SS_BRANCH);
   assert(probe(m.get(), 0, 2, pl, stat0, false) == Gecode::SS_BRANCH);
   assert(probe(m.get(), 1, 0, pl, stat0, false) == Gecode::SS_BRANCH);
   assert(probe(m.get(), 1, 1, pl, stat0, false) == Gecode::SS_BRANCH);
   assert(probe(m.get(), 1, 2, pl, stat0, false) == Gecode::SS_BRANCH);
   assert(probe(m.get(), 2, 0, pl, stat0, false) == Gecode::SS_BRANCH);
   assert(probe(m.get(), 2, 1, pl, stat0, false) == Gecode::SS_BRANCH);
   assert(probe(m.get(), 2, 2, pl, stat0, false) == Gecode::SS_BRANCH);
   pruning_table_t PT;
   assert(probe(m.get(), 0, 0, pl, PT, stat0, false) == Gecode::SS_BRANCH);
   assert(eqp(PT, {{0,0}}));
   assert(probe(m.get(), 0, 1, pl, PT, stat0, false) == Gecode::SS_BRANCH);
   assert(eqp(PT, {{0,0}, {0,1}}));
   assert(probe(m.get(), 0, 2, pl, PT, stat0, false) == Gecode::SS_BRANCH);
   assert(eqp(PT, {{0,0}, {0,1}, {0,2}}));
   assert(probe(m.get(), 1, 0, pl, PT, stat0, false) == Gecode::SS_BRANCH);
   assert(eqp(PT, {{0,0}, {0,1}, {0,2}, {1,0}}));
   assert(probe(m.get(), 1, 1, pl, PT, stat0, false) == Gecode::SS_BRANCH);
   assert(eqp(PT, {{0,0}, {0,1}, {0,2}, {1,0}, {1,1}}));
   assert(probe(m.get(), 1, 2, pl, PT, stat0, false) == Gecode::SS_BRANCH);
   assert(eqp(PT, {{0,0}, {0,1}, {0,2}, {1,0}, {1,1}, {1,2}}));
   assert(probe(m.get(), 2, 0, pl, PT, stat0, false) == Gecode::SS_BRANCH);
   assert(eqp(PT, {{0,0}, {0,1}, {0,2}, {1,0}, {1,1}, {1,2}, {2,0}}));
   assert(probe(m.get(), 2, 1, pl, PT, stat0, false) == Gecode::SS_BRANCH);
   assert(eqp(PT, {{0,0}, {0,1}, {0,2}, {1,0}, {1,1}, {1,2}, {2,0}, {2,1}}));
   assert(probe(m.get(), 2, 2, pl, PT, stat0, false) == Gecode::SS_BRANCH);
   assert(eqp(PT, {{0,0}, {0,1}, {0,2}, {1,0}, {1,1}, {1,2}, {2,0}, {2,1}, {2,2}}));
   assert(assignedvars(m->V) == 0);
   assert(sumdomsizes(m->V) == 27);
   const ReductionStatistics stat =
     lareduction<GenericMolsNB>(m.get(), RT::enumerate_solutions, pl,
       LAR::eag_npr);
   assert(stat.props() == 0);
   assert(stat.elimvals() == 0);
   assert(stat.prunes() == 0);
   assert(stat.maxprune() == 0);
   assert(stat.probes() == 27);
   assert(stat.rounds() == 1);
   assert(stat.solc() == 0);
   assert(stat.leafcount() == 0);
   assert(stat.sollist().empty());
  }

  {// A Latin square of order 3 with A[0,0] == 0:
   std::istringstream in_cond("squares A\nls A\n");
   std::istringstream in_ps("A\n0 * *\n* * *\n* * *\n");
   const AConditions ac = ReadAC()(in_cond);
   const PSquares ps = PSquares(3, in_ps);
   const GC::IntPropLevel pl = GC::IPL_VAL;
   const EncCond enc(ac, ps, pl);
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
   ReductionStatistics stat0(m->V);
   assert(probe(m.get(), 0, 0, pl, stat0, false) == Gecode::SS_BRANCH);
   assert(probe(m.get(), 0, 1, pl, stat0, false) == Gecode::SS_FAILED);
   assert(probe(m.get(), 0, 2, pl, stat0, false) == Gecode::SS_FAILED);
   assert(probe(m.get(), 1, 0, pl, stat0, false) == Gecode::SS_FAILED);
   assert(probe(m.get(), 1, 1, pl, stat0, false) == Gecode::SS_BRANCH);
   assert(probe(m.get(), 1, 2, pl, stat0, false) == Gecode::SS_BRANCH);
   assert(probe(m.get(), 2, 0, pl, stat0, false) == Gecode::SS_FAILED);
   assert(probe(m.get(), 2, 1, pl, stat0, false) == Gecode::SS_BRANCH);
   assert(probe(m.get(), 2, 2, pl, stat0, false) == Gecode::SS_BRANCH);
   pruning_table_t PT;
   assert(probe(m.get(), 0, 0, pl, PT, stat0, false) == Gecode::SS_BRANCH);
   assert(PT.empty());
   assert(probe(m.get(), 0, 1, pl, PT, stat0, false) == Gecode::SS_FAILED);
   assert(PT.empty());
   assert(probe(m.get(), 0, 2, pl, PT, stat0, false) == Gecode::SS_FAILED);
   assert(PT.empty());
   assert(probe(m.get(), 1, 0, pl, PT, stat0, false) == Gecode::SS_FAILED);
   assert(PT.empty());
   assert(probe(m.get(), 1, 1, pl, PT, stat0, false) == Gecode::SS_BRANCH);
   assert(eqp(PT, {{1,1}, {2,2}}));
   assert(probe(m.get(), 1, 2, pl, PT, stat0, false) == Gecode::SS_BRANCH);
   assert(eqp(PT, {{1,1}, {1,2}, {2,1}, {2,2}}));
   assert(probe(m.get(), 2, 0, pl, PT, stat0, false) == Gecode::SS_FAILED);
   assert(eqp(PT, {{1,1}, {1,2}, {2,1}, {2,2}}));
   assert(probe(m.get(), 2, 1, pl, PT, stat0, false) == Gecode::SS_BRANCH);
   assert(eqp(PT, {{1,1}, {1,2}, {2,1}, {2,2}}));
   assert(probe(m.get(), 2, 2, pl, PT, stat0, false) == Gecode::SS_BRANCH);
   assert(eqp(PT, {{1,1}, {1,2}, {2,1}, {2,2}}));
   assert(assignedvars(m->V) == 1);
   assert(sumdomsizes(m->V) == 21);
   const ReductionStatistics stat =
     lareduction<GenericMolsNB>(m.get(), RT::enumerate_solutions, pl,
       LAR::eag_npr);
   /* ERROR FALSE TESTS
   assert(stat.props() == 1);
   assert(stat.elimvals() == 2);
   assert(stat.prunes() == 0);
   assert(stat.maxprune() == 0);
   assert(stat.probes() == 9);
   assert(stat.rounds() == 1);
   assert(stat.solc() == 2);
   assert(stat.leafcount() == 0);
   const auto list_sol = extract(enc.ldecode(stat.sollist()));
   assert(eqp(list_sol, {
              {{{0,2,1},{2,1,0},{1,0,2}}},
              {{{0,1,2},{1,2,0},{2,0,1}}}}));
   */
  }

  {// A Latin square of order 3 with A[1,1] == 1:
   std::istringstream in_cond("squares A\nls A\n");
   std::istringstream in_ps("A\n* * *\n* 1 *\n* * *\n");
   const AConditions ac = ReadAC()(in_cond);
   const PSquares ps = PSquares(3, in_ps);
   const GC::IntPropLevel pl = GC::IPL_VAL;
   const EncCond enc(ac, ps, pl);
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
   ReductionStatistics stat0(m->V);
   assert(probe(m.get(), 0, 0, pl, stat0, false) == Gecode::SS_SOLVED);
   assert(probe(m.get(), 0, 1, pl, stat0, false) == Gecode::SS_BRANCH);
   assert(probe(m.get(), 0, 2, pl, stat0, false) == Gecode::SS_SOLVED);
   assert(probe(m.get(), 1, 0, pl, stat0, false) == Gecode::SS_BRANCH);
   assert(probe(m.get(), 1, 1, pl, stat0, false) == Gecode::SS_FAILED);
   assert(probe(m.get(), 1, 2, pl, stat0, false) == Gecode::SS_BRANCH);
   assert(probe(m.get(), 2, 0, pl, stat0, false) == Gecode::SS_SOLVED);
   assert(probe(m.get(), 2, 1, pl, stat0, false) == Gecode::SS_BRANCH);
   assert(probe(m.get(), 2, 2, pl, stat0, false) == Gecode::SS_SOLVED);
   pruning_table_t PT;
   assert(probe(m.get(), 0, 0, pl, PT, stat0, false) == Gecode::SS_SOLVED);
   assert(PT.empty());
   assert(probe(m.get(), 0, 1, pl, PT, stat0, false) == Gecode::SS_BRANCH);
   assert(eqp(PT, {{0,1}}));
   assert(probe(m.get(), 0, 2, pl, PT, stat0, false) == Gecode::SS_SOLVED);
   assert(eqp(PT, {{0,1}}));
   assert(probe(m.get(), 1, 0, pl, PT, stat0, false) == Gecode::SS_BRANCH);
   assert(eqp(PT, {{0,1}, {1,0}, {7,2}}));
   assert(probe(m.get(), 1, 1, pl, PT, stat0, false) == Gecode::SS_FAILED);
   assert(eqp(PT, {{0,1}, {1,0}, {7,2}}));
   assert(probe(m.get(), 1, 2, pl, PT, stat0, false) == Gecode::SS_BRANCH);
   assert(eqp(PT, {{0,1}, {1,0}, {1,2}, {7,0}, {7,2}}));
   assert(probe(m.get(), 2, 0, pl, PT, stat0, false) == Gecode::SS_SOLVED);
   assert(eqp(PT, {{0,1}, {1,0}, {1,2}, {7,0}, {7,2}}));
   assert(probe(m.get(), 2, 1, pl, PT, stat0, false) == Gecode::SS_BRANCH);
   assert(eqp(PT, {{0,1}, {1,0}, {1,2}, {2,1}, {7,0}, {7,2}}));
   assert(probe(m.get(), 2, 2, pl, PT, stat0, false) == Gecode::SS_SOLVED);
   assert(eqp(PT, {{0,1}, {1,0}, {1,2}, {2,1}, {7,0}, {7,2}}));
   assert(assignedvars(m->V) == 1);
   assert(sumdomsizes(m->V) == 21);
   const ReductionStatistics stat =
     lareduction<GenericMolsNB>(m.get(), RT::enumerate_solutions, pl,
       LAR::eag_npr);
   /* ERROR FALSE TESTS
   assert(stat.props() == 1);
   assert(stat.elimvals() == 2);
   assert(stat.prunes() == 0);
   assert(stat.maxprune() == 0);
   assert(stat.probes() == 3);
   assert(stat.rounds() == 1);
   assert(stat.solc() == 2);
   assert(stat.leafcount() == 0);
   const auto list_sol = extract(enc.ldecode(stat.sollist()));
   assert(eqp(list_sol, {
              {{{0,2,1},{2,1,0},{1,0,2}}},
              {{{2,0,1},{0,1,2},{1,2,0}}}}));
   */
  }

}

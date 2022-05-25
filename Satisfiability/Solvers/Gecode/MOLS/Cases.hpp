// Oleg Zaikin, 23.5.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

Test cases for lookahead-reduction and solvers.

BUG:

TODOS:

0. Add cases needed for testing lookahead-reduction.
  - DONE Square.
  - DONE Trivial (empty) Latin square.
  - Reduced variations.
  - Row latin squares.

1. Providing the number of solutions when a formula is not known.
  - For a square, there is a simple formula for the number of solutions.
  - For a Latin square, there is no such formula.
  - For now no need to consider such complicated cases -- concentrating
    on simple variations, as in TestEncoding.cpp.

2. Statistics for lookahead-reduction.
  - Add for each case a function that returns ReductionStatistics.

*/

#include <sstream>
#include <string>

#include <cassert>

#ifndef CASES_tqVXGkU1YS
#define CASES_tqVXGkU1YS

#include <gecode/int.hh>
#include <gecode/search.hh>

#include <Numerics/FloatingPoint.hpp>

#include "Conditions.hpp"
#include "Parsing.hpp"
#include "PartialSquares.hpp"
#include "Encoding.hpp"
#include "Constraints.hpp"
#include "Options.hpp"
#include "LookaheadReduction.hpp"

namespace Cases {

  using namespace Conditions;
  using namespace Parsing;
  using namespace PartialSquares;
  using namespace Encoding;
  using namespace Constraints;
  using namespace Options;
  using namespace LookaheadReduction;

  namespace FP = FloatingPoint;

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
    const size_t N) noexcept {
    std::istringstream in_cond(condstr);
    std::istringstream in_ps(psstr);
    const AConditions ac = ReadAC()(in_cond);
    const PSquares ps = PSquares(N, in_ps);
    const EncCond enc(ac, ps);
    return enc;
  }

  using space_ptr_t = std::unique_ptr<GenericMolsNB>;

  template <size_t N>
  struct Square {
  private:
    EncCond e;
  public:
    Square(const std::string psstr = "") :
      e(encoding("squares A\n", psstr, N)) {}
    //size_t solc() const noexcept { return FP::pow(N, N*N); }
    EncCond enc() const noexcept { return e; };
    space_ptr_t space() const noexcept {
      space_ptr_t m(new GenericMolsNB(e));
      m->status();
      return m;
    };
    ReductionStatistics laredstats(const LAR lar) const noexcept {
      space_ptr_t m = space();
      ReductionStatistics s(m->V);
      s.inc_rounds();
      for (auto i=0; i < FP::pow(N, 3); ++i) s.inc_probes();
      if (pruning(lar)) s.maxprune(FP::pow(N, 3));
      return s;
    }
  };

  template <size_t N>
  struct TrivialLatinSquare {
  private:
    EncCond e;
  public:
    TrivialLatinSquare(const std::string psstr = "") :
      e(encoding("squares A\nls A\n", psstr, N)) {}
    EncCond enc() const noexcept { return e; };
    space_ptr_t space() const noexcept {
      space_ptr_t m(new GenericMolsNB(e));
      m->status();
      return m;
    };
  };

}

#endif

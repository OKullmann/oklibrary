// Oleg Zaikin, 23.5.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  Test cases for lookahead-reduction and solvers.

  Namespace Cases, abbreviated "CS";

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

#ifndef CASES_tqVXGkU1YS
#define CASES_tqVXGkU1YS

#include <sstream>
#include <string>
#include <memory>

#include <cassert>

#include <gecode/int.hh>
#include <gecode/search.hh>

#include <Numerics/FloatingPoint.hpp>
#include <Transformers/Generators/Random/LatinSquares.hpp>

#include "Conditions.hpp"
#include "Parsing.hpp"
#include "PartialSquares.hpp"
#include "Encoding.hpp"
#include "Constraints.hpp"
#include "Options.hpp"
#include "LookaheadReduction.hpp"

namespace Cases {

  namespace GC = Gecode;

  namespace FP = FloatingPoint;
  namespace LS = LatinSquares;

  namespace CD = Conditions;
  namespace PG = Parsing;
  namespace PS = PartialSquares;
  namespace EC = Encoding;
  namespace CT = Constraints;
  namespace OP = Options;
  namespace LR = LookaheadReduction;

  using size_t = Conditions::size_t;
  using float_t = LR::float_t;


  class GenericMolsNB : public CT::GenericMols0 {
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
    GenericMolsNB(const EC::EncCond& enc) : GenericMols0(enc) {
      new (*this) Void(*this);
    }
  };

  typedef std::unique_ptr<GenericMolsNB> space_ptr_t;


  EC::EncCond encoding(const std::string& cond,
                       PS::PSquares ps) noexcept {
    return EC::EncCond(PG::ReadAC()(cond), ps);
  }
  EC::EncCond encoding(const std::string& cond, const std::string& ps,
                       const size_t N) noexcept {
    return encoding(cond, {N,ps});
  }


  struct Square {
    const size_t N;
    const size_t n = N*N;
    const size_t vals = N*n;

    const EC::EncCond e;

    Square(const size_t N_, const std::string psstr = "") :
      N(N_), e(encoding("squares A\n", psstr, N)) {
      assert(e.num_vars == n);
    }
    space_ptr_t space() const noexcept {
      space_ptr_t m(new GenericMolsNB(e)); m->status(); return m;
    };

    float_t numsol() const noexcept {
      if (e.ps.elimvals() == 0) return FP::pow(N, n);
      assert(e.ps.psqs.size() == 1);
      const auto& ps = e.ps.psqs[0].ps;
      float_t prod = 1;
      for (const auto& row : ps)
        for (const auto& c : row) prod *= c.vals();
      return prod;
    }

    LR::ReductionStatistics laredstats(const OP::LAR lar) const noexcept {
      LR::ReductionStatistics s(vals);
      s.inc_rounds();
      for (size_t i = 0; i < vals; ++i) s.inc_probes();
      if (pruning(lar)) s.maxprune(vals);
      return s;
    }
  };

  struct LaSq {
    const size_t N;
    const size_t n = N*N;
    const size_t vals = N*n;

    const EC::EncCond e;

    float_t numsol() const noexcept {
      const auto r = e.ps.restricted_count();
      if (r == 0) return LS::count_ls(N, LS::StRLS::none);
      else if (r == 1)
        return LS::count_ls(N, LS::StRLS::none) / N * (N - e.ps.elimvals());
      else return -1;
    }

    LaSq(const size_t N_, const std::string psstr = "") :
      N(N_), e(encoding("squares A\nls A\n", psstr, N)) {}
    space_ptr_t space() const noexcept {
      space_ptr_t m(new GenericMolsNB(e)); m->status(); return m;
    };
  };

}

#endif

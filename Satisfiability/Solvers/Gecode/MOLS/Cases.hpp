// Oleg Zaikin, 23.5.2022 (Swansea)
/* Copyright 2022, 2024 Oliver Kullmann
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
#include "GcVariables.hpp"

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
  namespace GV = GcVariables;

  using size_t = Conditions::size_t;
  using float_t = LR::float_t;

  struct Void : GC::Brancher {
    Void(const GC::Home home) : GC::Brancher(home) {}
    Void(GC::Space& home, Void& b) : GC::Brancher(home,b) {}
    GC::Brancher* copy(GC::Space& home) override {
      return new (home) Void(home,*this);
    }
    bool status(const GC::Space& s) const noexcept override {
      return not GcVariables::empty(static_cast<const CT::GenericMols0&>(s).V);
    }
    const GC::Choice* choice(GC::Space&) override {
      assert(0); return nullptr;
    }
    const GC::Choice* choice(const GC::Space&, GC::Archive&) override {
      assert(0); return nullptr;
    }
    GC::ExecStatus commit(GC::Space&, const GC::Choice&, unsigned) override {
      assert(0); return GC::ExecStatus(0);
    }
  };
  class GenericMolsNB : public CT::GenericMols1 {
  public :
    GenericMolsNB(const EC::EncCond& enc) : GenericMols1(enc) {
      new (*this) Void(*this);
    }
  };

  typedef std::unique_ptr<GenericMolsNB> space_ptr_t;


  EC::EncCond encoding(const std::string& cond,
                       PS::PSquares ps) {
    return EC::EncCond(PG::ReadAC()(cond), ps);
  }
  EC::EncCond encoding(const std::string& cond, const std::string& ps,
                       const size_t N) {
    return encoding(cond, {N,ps});
  }


  struct Square {
    const size_t N;
    const size_t n = N*N;
    const size_t vals = N*n;

    const EC::EncCond e;
    inline static const std::string cond = "squares A\n";

    Square(const size_t N, const std::string psstr = "") :
      N(N), e(encoding(cond, psstr, N)) {
      assert(e.num_vars == n);
    }
    Square(const size_t N, const PS::PSquares ps) :
      N(N), e(encoding(cond, ps)) {
      assert(ps.size() == 1);
      assert(ps.consistent());
      assert(e.N == ps.N);
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

    // One round of lookahead-reduction:
    LR::ReductionStatistics laredstats(const OP::RDL rdl,
                                       const OP::LAR lar,
                                       const OP::RT rt) const noexcept {
      const size_t ev = e.ps.elimvals(), units = e.ps.unit_count();
      assert(vals >= n+ev+units);
      assert(n >= units);
      const size_t mu0 = (vals - n) - ev, depth = 0;
      LR::ReductionStatistics s(mu0, n-units, depth);
      s.inc_rounds();
      for (size_t i = 0; i < n-units; ++i) s.inc_elimfv();
      switch (rdl) {
      case OP::RDL::basic : {
        const size_t probes = (vals - ev) - units;
        for (size_t i = 0; i < probes; ++i) s.inc_probes();
        if (pruning(lar)) s.maxprune(probes);
        break;
      }
      case OP::RDL::eaut : [[fallthrough]];
      case OP::RDL::efv : {
        s.inc_solc(); s.set_leaf();
        if (with_solutions(rt)) {
          const std::unique_ptr<GenericMolsNB> m = space();
          const GC::IntVarArray V = m->V;
          for (int v = 0; v < V.size(); ++v)
            if (V[v].size() != 1) GV::set_var(*m, V[v], 0);
          const auto status = m->status();
          assert(status == GC::SS_SOLVED);
          s.sollist(GV::extract(V));
        }
        break;
      }}
      return s;
    }

    bool operator ==(const Square&) const noexcept = default;
  };

  struct LaSq {
    const size_t N;
    const size_t n = N*N;
    const size_t vals = N*n;

    const EC::EncCond e;
    inline static const std::string cond = "squares A\nls A\n";

    LaSq(const size_t N, const std::string psstr = "") :
      N(N), e(encoding(cond, psstr, N)) {
      assert(e.num_vars == n);
    }
    LaSq(const size_t N, const PS::PSquares ps) :
      N(N), e(encoding(cond, ps)) {
      assert(e.N == ps.N);
      assert(e.num_vars == n);
    }

    space_ptr_t space() const noexcept {
      space_ptr_t m(new GenericMolsNB(e)); m->status(); return m;
    };

    float_t numsol() const noexcept {
      const auto r = e.ps.restricted_count();
      if (r == 0) return LS::count_ls(N, LS::StRLS::none);
      else if (r == 1)
        return LS::count_ls(N, LS::StRLS::none) / N * (N - e.ps.elimvals());
      else return -1;
    }

    // 1 round of lookahead-reduction.
    // If N==2, then 2 probes V[0]==0 and V[0]===1 are done,
    // each leads to one solution, so both values are eliminated,
    // and finally 1 propagation is done and a leaf is formed.
    // The pruning-set remains empty.
    // Else if N>2, no solutions are found, and N*N*N probes are done.
    // Maximal size of pruning-set is N*N*N as well.
    LR::ReductionStatistics laredstats(const OP::LAR lar,
      const OP::RT rt) const noexcept {
      LR::ReductionStatistics s(vals - n, n, 0);
      s.inc_rounds();
      if (N > 2) {
        for (size_t i = 0; i < vals; ++i) s.inc_probes();
        if (pruning(lar)) s.maxprune(vals);
      }
      else if (N == 2) {
        s.inc_probes();
        s.inc_elimvals();
        s.elim({0,0});
        s.inc_solc();
        s.set_leaf();
        if (with_solutions(rt)) s.sollist({0,1,1,0});
        if (not OP::test_sat(rt)) {
          s.inc_probes();
          s.inc_elimvals();
          s.elim({0,1});
          s.inc_solc();
          if (with_solutions(rt)) s.sollist({1,0,0,1});
          if (not (with_stop(rt))) s.inc_props();
        }
      }
      return s;
    }

    bool operator ==(const LaSq&) const noexcept = default;
  };

}

#endif

// Oleg Zaikin, 11.5.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Helper functions for dealing with Gecode variables

  Namespace GcVariables, abbreviated "GV".

    - using size_t

    - typdef VarVec = GC::IntVarArray

    - out(std::ostream*, VarVec)

    - typedef values_t = std::vector<int>
    - typedef solutions_t = std::vector<int>
    - typedef domsizes_t = std::vector<size_t

    - class GcIntArraySpace, derived from GC::Space,
      a wrapper around VarVec
    - class GcIntVarArray, a wrapper around GcIntArraySpace,
      to simulate a "pure" VarVec

  Helper functions:

    - empty(VarVec) -> bool
    - domsizes(VarVec) -> domsizes_t
    - sumdomsizes(VarVec) -> size_t
    - values(VarVec V, int v) -> values_t
    - values(VarVec) -> std::vector<values_t>
    - extract(VarVec) -> solutions_t (assumes all variables are assigned)

  Branching:

    - set_var(GC::Space, GC::IntVar v, int val)
    - unset_var(GC::Space, GC::IntVar v, int val)

    - gcbv(VarVec, OP::BHV) -> int (Gecode-branching-variable)

BUG:

TODOS:

1. set_var, unset_var need to be tested
    - This needs status().
    - Using a different functionality, based on views, has the additional
      advantage that set_var, unset_var don't need parameter s.
    - But first proper test-cases are needed.

2. Test gcbv.

*/

#ifndef GCVARIABLES_FQM6cOCEcZ
#define GCVARIABLES_FQM6cOCEcZ

#include <vector>
#include <ostream>

#include <cassert>

#include <gecode/int.hh>
#include <gecode/search.hh>

#include <Numerics/NumTypes.hpp>

#include "Conditions.hpp"
#include "Options.hpp"

namespace GcVariables {

  namespace GC = Gecode;

  namespace FP = FloatingPoint;
  namespace OP = Options;
  namespace CD = Conditions;

  using size_t = CD::size_t;

  typedef GC::IntVarArray VarVec;
  void out(std::ostream& o, const VarVec& V) {
    for (int v = 0; v < V.size(); ++v) {
      o << v << ":";
      for (GC::IntVarValues j(V[v]); j(); ++j) o << " " << j.val();
      o << "\n";
    }
  }


  struct GcIntArraySpace : GC::Space {
    const int n, dom;
    VarVec V;
    GcIntArraySpace(const size_t n, const size_t dom = 1) noexcept :
      n(n), dom(dom), V(*this, n, 0, dom-1) {
      assert(n > 0 and dom > 0);
    }
  protected :
    GcIntArraySpace(GcIntArraySpace& gm) :
      GC::Space(gm), n(gm.n), dom(gm.dom), V(gm.V) {
      V.update(*this, gm.V);
    }
    GC::Space* copy() override { return new GcIntArraySpace(*this); }
  };

  class GcIntVarArray {
    typedef std::unique_ptr<GcIntArraySpace> intarr_ptr_t;
    const intarr_ptr_t m;
    const VarVec V;
  public:
    GcIntVarArray(const size_t n, const size_t dom = 1)
      noexcept : m(new GcIntArraySpace(n, dom)), V(m->V) {
        assert(n > 0 and dom > 0);
      }
    operator VarVec() const noexcept { return V; }
    const auto& operator [](const int i) const { return V[i]; }
    auto& operator [](const int i) { return V[i]; }
  };


  bool empty(const VarVec& V) noexcept {
    for (int i = 0; i < V.size(); ++i) if (not V[i].assigned()) return false;
    return true;
  }

  typedef std::vector<size_t> domsizes_t;
  domsizes_t domsizes(const VarVec& V) noexcept {
    domsizes_t res; res.reserve(V.size());
    for (int v = 0; v < V.size(); ++v) res.push_back(V[v].size());
    return res;
  }

  size_t sumdomsizes(const VarVec& V) noexcept {
    size_t sum = 0;
    for (int v = 0; v < V.size(); ++v) sum += V[v].size();
    return sum;
  }

  typedef std::vector<int> values_t;
  values_t values(const VarVec& V, const int v) {
    assert(v >= 0 and v < V.size());
    values_t res;
    for (GC::IntVarValues j(V[v]); j(); ++j) res.push_back(j.val());
    assert(res.size() == size_t(V[v].size()));
    return res;
  }
  std::vector<values_t> values(const VarVec& V) {
    std::vector<values_t> res;
    for (int v=0; v<V.size(); ++v) {
      values_t vec;
      for (GC::IntVarValues j(V[v]); j(); ++j) vec.push_back(j.val());
      assert(vec.size() == size_t(V[v].size()));
      res.push_back(vec);
    }
    assert(res.size() == size_t(V.size()));
    return res;
  }
  typedef std::vector<int> solutions_t;
  solutions_t extract(const VarVec& V) {
    const size_t N = V.size();
    solutions_t res; res.reserve(N);
    for (size_t v = 0; v < N; ++v) {
      const GC::Int::IntView view = V[v];
      assert(view.assigned());
      const GC::IntVarValues j(view);
      res.push_back(j.val());
    }
    return res;
  }


  void set_var(GC::Space& s, const GC::IntVar& v, const int val) noexcept {
    GC::rel(s, v, GC::IRT_EQ, val);
  }
  void unset_var(GC::Space& s, const GC::IntVar& v, const int val) noexcept {
    GC::rel(s, v, GC::IRT_NQ, val);
  }


  // Determining a variable according to gc-branching-heuristic bv:
  int gcbv(const VarVec& V, const OP::BHV bv) noexcept {
    const auto size = V.size();
    assert(size >= 1);
    switch (bv) {
    case OP::BHV::first :
      for (int v = 0; v < size; ++v) if (V[v].size() != 1) return v;
      assert(false); [[fallthrough]];
    case OP::BHV::mindeg : {
      int min = std::numeric_limits<int>::max(), opt=-1;
      for (int v = 0; v < size; ++v) {
        if (V[v].size() == 1) continue;
        const int deg = V[v].degree();
        if (deg < min) { min = deg; opt = v; }
      }
      assert(opt >= 0); return opt;
    }
    case OP::BHV::maxdeg : {
      int max = -1, opt=-1;
      for (int v = 0; v < size; ++v) {
        if (V[v].size() == 1) continue;
        const int deg = V[v].degree();
        if (deg > max) { max = deg; opt = v; }
      }
      assert(opt >= 0); return opt;
    }
    case OP::BHV::mindom : {
      int min = std::numeric_limits<int>::max(), opt=-1;
      for (int v = 0; v < size; ++v) {
        if (V[v].size() == 1) continue;
        const int dom = V[v].size();
        if (dom < min) { min = dom; opt = v; }
      }
      assert(opt >= 0); return opt;
    }
    case OP::BHV::maxdom : {
      int max = -1, opt=-1;
      for (int v = 0; v < size; ++v) {
        if (V[v].size() == 1) continue;
        const int dom = V[v].size();
        if (dom > max) { max = dom; opt = v; }
      }
      assert(opt >= 0); return opt;
    }
    case OP::BHV::mindegdom : {
      float_t min = FP::pinfinity; int opt=-1;
      for (int v = 0; v < size; ++v) {
        if (V[v].size() == 1) continue;
        const float_t q = float_t(V[v].degree()) / V[v].size();
        if (q < min) { min = q; opt = v; }
      }
      assert(opt >= 0); return opt;
    }
    case OP::BHV::maxdegdom : {
      float_t max = FP::minfinity; int opt=-1;
      for (int v = 0; v < size; ++v) {
        if (V[v].size() == 1) continue;
        const float_t q = float_t(V[v].degree()) / V[v].size();
        if (q > max) { max = q; opt = v; }
      }
      assert(opt >= 0); return opt;
    }
    default : assert(false); return -1;}
  }

}

#endif

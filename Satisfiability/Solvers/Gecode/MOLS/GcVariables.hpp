// Oleg Zaikin, 11.5.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Helper functions for dealing with Gecode variables

  Namespace GcVariables, abbreviated "GV".


BUG:

1. gcintarr leads to undefined behaviour:
    - DONE The object of the returned VarVec is immediately destroyed.
    - DONE The solution is to use implicit conversion of
      GecodeIntVarArray to VarVec (so that the GecodeIntVarArray-
      object is kept), and providing members of VarVec.
    - The name "GecodeIntVarArray" is also inappropriate (and too
      long).
    - "GcIntVarArray" is somewhat better.

TODOS:

1. set_var, unset_var need to be tested
    - This needs status().
    - Using a different functionality, based on views, has the additional
      advantage that set_var, unset_var don't need parameter s.
    - But first proper test-cases are needed.

*/

#ifndef GCVARIABLES_FQM6cOCEcZ
#define GCVARIABLES_FQM6cOCEcZ

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


  struct GenericIntArray : GC::Space {
    VarVec V;
    GenericIntArray(const size_t varnum, const size_t domainsize = 1)
      noexcept : V(*this, varnum, 0, domainsize-1) {
      assert(varnum > 0 and domainsize > 0);
    }
  protected :
    GenericIntArray(GenericIntArray& gm) : GC::Space(gm), V(gm.V) {
      V.update(*this, gm.V);
    }
    GC::Space* copy() override { return new GenericIntArray(*this); }
  };

  class GecodeIntVarArray {
    typedef std::unique_ptr<GenericIntArray> intarr_ptr_t;
    const intarr_ptr_t m;
    const VarVec V;
  public:
    GecodeIntVarArray(const size_t varnum, const size_t domainsize = 1)
      noexcept : m(new GenericIntArray(varnum, domainsize)), V(m->V) {
      assert(varnum > 0 and domainsize > 0);
    }
    operator VarVec() const noexcept { return V; }
    const auto& operator [](const int i) const { return V[i]; }
    auto& operator [](const int i) { return V[i]; }
  };


  void out(std::ostream& o, const GC::IntVarArray& V) {
    for (int v = 0; v < V.size(); ++v) {
      o << v << ":";
      for (GC::IntVarValues j(V[v]); j(); ++j) o << " " << j.val();
      o << "\n";
    }
  }


  bool empty(const GC::IntVarArray& V) noexcept {
    for (int i = 0; i < V.size(); ++i) if (not V[i].assigned()) return false;
    return true;
  }

  size_t sumdomsizes(const GC::IntVarArray& V) noexcept {
    size_t sum = 0;
    for (int v = 0; v < V.size(); ++v) sum += V[v].size();
    return sum;
  }

  typedef std::vector<int> values_t;
  values_t values(const GC::IntVarArray& V, const int v) {
    assert(v >= 0 and v < V.size());
    values_t res;
    for (GC::IntVarValues j(V[v]); j(); ++j) res.push_back(j.val());
    assert(res.size() == size_t(V[v].size()));
    return res;
  }
  std::vector<values_t> values(const GC::IntVarArray& V) {
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
  solutions_t extract(const GC::IntVarArray& V) {
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


  int gcbv(const GC::IntVarArray& V, const OP::BHV bv) noexcept {
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

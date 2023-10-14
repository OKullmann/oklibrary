// Oliver Kullmann, 21.6.2022 (Swansea)
/* Copyright 2022, 2023 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  Measures and distances

  Namespace Measures, abbreviated "MS".

  Imported typedefs:

   - size_t = CD::size_t
   - VarVec = GcVariables::VarVec
   - domsizes_t = GcVariables::domsizes_t
   - float_t = FP::float80

  Helper functions:

   - tr(int) -> size_t (translating int to size_t, possibly checked)
   - mumi(VarVec) -> float_t (number of unassigned variables)

  Logarithmic weights

   - wmuld(size_t) -> float_t

  Measures:

   - wnumvars(VarVec, weights_t*) -> float_t
   - wnumvars(domsizes_t, weights_t*) -> float_t

   - typedef canonical_measures_t = array<float_t, 3>
     component 0 : mu0 = sum of (domain-size - 1)
     component 1 : mu1 = sum of binary logarithms of domain-size
     component 2 : mu2 = number of unassigned variables

   Special cases:
    - muap(VarVec) -> float_t
    - muap(domsizes_t) -> float_t
    - muld(VarVec) -> float_t
    - muld(domsizes_t) -> float_t

  Distances:

   - new_vars(VarVec, VarVec, weights_t*, size_t) -> float)t
   - new_vars(domsizes_t, VarVec, weights_t*, size_t) -> float)t

*/

#ifndef MEASURES_MzdnyXKN5C
#define MEASURES_MzdnyXKN5C

#include <array>
#include <limits>
#include <type_traits>
#include <functional>

#include <cassert>

#include <gecode/int.hh>

#include <Numerics/NumTypes.hpp>
#include <Numerics/NumBasicFunctions.hpp>

#include "Conditions.hpp"
#include "Options.hpp"
#include "GcVariables.hpp"

namespace Measures {

  namespace GC = Gecode;
  namespace FP = FloatingPoint;
  namespace CD = Conditions;
  namespace OP = Options;

  using size_t = CD::size_t;

  using VarVec = GcVariables::VarVec;
  using domsizes_t = GcVariables::domsizes_t;
  using degrees_t = GcVariables::degrees_t;

  typedef FP::float80 float_t;

  // Converting int to size_t (checked):
  inline constexpr size_t tr(const int size,
                           [[maybe_unused]] const size_t bound = 0) noexcept {
    assert(bound <= std::numeric_limits<int>::max());
    assert(size >= int(bound));
    return size;
  }


  // The logarithmic weights:
  constexpr size_t N_muld = 100;
  static_assert(N_muld >= 10);
  typedef std::array<float_t, N_muld+1> given_wfloat_t;
  consteval given_wfloat_t init_wmuld() noexcept {
    static_assert(std::is_same_v<float_t, FP::float80>);
    given_wfloat_t res; res[0] = FP::minfinity;
    for (size_t i = 2; i <= N_muld; ++i) res[i] = FP::log2(i);
    return res;
  }
  constexpr given_wfloat_t given_wmuld = init_wmuld();
  static_assert(given_wmuld.size() == N_muld+1);
  static_assert(given_wmuld[0] == FP::minfinity);
  static_assert(given_wmuld[1] == 0); static_assert(given_wmuld[2] == 1);
  static_assert(given_wmuld[4] == 2);
  constexpr float_t wmuld(const size_t i) noexcept {
    static_assert(std::is_same_v<float_t, FP::float80>);
    return i <= N_muld ? given_wmuld[i] : FP::log2(i);
  }
  static_assert(wmuld(0) == FP::minfinity); static_assert(wmuld(8) == 3);


  /* Measures */

  // Domain-size D gets weight w[D]:
  float_t wnumvars(const VarVec& V,
                       const OP::weights_t* const w) noexcept {
    float_t sum = 0;
    for (int v = 0; v < V.size(); ++v) {
      const size_t s = tr(V[v].size(), 1); assert(s < w->size());
      sum += (*w)[s];
    }
    return sum;
  }
  float_t wnumvars(const domsizes_t& V,
                       const OP::weights_t* const w) noexcept {
    float_t sum = 0;
    for (size_t v = 0; v < V.size(); ++v) {
      const size_t s = V[v]; assert(s < w->size());
      sum += (*w)[s];
    }
    return sum;
  }

  float_t wnumvars_muldeg(const VarVec& V,
                          const OP::weights_t* const w) noexcept {
    float_t sum = 0;
    for (int v = 0; v < V.size(); ++v) {
      const auto& var = V[v];
      const size_t s = tr(var.size(), 1); assert(s < w->size());
      const auto d = var.degree();
      sum += (*w)[s] * d;
    }
    return sum;
  }
  float_t wnumvars_muldeg(const domsizes_t& V, const degrees_t& Vd,
                          const OP::weights_t* const w) noexcept {
    const size_t size = V.size();
    assert(size == Vd.size());
    float_t sum = 0;
    for (size_t v = 0; v < size; ++v) {
      const size_t s = V[v]; assert(s < w->size());
      const size_t d = Vd[v];
      sum += (*w)[s] * d;
    }
    return sum;
  }

  // A variable of domain-size D has weight D-1:
  float_t muap(const VarVec& V) noexcept { // mu0
    const int size = V.size(); float_t sum = - float_t(size);
    for (int v = 0; v < size; ++v) sum += tr(V[v].size(), 1);
    return sum;
  }
  float_t muap(const domsizes_t& V) noexcept { // mu0
    const size_t size = V.size(); float_t sum = - float_t(size);
    for (size_t v = 0; v < size; ++v) sum += V[v];
    return sum;
  }

  // The binary logarithm of the number of total assignments:
  float_t muld(const VarVec& V) noexcept { // mu1
    const int size = V.size(); float_t sum = 0;
    for (int v = 0; v < size; ++v) sum += wmuld(tr(V[v].size(), 1));
    return sum;
  }
  float_t muld(const domsizes_t& V) noexcept {
    const size_t size = V.size(); float_t sum = 0;
    for (size_t v = 0; v < size; ++v) sum += wmuld(V[v]);
    return sum;
  }

  // The number of unassigned variables:
  float_t mumi(const VarVec& V) noexcept { // mu2
    const int size = V.size(); float_t sum = 0;
    for (int v = 0; v < size; ++v) sum += tr(V[v].size(), 1) > 1;
    return sum;
  }
  typedef std::array<float_t, 3> canonical_measures_t;
  canonical_measures_t muall(const VarVec& V) noexcept { // mu0/1/2
    const int size = V.size(); canonical_measures_t res;
    res[0] = - float_t(size);
    for (int v = 0; v < size; ++v) {
      const auto ds = tr(V[v].size(), 1);
      res[0] += ds; res[1] += wmuld(ds); res[2] += ds > 1;
    }
    return res;
  }

  float_t initial_measure(const domsizes_t& V0, const degrees_t& V0deg,
                          const OP::weights_t* const w,
                          const OP::DIS d) noexcept {
    if (OP::is_newv(d)) return 0;
    assert(OP::is_deltaL(d));
    if (d == OP::DIS::wdeltaL) return wnumvars(V0, w);
    assert(d == OP::DIS::wdeltaL_degm);
    return wnumvars_muldeg(V0, V0deg, w);
  }


  /* Distances */

  float_t new_vars(const VarVec& V, const VarVec& nV,
                   const OP::weights_t* const w,
                   const size_t depth) noexcept {
    float_t sum = 0;
    const float_t w1 = FP::exp2((*w)[1] * depth);
    for (int v = 0; v < V.size(); ++v) {
      const size_t s = tr(V[v].size(), 1), sn = tr(nV[v].size(), 1);
      if (sn == s) continue;
      assert(sn < s);
      if (sn == 1) sum += w1;
      else { assert(sn < w->size()); sum += (*w)[sn]; }
    }
    return sum;
  }
  float_t new_vars(const domsizes_t& V, const VarVec& nV,
                   const OP::weights_t* const w,
                   const size_t depth) noexcept {
    float_t sum = 0;
    const float_t w1 = FP::exp2((*w)[1] * depth);
    for (size_t v = 0; v < V.size(); ++v) {
      const size_t s = V[v], sn = tr(nV[v].size(), 1);
      if (sn == s) continue;
      assert(sn < s);
      if (sn == 1) sum += w1;
      else { assert(sn < w->size()); sum += (*w)[sn]; }
    }
    return sum;
  }

  float_t new_vars_muldeg(const domsizes_t& V, const degrees_t& Vdeg,
                          const VarVec& nV,
                          const OP::weights_t* const w,
                          const size_t depth) noexcept {
    float_t sum = 0;
    const float_t w1 = FP::exp2((*w)[1] * depth);
    for (size_t v = 0; v < V.size(); ++v) {
      const auto& newvar = nV[v];
      const size_t oldsize = V[v], newsize = tr(newvar.size(), 1);
      if (newsize == oldsize) continue;
      assert(newsize < oldsize);
      if (newsize == 1) {
        const size_t olddeg = Vdeg[v];
        sum += w1 * olddeg;
      }
      else {
        const auto newdeg = newvar.degree();
        assert(newsize < w->size());
        sum += (*w)[newsize] * newdeg;
      }
    }
    return sum;
  }

  typedef std::function<float_t(const VarVec&)> measure_t;
  typedef std::function<float_t(const VarVec&)> distance_t;

  distance_t distance_choice(const domsizes_t& V0,
                             const degrees_t& V0deg,
                             const OP::weights_t* const w,
                             const size_t depth,
                             const float_t mu0,
                             const OP::DIS d) noexcept {
    assert(OP::deltaL_or_newv(d));
    if (OP::is_newv(d)) {
      if (d == OP::DIS::newvars)
        return distance_t([&V0,w,depth](const VarVec& nV)noexcept{
                            return new_vars(V0, nV, w, depth);});
      assert(d == OP::DIS::newvars_degm);
      return distance_t([&V0,&V0deg,w,depth](const VarVec& nV)noexcept{
                          return new_vars_muldeg(V0,V0deg, nV, w, depth);});
    }
    else {
      if (d == OP::DIS::wdeltaL)
        return distance_t([w,mu0](const VarVec& nV)noexcept{
                            return mu0 - wnumvars(nV, w);});
      assert(d == OP::DIS::wdeltaL_degm);
      return distance_t([w,mu0](const VarVec& nV)noexcept{
                          return mu0 - wnumvars_muldeg(nV, w);});
    }
  }

}

#endif

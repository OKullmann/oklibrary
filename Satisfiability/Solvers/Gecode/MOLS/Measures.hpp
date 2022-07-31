// Oliver Kullmann, 21.6.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  Measures and distances

  Namespace Measures, abbreviated "MS".

*/

#ifndef MEASURES_MzdnyXKN5C
#define MEASURES_MzdnyXKN5C

#include <array>
#include <limits>
#include <type_traits>

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

  typedef FP::float80 float_t;

  // Converting int to size_t (checked):
  inline constexpr size_t tr(const int size,
                           [[maybe_unused]] const size_t bound = 0) noexcept {
    assert(bound <= std::numeric_limits<int>::max());
    assert(size >= int(bound));
    return size;
  }


  // The logarithmic weights:
  typedef FP::float80 muld_t;
  constexpr size_t N_muld = 100;
  static_assert(N_muld >= 10);
  typedef std::array<muld_t, N_muld+1> given_wmuld_t;
  consteval given_wmuld_t init_wmuld() noexcept {
    static_assert(std::is_same_v<muld_t, FP::float80>);
    given_wmuld_t res; res[0] = FP::minfinity;
    for (size_t i = 2; i <= N_muld; ++i) res[i] = FP::log2(i);
    return res;
  }
  constexpr given_wmuld_t given_wmuld = init_wmuld();
  static_assert(given_wmuld.size() == N_muld+1);
  static_assert(given_wmuld[0] == FP::minfinity);
  static_assert(given_wmuld[1] == 0); static_assert(given_wmuld[2] == 1);
  static_assert(given_wmuld[4] == 2);
  constexpr muld_t wmuld(const size_t i) noexcept {
    static_assert(std::is_same_v<muld_t, FP::float80>);
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

  // The number of non-assigned variables:
  float_t mumi(const VarVec& V) noexcept { // mu2
    const int size = V.size(); float_t sum = 0;
    for (int v = 0; v < size; ++v) sum += tr(V[v].size(), 1) > 1;
    return sum;
  }
  typedef std::array<float_t, 3> canonical_measures_t;
  canonical_measures_t muall(const VarVec& V) noexcept { // mu0-2
    const int size = V.size(); canonical_measures_t res;
    res[0] = - float_t(size);
    for (int v = 0; v < size; ++v) {
      const auto ds = tr(V[v].size(), 1);
      res[0] += ds; res[1] += wmuld(ds); res[2] += ds > 1;
    }
    return res;
  }


  /* Distances */

  float_t new_vars(const VarVec& V, const VarVec& nV,
                   const OP::weights_t* const w,
                   const size_t depth) noexcept {
    float_t sum = 0;
    const float_t w1 = FP::exp2((*w)[1] * depth);
    for (int v = 0; v < V.size(); ++v) {
      const size_t s = tr(V[v].size(), 1), sn = tr(nV[v].size(), 1);
      if (sn == s) continue; assert(sn < s);
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
      if (sn == s) continue; assert(sn < s);
      if (sn == 1) sum += w1;
      else { assert(sn < w->size()); sum += (*w)[sn]; }
    }
    return sum;
  }

}

#endif

// Oliver Kullmann, 29.4.2021 (Swansea)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  Partial assignments

*/

#ifndef PARTIALASSIGNMENTS_hicvHzc11p
#define PARTIALASSIGNMENTS_hicvHzc11p

#include <vector>
#include <exception>

#include <cassert>

#include "VarLit.hpp"
#include "ClauseSets.hpp"

namespace MAUT {

  enum class PA : LIT {o=0, t=1, f=-1 };
  inline constexpr PA operator -(const PA p) noexcept {
    return PA(-LIT(p));
  }
  static_assert(-PA::o == PA::o);
  static_assert(-PA::t == PA::f);
  static_assert(-PA::f == PA::t);
  inline constexpr PA neg(const PA p, const LIT x) noexcept {
    if (x > 0) return p;
    else if (x == 0) return PA::o;
    else return -p;
  }

  typedef std::vector<PA> PASS;

  struct Pass {

    const VAR n;

    explicit Pass(const VAR n) : n(n), vec(n+1) {}

    PA operator[] (const VAR v) const noexcept {
      assert(valid(v) and v <= n);
      return vec[v];
    }
    PA& operator[] (const VAR v) noexcept {
      assert(valid(v) and v <= n);
      return vec[v];
    }
    PA operator() (const LIT x) const noexcept {
      assert(valid(x));
      const VAR v = var(x);
      assert(v <= n);
      return neg(vec[v], x);
    }
    void set(const LIT x, const PA p) noexcept {
      assert(valid(x));
      if (x >= 0) vec[var(x)] = p;
      else vec[var(x)] = -p;
    }

    PA at(const VAR v) const {
      assert(valid(v));
      if (v > n)
        throw std::out_of_range(
          "Pass::at, v=" + std::to_string(v) + ", n=" + std::to_string(n));
      return vec[v];
    }
    PA& at(const VAR v) {
      assert(valid(v));
      if (v > n)
        throw std::out_of_range(
          "Pass::at, v=" + std::to_string(v) + ", n=" + std::to_string(n));
      return vec[v];
    }

  private :

    PASS vec;

  };


  bool sat(const Pass& pa, const CL& C) {
    return std::any_of(C.begin(), C.end(),
                       [&pa](const LIT x){return pa(x)==PA::t;});
  }

  bool occurs(const LIT x, const Pass& pa, const ClauseSet& F) noexcept {
    assert(valid(x) and not singular(x));
    assert(var(x) <= pa.n and var(x) <= F.occ.n);
    assert(pa(var(x)) == PA::o);
    for (const LitOcc Cp : F.occ[x]) if (not sat(pa, *Cp)) return true;
    return false;
  }


  size_t add_pure(Pass& pa, const ClauseSet& F) {
    const VAR n = F.dp.n;
    assert(n == F.occ.n and n == pa.n);
    size_t count = 0;
    for (VAR v = 1; v <= n; ++v) {
      if (pa[v] != PA::o) continue;
      if (not occurs(lit(v,1), pa, F)) {
        pa[v] = PA::t; ++count; continue;
      }
      if (not occurs(lit(v,-1), pa, F)) {
        pa[v] = PA::f; ++count;
      }
    }
    return count;
  }

}

#endif
